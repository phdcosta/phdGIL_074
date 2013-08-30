#include "phdControlManager.h"

//--------------------------------------------------------------------------------------------------------------
phdControlLink::phdControlLink(phdControlBase * _parent, phdControlLink * _remote) {
	parentControl = _parent;
	remoteLink = _remote;
}

void phdControlLink::setRemoteLink(phdControlLink * _remoteLink) {
	remoteLink = _remoteLink;
}

bool phdControlLink::pointInside(float _x, float _y) {
	return (ofDistSquared(_x, _y, x, y) <= ofDistSquared(0, 0, (float)(linkSize*0.5), (float)(linkSize*0.5) ) );
}

void phdControlLink::draw() {
	ofCircle(x, y, (float)(linkSize * 0.5));
}

//--------------------------------------------------------------------------------------------------------------
phdControlLinks::phdControlLinks(phdControlBase * _parent, phdControlLinksDrawMode _drawMode) {
	parent = _parent;
	focusedIndex = -1;
}

void phdControlLinks::setParentControl(phdControlBase * _parent) {
	parent = _parent;
	for(int i = 0; i < links.size(); i++) {
		links[i].parentControl = _parent;
	}
}

phdControlLink * phdControlLinks::addLink(phdControlLink * _remoteLink) {
	links.push_back(phdControlLink(parent, _remoteLink));
	return &links[links.size()-1];
};

void phdControlLinks::linkIndexedToRemoteLink(int _index, phdControlLink * _remoteLink) {
	if(_index > -1 && _index < links.size()) {
		links[_index].setRemoteLink(_remoteLink);
	}
}

void phdControlLinks::setDrawMode(phdControlLinksDrawMode _drawMode) {
	drawMode = _drawMode;
	setPosition(x, y);
}

void phdControlLinks::setPosition(float _x, float _y) {

	if(_x == x && _y == y) return;

	x = _x;
	y = _y;

	for(int i = 0; i < links.size(); i++) {
		switch(drawMode) {
		case ldmHorLeft :
			links[i].setPosition(_x - (float)((i + 0.5) * linkSize * 1.25), _y);
			break;
		case ldmHorRight :
			links[i].setPosition(_x + (float)((i + 0.5) * linkSize * 1.25), _y);
			break;
		case ldmVerTop :
			links[i].setPosition(_x, _y - (float)((i + 0.5) * linkSize * 1.25));
			break;
		case ldmVerBottom :
			links[i].setPosition(_x, _y + (float)((i + 0.5) * linkSize * 1.25));
			break;
		}
	}
}

bool phdControlLinks::updateHitTestInfo(float _x, float _y) {
	for(int i = 0; i < links.size(); i++) {
		if(links[i].pointInside(_x, _y)) {
			focusedIndex = i;
			return true;
		}
	}
	focusedIndex = -1;
	return false;
}

void phdControlLinks::draw(float _x, float _y) {

	ofPushStyle();

	setPosition(_x, _y);

	ofNoFill();
	ofSetColor(192, 192, 255, 255);
	for(int i = 0; i < links.size(); i++) {
		if(focusedIndex == i) ofFill();
		links[i].draw();
		if(focusedIndex == i) ofNoFill();

		if(links[i].remoteLink != NULL) {
			phdControlLink * _remoteLink = links[i].remoteLink;
			ofLine(links[i].x, links[i].y, _remoteLink->x, _remoteLink->y);
		}
	}
	ofPopStyle();
}

//----------------------------------------------------------------------------------------------------------------------

phdControlBase::phdControlBase(phdControlManager * _manager) {
	selected = focused = dragging = visible = false;
	x = y = width = height = 10.0;
	setPopupMenu(NULL);
	inputLinks.setDrawMode(ldmHorLeft);
	outputLinks.setDrawMode(ldmHorLeft);
}

bool phdControlBase::updateHitTestInfo(float _x, float _y) {
	phdHitTestInfoType _result = ghtOutside;
	if(_x >= getX() && _y >= getY() && _x <= getX() + getWidth() && _y <= getY() + getHeight()) _result =  ghtInside;
	if(inputLinks.updateHitTestInfo(_x, _y)) _result =  ghtInputLink;
	if(outputLinks.updateHitTestInfo(_x, _y)) _result =  ghtOutputLink;
	return (_result != ghtOutside);
}

void phdControlBase::populateGui(phdControlBase * _gui) {
}

void phdControlBase::update() {
}

void phdControlBase::draw() {

	ofPushStyle();

	ofNoFill();
	ofSetColor(255,255,255,255);
	if(selected) ofSetColor(255,255,0,255);
	if(focused && !selected) ofSetColor(255,0,100,255);
	ofRect(getX(), getY(), getWidth(), getHeight());

	inputLinks.draw(getX()-(float)(linkSize*0.75), getY()-(float)(linkSize*0.75));
	outputLinks.draw(getX()-(float)(linkSize*0.75), getY()+getHeight()+(float)(linkSize*0.75));

	ofDrawBitmapString(ofToString(inputLinks.focusedIndex), getX() + 10.0, getY() + 10.0);
	ofDrawBitmapString(ofToString(outputLinks.focusedIndex), getX() + 10.0, getY() + 25.0);

	ofPopStyle();
}

void phdControlBase::onMouseMoved(ofMouseEventArgs& args) {
}

void phdControlBase::onMousePressed(ofMouseEventArgs& args) {
}

void phdControlBase::onMouseDragged(ofMouseEventArgs& args) {
}

void phdControlBase::onMouseReleased(ofMouseEventArgs& args) {
}

void phdControlBase::onPopupClick(phdPopupMenuEventData &_eventData) {
	if(_eventData.menu != NULL) {
		ofRemoveListener(_eventData.menu->popEvent, this, &phdControlBase::onPopupClick);
	}
}

string phdControlBase::getSetupString() {
	return "";
}

void phdControlBase::saveToStrings(vector<string> & _strings, string _title) {
}

void phdControlBase::saveToAttributes(phdAttributeList & _attributes) {

}

void phdControlBase::loadFromAttributes(phdAttributeList & _attributes) {

}

//----------------------------------------------------------------------------------------------------------------------
phdControlManager::phdControlManager() {
	usingEvents = false;
	selectedControl = NULL;
	focusedControl = NULL;
	popupMenu = NULL;
	setDrawMode(cmdmBoth);
	enableEvents();
}

phdControlManager::~phdControlManager() {
	resetFocus();
	disableEvents();
	freeControls();
}

void phdControlManager::freeControls() {
	for(int i = 0; i < controls.size(); i++) {
		delete controls[i]; controls[i] = NULL;
	}
	controls.clear();
}

void phdControlManager::deleteItem(int _index, bool _freeMem) {
	if(_index > -1 && _index < controls.size()) {
		if(_freeMem) { delete controls[_index]; controls[_index] = NULL; }
		controls.erase(controls.begin() + _index);
	}
}

void phdControlManager::setWarpersDrawMode(string _drawMode) {
	for(int i = controls.size()-1; i > -1; i--) {
		if(controls[i]->getClassName() == "PHDPROJECTOR") {
			controls[i]->setupFromString("DRAWMODE|" + _drawMode + "|");
		}
	}
}

void phdControlManager::setDrawMode(phdControlManagerDrawMode _drawMode) {
	if(drawMode != _drawMode) {
		drawMode = _drawMode;
	}
}

void phdControlManager::setSelectedControl(phdControlBase * _control, bool _autoFocus) {
	if(selectedControl != _control) {
		if(_autoFocus && selectedControl != NULL)	selectedControl->setSelected(false);

		selectedControl = _control;

		if(_autoFocus && selectedControl != NULL) {
			selectedControl->setSelected(true);
			int _index = indexOf(selectedControl);
			if(_index != -1) {
				controls.erase(controls.begin() + _index);
				//controls.push_back(selectedControl);
				controls.insert(controls.begin(), selectedControl);
			}
		}
	}
}

phdControlBase * phdControlManager::getSelectedControl() {
	return selectedControl;
}

void phdControlManager::setFocusedControl(phdControlBase * _control, bool _autoFocus) {
	if(focusedControl != _control) {
		if(_autoFocus && focusedControl != NULL) focusedControl->setFocused(false);
		focusedControl = _control;
		if(_autoFocus && focusedControl != NULL) focusedControl->setFocused(true);
	}
}

phdControlBase * phdControlManager::getFocusedControl() {
	return focusedControl;
}

int phdControlManager::indexOf(phdControlBase * _control) {
	for(int i = 0; i < controls.size(); i++) {
		if(controls[i] == _control) return i;
	}
	return -1;
}

int phdControlManager::addControl(phdControlBase * _control) {
	if(_control != NULL) {
		_control->setManager(this);
		controls.push_back(_control);
	}
	return controls.size()-1;
}

void phdControlManager::delControl(phdControlBase * _control) {
	int _index = indexOf(_control);
	delControl(_index);
}

void phdControlManager::delControl(int _index) {
	if(_index > -1 && _index < controls.size()) {
		delete controls[_index]; controls[_index] = NULL;
		controls.erase(controls.begin() + _index);
	}
}

void phdControlManager::update() {
	for(int i = 0; i < controls.size(); i++) controls[i]->update();
}

void phdControlManager::draw() {

	// draw controls
	if(drawMode == cmdmControls || drawMode == cmdmBoth) {
		for(int i = controls.size()-1; i > -1; i--) {
			if(controls[i] != selectedControl && controls[i]->getClassName() != "PHDPROJECTOR") controls[i]->draw();
		}
	}

	// draw selected control
	if(selectedControl != NULL && selectedControl->getClassName() != "PHDPROJECTOR") selectedControl->draw();
}

void phdControlManager::drawWarpers() {
	// draw projectors	
	if(drawMode == cmdmWarpers || drawMode == cmdmBoth) {
		for(int i = controls.size()-1; i > -1; i--) {
			if(controls[i]->getClassName() == "PHDPROJECTOR") controls[i]->draw();
		}
	}
}

void phdControlManager::mouseMoved(ofMouseEventArgs& args) {

	if(getPopupMenu() != NULL && getPopupMenu()->getVisible() && getPopupMenu()->updateHitTestInfo(args.x,args.y)) {
		getPopupMenu()->onMouseMoved(args);
		return;
	}

	if(selectedControl != NULL && selectedControl->updateHitTestInfo(args.x, args.y)) {
		selectedControl->onMouseMoved(args);
		return;
	}

	phdControlBase * _control = NULL;

	if(drawMode == cmdmControls || drawMode == cmdmBoth) {
		for(int i = 0; i < controls.size(); i++) {
			if(controls[i] != NULL && controls[i]->getClassName() != "PHDPROJECTOR" &&
				controls[i]->updateHitTestInfo(args.x, args.y)) {
				_control = controls[i];
				break;
			}
		}
	}

	if(drawMode == cmdmWarpers || drawMode == cmdmBoth) {
		if(_control == NULL) {
			for(int i = 0; i < controls.size(); i++) {
				if(controls[i] != NULL && controls[i]->getClassName() == "PHDPROJECTOR" &&
					controls[i]->updateHitTestInfo(args.x, args.y)) {
					_control = controls[i];
					break;
				}
			}
		}
	}

	setFocusedControl(_control);

	if(focusedControl != NULL) focusedControl->onMouseMoved(args);
}

void phdControlManager::mousePressed(ofMouseEventArgs& args) {
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMousePressed(args);
		return;
	}

	if(selectedControl != NULL && selectedControl->updateHitTestInfo(args.x, args.y)) {
		selectedControl->onMousePressed(args);
	} else {
		setSelectedControl(focusedControl);
		if(selectedControl != NULL) selectedControl->onMousePressed(args);
	}
}

void phdControlManager::mouseDragged(ofMouseEventArgs& args) {
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible() && getPopupMenu()->updateHitTestInfo(args.x,args.y)) {
		getPopupMenu()->onMouseDragged(args);
		return;
	}

	if(selectedControl != NULL) selectedControl->onMouseDragged(args);
}

void phdControlManager::mouseReleased(ofMouseEventArgs& args) {
	// if popup has focus
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMouseReleased(args);
		ofRemoveListener(getPopupMenu()->popEvent, this, &phdControlManager::onPopupClick);
		getPopupMenu()->setVisible(false);
		return;
	}

	if(selectedControl == NULL) {
		if(args.button == 2 && getPopupMenu() != NULL) { // right click
			getPopupMenu()->setOwner(this);
			getPopupMenu()->popup(args.x, args.y, (void*)this, "MANAGER");
			ofAddListener(getPopupMenu()->popEvent, this, &phdControlManager::onPopupClick);
			return;
		}
	}

	if(selectedControl != NULL) selectedControl->onMouseReleased(args);
}

void phdControlManager::enableEvents() {
	if(!usingEvents) {
		ofAddListener(ofEvents().mouseMoved, this, &phdControlManager::mouseMoved);
		ofAddListener(ofEvents().mousePressed, this, &phdControlManager::mousePressed);
		ofAddListener(ofEvents().mouseDragged, this, &phdControlManager::mouseDragged);
		ofAddListener(ofEvents().mouseReleased, this, &phdControlManager::mouseReleased);
		usingEvents = true;
	}
}

void phdControlManager::disableEvents(){
	if(usingEvents) {
		ofRemoveListener(ofEvents().mouseMoved, this, &phdControlManager::mouseMoved);
		ofRemoveListener(ofEvents().mousePressed, this, &phdControlManager::mousePressed);
		ofRemoveListener(ofEvents().mouseDragged, this, &phdControlManager::mouseDragged);
		ofRemoveListener(ofEvents().mouseReleased, this, &phdControlManager::mouseReleased);
		usingEvents = false;
	}
}

void phdControlManager::onPopupClick(phdPopupMenuEventData &_eventData) {
	if(_eventData.menu != NULL) {
		ofRemoveListener(_eventData.menu->popEvent, this, &phdControlManager::onPopupClick);
		_eventData.menu->setVisible(false);
	}
}

