#include "phdShapeGroup.h"

//--------------------------------------------------------------------------------------------------------------
phdShapeGroup::phdShapeGroup() {
	visible = true;
	usingEvents = false;
	selected = NULL;
	focused = NULL;
	//enableEvents();
}

phdShapeGroup::~phdShapeGroup() {
//	disableEvents();
	freeShapes();
}

phdShapeBase * phdShapeGroup::operator[](unsigned i) { return shapes[i]; }

int phdShapeGroup::shapesCount() { return shapes.size(); }
	
void phdShapeGroup::freeShapes() {
	setFocused(NULL);
	setSelected(NULL);
	for (vector<phdShapeBase*>::iterator i = shapes.begin(); i != shapes.end(); i++) {
		delete(*i); (*i) = NULL;
	}
	shapes.clear();
}

void phdShapeGroup::setVisible(bool _value) {
	if(visible != _value) {
		visible = _value;
		if(visible) {
			//enableEvents();
		} else {
			//disableEvents();
		}
	}
}

int phdShapeGroup::indexOf(phdShapeBase * _shape) {
	for(int i = 0; i < shapes.size(); i++) {
		if(shapes[i] == _shape) return i;
	}
	return -1;
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

void phdShapeGroup::draw(ofMatrix4x4 & _mat) {
	if(visible)  {
		for(int i = 0; i < shapes.size(); i++) shapes[i]->draw(_mat);
	}
}

void phdShapeGroup::setSelected(phdShapeBase * _shape, bool _force) {
	if(selected != NULL) selected->setSelected(false);
	if(_shape != NULL || _force) selected = _shape;
	if(selected != NULL) selected->setSelected(true);
}

void phdShapeGroup::setFocused(phdShapeBase * _shape) {
	if(focused != NULL) focused->setFocused(false);
	focused = _shape;
	if(focused != NULL) focused->setFocused(true);
}
/*
void phdShapeGroup::mouseMoved(ofMouseEventArgs& args) {

	if(focused != NULL && focused->updateHitTestInfo(args.x, args.y)) return;

	setFocused(NULL);
	for(int i = 0; i < shapes.size(); i++) {
		if(shapes[i]->updateHitTestInfo(args.x, args.y)) {
			setFocused(shapes[i]);
		}
	}
	if(focused != NULL) focused->shapeMouseMoved(args);
}

void phdShapeGroup::mousePressed(ofMouseEventArgs& args) {
	if(args.button == 0 && selected != focused) setSelected(focused);
	if(selected != NULL) selected->shapeMousePressed(args);
}

void phdShapeGroup::mouseDragged(ofMouseEventArgs& args) {
	if(selected != NULL) {
		selected->shapeMouseDragged(args);
		return;
	}
}

void phdShapeGroup::mouseReleased(ofMouseEventArgs& args) {
	if(selected != NULL) selected->shapeMouseReleased(args);
}

void phdShapeGroup::enableEvents() {
	if(!usingEvents) {
		ofAddListener(ofEvents.mouseMoved, this, &phdShapeGroup::mouseMoved);
		ofAddListener(ofEvents.mousePressed, this, &phdShapeGroup::mousePressed);
		ofAddListener(ofEvents.mouseDragged, this, &phdShapeGroup::mouseDragged);
		ofAddListener(ofEvents.mouseReleased, this, &phdShapeGroup::mouseReleased);
		usingEvents = true;
	}
}

void phdShapeGroup::disableEvents(){
	if(usingEvents) {
		ofRemoveListener(ofEvents.mouseMoved, this, &phdShapeGroup::mouseMoved);
		ofRemoveListener(ofEvents.mousePressed, this, &phdShapeGroup::mousePressed);
		ofRemoveListener(ofEvents.mouseDragged, this, &phdShapeGroup::mouseDragged);
		ofRemoveListener(ofEvents.mouseReleased, this, &phdShapeGroup::mouseReleased);
		usingEvents = false;
	}
}
*/

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
