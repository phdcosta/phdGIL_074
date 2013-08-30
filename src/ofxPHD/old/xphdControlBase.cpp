#include "phdControlBase.h"

//----------------------------------------------------------------------------------------------------------------------

phdControlBase::phdControlBase() {
	selected = focused = dragging = visible = false;
	x = y = width = height = 10.0;
	setPopupMenu(NULL);
	inputLinks.setDrawMode(ldmHorRight);
	outputLinks.setDrawMode(ldmHorRight);
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
