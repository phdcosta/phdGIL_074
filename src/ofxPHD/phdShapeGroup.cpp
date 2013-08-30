#include "phdShapeGroup.h"

//--------------------------------------------------------------------------------------------------------------
phdShapeGroup::phdShapeGroup() {
	visible = true;
	focused = NULL;
	selected = NULL;
	setPopupMenu(NULL);
}

phdShapeGroup::~phdShapeGroup() {
	freeShapes();
}

void phdShapeGroup::freeShapes() {
	setFocused(NULL);
	setSelected(NULL);
	for (vector<phdShapeBase*>::iterator i = shapes.begin(); i != shapes.end(); i++) {
		delete(*i); (*i) = NULL;
	}
	shapes.clear();
}

int phdShapeGroup::indexOf(phdShapeBase * _shape) {
	for(int i = 0; i < shapes.size(); i++) {
		if(shapes[i] == _shape) return i;
	}
	return -1;
}

void phdShapeGroup::setVisible(bool _value) {
	if(visible != _value) {
		visible = _value;
	}
}

void phdShapeGroup::setSelected(phdShapeBase * _shape) {
	if(selected != NULL) selected->setSelected(false);
	selected = _shape;
	if(selected != NULL) selected->setSelected(true);
}

void phdShapeGroup::setFocused(phdShapeBase * _shape) {
	if(focused != NULL) focused->setFocused(false);
	focused = _shape;
	if(focused != NULL) focused->setFocused(true);
}

int phdShapeGroup::addShape(phdShapeBase * _shape) {
	shapes.push_back(_shape);
	return shapes.size()-1;
}

void phdShapeGroup::delShape(int _index) {
	if(_index > -1 && _index < shapes.size()) { 
		if(shapes[_index] == focused) setFocused(NULL);
		if(shapes[_index] == selected) setSelected(NULL);
		delete shapes[_index];
		shapes[_index] = NULL;
		shapes.erase(shapes.begin() + _index);
	}
}

void phdShapeGroup::delShape(phdShapeBase * _shape) {
	int _index = indexOf(_shape);
	if(_index != -1) delShape(_index);
}

void phdShapeGroup::adjustNearVertices(double _tolerance) {
	for(int a = 0; a < shapes.size(); a++) {
		phdShapeBase * shapeA = shapes[a];
		if(shapeA != NULL) {
			ofMatrix4x4 _aSTO = shapeA->gimbal.getSTOMatrix();
			ofMatrix4x4 _aOTS = shapeA->gimbal.getOTSMatrix();
			for(int b = 0; b < shapes.size(); b++) {
				phdShapeBase * shapeB = shapes[b];
				if(shapeB != NULL) {
					ofMatrix4x4 _bOTS = shapeB->gimbal.getOTSMatrix();
					ofMatrix4x4 _bSTO = shapeB->gimbal.getSTOMatrix();
					if(shapeA != shapeB) {
						for(int i = 0; i < shapeA->vertices.size(); i++) {
							ofPoint _p1 = shapeA->vertices[i] * _aOTS;
							for(int j = 0; j < shapeB->vertices.size(); j++) {
								ofPoint _p2 = shapeB->vertices[j] * _bOTS;
								double _distance = ofDist(_p1.x,_p1.y,_p2.x,_p2.y);
								if(_tolerance > _distance && _distance > 0.0) {

									ofPoint _pos = (_p1+_p2) / 2.0;

									_p1 = _p2 = _pos;

									// convert pos to object coords
									ofPoint _posA = _pos * _aSTO;
									ofPoint _posB = _pos * _bSTO;

									shapeA->vertices[i].x = _posA.x;
									shapeA->vertices[i].y = _posA.y;
									shapeB->vertices[j].x = _posB.x;
									shapeB->vertices[j].y = _posB.y;
								}
							}
						}
					}
				}
			}
		}
	}
}

void phdShapeGroup::update() {
	if(!visible)  return;
	for(int i = 0; i < shapes.size(); i++) shapes[i]->update();
}

void phdShapeGroup::draw(ofMatrix4x4 & _mat) {
	if(!visible)  return;

	ofPushStyle();

	ofSetColor(255,255,255,255);
	ofNoFill();
	ofEnableAlphaBlending();

	for(int i = shapes.size()-1; i > -1; i--) {
		if(shapes[i] != selected) shapes[i]->draw(_mat);
	}
	if(selected != NULL) selected->draw(_mat);

	ofPopStyle();
}

void phdShapeGroup::onMouseMoved(ofMouseEventArgs& args) {

	if(!visible)  return;

	// if popup has focus
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		ofMouseEventArgs _args = args; _args.x = ofGetMouseX(); _args.y = ofGetMouseY();
		getPopupMenu()->onMouseMoved(_args);
		return;
	}

	if(focused != NULL && focused->updateHitTestInfo(args.x, args.y)) return;

	phdShapeBase * _shape = NULL;
	for(int i = 0; i < shapes.size(); i++) {
		if(shapes[i] != NULL && shapes[i]->updateHitTestInfo(args.x, args.y)) {
			_shape = shapes[i];
			break;
		}
	}

	setFocused(_shape);

	if(focused != NULL) focused->onMouseMoved(args);
}

void phdShapeGroup::onMousePressed(ofMouseEventArgs& args) {

	if(!visible)  return;

	// if popup has focus
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		ofMouseEventArgs _args = args; _args.x = ofGetMouseX(); _args.y = ofGetMouseY();
		getPopupMenu()->onMousePressed(_args);
		return;
	}

	if(selected != focused) setSelected(focused);

	if(selected != NULL) selected->onMousePressed(args);
}

void phdShapeGroup::onMouseDragged(ofMouseEventArgs& args) {

	if(!visible)  return;

	// if popup has focus
	ofMouseEventArgs _args = args; _args.x = ofGetMouseX(); _args.y = ofGetMouseY();
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMouseDragged(_args);
		return;
	}

	if(selected != NULL) { selected->onMouseDragged(args); }
}

void phdShapeGroup::onMouseReleased(ofMouseEventArgs& args) {

	if(!visible)  return;

	// if popup has focus
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		ofMouseEventArgs _args = args; _args.x = ofGetMouseX(); _args.y = ofGetMouseY();
		getPopupMenu()->onMouseReleased(_args);
		ofRemoveListener(getPopupMenu()->popEvent, this, &phdShapeGroup::onPopupClick);
		return;
	}

	if(selected != NULL && selected == focused) {
		if(args.button == 2 && getPopupMenu() != NULL) { // right click
			getPopupMenu()->setOwner(selected);
			getPopupMenu()->popup(ofGetMouseX(), ofGetMouseY(), (void*)selected, "SHAPE");
			ofAddListener(getPopupMenu()->popEvent, this, &phdShapeGroup::onPopupClick);
			return;
		}
	}

	if(selected != NULL) selected->onMouseReleased(args);
}

void phdShapeGroup::onPopupClick(phdPopupMenuEventData &_eventData) {
	if(_eventData.menu != NULL) {
		ofRemoveListener(_eventData.menu->popEvent, this, &phdShapeGroup::onPopupClick);
	}
}
