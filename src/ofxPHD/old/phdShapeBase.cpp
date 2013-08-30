#include "phdShapeBase.h"

//--------------------------------------------------------------------------------------------------------------
phdShapeBase::phdShapeBase() {
	updated = true;
	closed = true;
	selected = false;
	focused = false;
	canMoveVertex = true;
	canOpenClose = true;
	maxVertices = -1; // any size
	normalColor.set(ofColor::gray, ofColor::red, ofColor::magenta, ofColor::magenta);
	selectedColor.set(ofColor::white, ofColor::cyan, ofColor::magenta, ofColor::cyan);
}

void phdShapeBase::setSelected(bool _value) {
	selected = _value;
}

void phdShapeBase::setFocused(bool _value) {
	focused = _value;
}

void phdShapeBase::setVertices(ofPolyline & _points) {
	gimbal.updateByRectangle(_points.getBoundingBox());
	vertices.setTransformed(_points, gimbal.getSTOMatrix());
	updated = true;
}

void phdShapeBase::getScreenVertices(ofPolyline & _points) {
	_points.clear();
	vertices.getTransformed(_points, gimbal.getOTSMatrix());
}

void phdShapeBase::setRotationPoint(double _x, double _y) {
	gimbal.cx = _x;
	gimbal.cy = _y;
	updated = true;
}

void phdShapeBase::setAngle(double _ax, double _ay) {
	gimbal.angle = ofRadToDeg(atan2(_ay-gimbal.cy, _ax-gimbal.cx));
	updated = true;
}

void phdShapeBase::setWidthScale(double _x, double _y) {
	gimbal.sx = ofDist(_x,_y,gimbal.cx,gimbal.cy);
	updated = true;
}

void phdShapeBase::setHeightScale(double _x, double _y) {
	gimbal.sy = ofDist(_x,_y,gimbal.cx,gimbal.cy);
	updated = true;
}

void phdShapeBase::setVertex(int _index, double _x, double _y) {
	if(canMoveVertex && _index > -1 && _index < vertices.size()) {
		ofPoint _s = ofPoint(_x, _y);
		_s = _s * gimbal.getSTOMatrix();
		vertices.setVertex(_index, _s.x, _s.y);
		updated = true;
	}
}

void phdShapeBase::translate(double _dx, double _dy) {
	gimbal.translate(_dx, _dy);
	updated = true;
}

void phdShapeBase::rotate(double _da) {
	gimbal.rotate(_da);
	updated = true;
}

void phdShapeBase::resize(double _sx, double _sy) {
	gimbal.resize(_sx, _sy);
	updated = true;
}

void phdShapeBase::addVertex(double _x, double _y) {
	if(maxVertices == -1 || vertices.size() < maxVertices) {
		//ofVec3f _p = ofVec3f(_x,_y,0.0); _p = _p * gimbal.getSTOMatrix();
		//vertices.addVertex(_p.x, _p.y);
		vertices.addVertex(_x,_y); // object coordinate system
		updated = true;
	}
}

void phdShapeBase::insertVertex(int _index, double _x, double _y) {
	if(maxVertices == -1 || vertices.size() < maxVertices) {
		ofVec3f _p = ofVec3f(_x,_y,0.0);
		vertices.insertVertex(_index, _p * gimbal.getSTOMatrix());
		updated = true;
	}
}

bool phdShapeBase::updateHitTestInfo(double _x, double _y) {

	hitTestInfo.set(htiNone, -1, goNone);

	ofPolyline _scrKeys;
	gimbal.getKeyPoints(_scrKeys); 

	// try gimbal keypoints
	hitTestInfo.index = phdPointOverVertex(_scrKeys, _x, _y);
	if(hitTestInfo.index != -1) {
		hitTestInfo.info = htiGimbal;
		if(hitTestInfo.index == 0) {		hitTestInfo.operation = goWidth;
		} else if(hitTestInfo.index == 1) {	hitTestInfo.operation = goRotate;
		} else if(hitTestInfo.index == 2) {	hitTestInfo.operation = goCenter;
		} else if(hitTestInfo.index == 3) {	hitTestInfo.operation = goHeight;
		}
		return true;
	}

	ofPolyline _scrPoints; vertices.getTransformed(_scrPoints, gimbal.getOTSMatrix());

	// try vertex
	hitTestInfo.index = phdPointOverVertex(_scrPoints, _x, _y);
	if(hitTestInfo.index != -1) {
		hitTestInfo.info = htiVertex;
		hitTestInfo.operation = goVertex;
		return true;
	}

	// try edges
	hitTestInfo.index = phdPointNearEdge(_scrPoints, _x, _y, 2.0, closed);
	if(hitTestInfo.index != -1) {
		hitTestInfo.info = htiEdge;
		hitTestInfo.operation = goEdge;
		return true;
	}

	// try inside
	if(closed && phdPointInsidePolygon(_scrPoints, _x, _y)) {
		hitTestInfo.info = htiInside;
		hitTestInfo.operation = goInside;
		hitTestInfo.index = 0;
		return true;
	}
	
	return false;
}

void phdShapeBase::shapeMouseMoved(ofMouseEventArgs& args) {

}

void phdShapeBase::shapeMousePressed(ofMouseEventArgs& args) {
	
	clickPos.set(args.x, args.y, 0.0);
	dragPos.set(args.x, args.y, 0.0);
	
	updateHitTestInfo(args.x, args.y);
}

void phdShapeBase::shapeMouseDragged(ofMouseEventArgs& args) {

	if(args.button == 0) { // left button
		if(hitTestInfo.operation == goWidth) {
			setWidthScale(args.x, args.y);
		} else if(hitTestInfo.operation == goRotate) {
			setAngle(args.x, args.y);
		} else if(hitTestInfo.operation == goCenter) {
			setRotationPoint(args.x, args.y);
		} else if(hitTestInfo.operation == goHeight) {
			setHeightScale(args.x, args.y);
		} else if(canMoveVertex && hitTestInfo.operation == goVertex) {
			setVertex(hitTestInfo.index, args.x, args.y);
		} else if(hitTestInfo.operation == goEdge) {
			if(!closed) {
				gimbal.cx += args.x - dragPos.x;
				gimbal.cy += args.y - dragPos.y;
				updated = true;
			}
		} else if(hitTestInfo.operation == goInside) {
			gimbal.cx += args.x - dragPos.x;
			gimbal.cy += args.y - dragPos.y;
			updated = true;
		}
	}

	dragPos.set(args.x, args.y, 0.0);
}

void phdShapeBase::shapeMouseReleased(ofMouseEventArgs& args) {
	if(hitTestInfo.operation == goWidth) {

	} else if(hitTestInfo.operation == goRotate) {

	} else if(hitTestInfo.operation == goCenter) {
		if(canOpenClose && args.button == 2) { // right button
			closed = !closed;
			updated = true;
		}
	} else if(hitTestInfo.operation == goHeight) {

	} else if(hitTestInfo.operation == goVertex) {

	} else if(hitTestInfo.operation == goEdge) {
		if(args.button == 2 && (maxVertices == -1 || vertices.size() < maxVertices)) { // right button
			insertVertex(hitTestInfo.index + 1, args.x, args.y);
			updated = true;
		}
	} else if(hitTestInfo.operation == goInside) {

	}
}

void phdShapeBase::update() {

}

void phdShapeBase::drawFace(ofPolyline & _coords, ofPolyline _texels) {

	if(!closed) return;

	if(selected) {
		ofSetColor(selectedColor.fill);
	} else {
		ofSetColor(normalColor.fill);
	}

	ofFill();

	glBegin(GL_POLYGON);
	for(int i = 0; i < _coords.size(); i++) {
		glVertex2f(_coords[i].x, _coords[i].y);
	}
	glEnd();
}

void phdShapeBase::drawBorder(ofPolyline & _coords) {
	if(selected) {
		ofSetColor(selectedColor.border);
	} else {
		ofSetColor(normalColor.border);
	}
	for(int i = 0; i < _coords.size(); i++) {
		if(hitTestInfo.info == htiEdge && hitTestInfo.index == i) {
			ofSetColor(selectedColor.border);
		} else {
			ofSetColor(normalColor.border);
		}
		if(i == _coords.size()-1) {
			if(closed) ofLine(_coords[_coords.size()-1].x, _coords[_coords.size()-1].y, _coords[0].x, _coords[0].y);
		} else {
			ofLine(_coords[i].x, _coords[i].y, _coords[i+1].x, _coords[i+1].y);
		}
	}
}

void phdShapeBase::drawEdgeInfo(ofPolyline & _coords) {
	if(selected) {
		ofSetColor(selectedColor.border);
	} else {
		ofSetColor(normalColor.border);
	}
	for(int i = 0; i < _coords.size(); i++) {
		if(hitTestInfo.info == htiEdge && hitTestInfo.index == i) {
			ofSetColor(selectedColor.border);
		} else {
			ofSetColor(normalColor.border);
		}
		if(i == _coords.size()-1) {
			if(closed) drawEdgeWithSizeLabel(_coords[_coords.size()-1].x, _coords[_coords.size()-1].y, _coords[0].x, _coords[0].y, ofToString(ofDist(_coords[_coords.size()-1].x, _coords[_coords.size()-1].y, _coords[0].x, _coords[0].y),1), false);
		} else {
			drawEdgeWithSizeLabel(_coords[i].x, _coords[i].y, _coords[i+1].x, _coords[i+1].y, ofToString(ofDist(_coords[i].x, _coords[i].y, _coords[i+1].x, _coords[i+1].y),1), false);
		}
	}
}

void phdShapeBase::drawHitTestInfo(ofPolyline & _coords, ofMatrix4x4 & _mat, bool _gimbal, bool _edge, bool _vertex) {
	if(_edge && (maxVertices == -1 || vertices.size() < maxVertices)) {
		for(int i = 0; i < _coords.size(); i++) {
			if(hitTestInfo.info == htiEdge && hitTestInfo.index == i) {
				ofSetColor(selectedColor.border);
			} else {
				ofSetColor(normalColor.border);
			}
			if(i == _coords.size()-1) {
				if(closed) ofLine(_coords[_coords.size()-1].x, _coords[_coords.size()-1].y, _coords[0].x, _coords[0].y);
			} else {
				ofLine(_coords[i].x, _coords[i].y, _coords[i+1].x, _coords[i+1].y);
			}
		}
	}
	if(_vertex && canMoveVertex) {
		for(int i = 0; i < _coords.size(); i++) {
			if(hitTestInfo.info == htiVertex && hitTestInfo.index == i) {
				ofSetColor(selectedColor.vertex);
			} else {
				ofSetColor(normalColor.vertex);
			}
			ofCircle(_coords[i].x, _coords[i].y, 5);
		}
	}
	if(_gimbal) {
		for(int i = 0; i < _coords.size(); i++) {
			if(hitTestInfo.info == htiGimbal) {
				ofSetColor(selectedColor.gimbal);
			} else {
				ofSetColor(normalColor.gimbal);
			}
			gimbal.draw(_mat);
		}
	}
}

void phdShapeBase::draw(ofMatrix4x4 & _mat) {

	ofPolyline _poly; vertices.getTransformed(_poly, gimbal.getOTSMatrix() * _mat);

	if(selected) {
		if(closed) drawFace(_poly, _poly);
		drawBorder(_poly);
		drawHitTestInfo(_poly, _mat, true, true, true);
	} else if(focused) {
		if(closed) drawFace(_poly, _poly);
		drawBorder(_poly);
		drawHitTestInfo(_poly, _mat, true, false, false);
	} else {
		if(closed) drawFace(_poly, _poly);
		drawBorder(_poly);
	}

	if(selected) drawStatus();
}
