#include "phdShapeQuadWarp.h"

//-----------------------------------------------------------------------------------------------------------------
phdShapeQuadWarp::phdShapeQuadWarp() {
	phdShapeBase::phdShapeBase();
	closed = true;
	maxVertices = 4;
	canMoveVertex = true;
	canOpenClose = false;
	faceType = wftQuadDLD;
}

void phdShapeQuadWarp::setVertexPos(int _index, double _x, double _y) {
	phdShapeBase::setVertexPos(_index, _x, _y);
}

void phdShapeQuadWarp::update() {

}

void phdShapeQuadWarp::drawEdgeInfo(ofPolyline & _coords) {

	if(_coords.size() < 4) {
		phdShapeBase::drawEdgeInfo(_coords);
		return;
	}

	if(selected) {
		ofSetColor(selectedColor.border);
	} else {
		ofSetColor(normalColor.border);
	}

	float _x0, _y0, _x1, _y1, _x2, _y2, _x3, _y3;

	_x0 = _coords[0].x; _y0 = _coords[0].y;
	_x1 = _coords[1].x; _y1 = _coords[1].y;
	_x2 = _coords[2].x; _y2 = _coords[2].y;
	_x3 = _coords[3].x; _y3 = _coords[3].y;

	float _d01 = ofDist(_x0,_y0,_x1,_y1);
	float _d12 = ofDist(_x1,_y1,_x2,_y2);
	float _d23 = ofDist(_x2,_y2,_x3,_y3);
	float _d30 = ofDist(_x3,_y3,_x0,_y0);
	float _d02 = ofDist(_x0,_y0,_x2,_y2);
	float _d13 = ofDist(_x1,_y1,_x3,_y3);

	/*if(_d02 > _d13) {
		_d01 /= _d02; _d12 /= _d02; _d23 /= _d01; _d30 /= _d01; _d13 /= _d02; _d02 = 1.0;
	} else {
		_d01 /= _d13; _d12 /= _d13; _d23 /= _d13; _d30 /= _d13; _d02 /= _d13; _d13 = 1.0;
	}*/

	if(faceType == wftTriTR) {
		drawEdgeWithSizeLabel(_x0, _y0, _x2, _y2, ofToString(_d02, 2), true);
		drawEdgeWithSizeLabel(_x2, _y2, _x1, _y1, ofToString(_d12, 2), false);
		drawEdgeWithSizeLabel(_x1, _y1, _x0, _y0, ofToString(_d01, 2), false);
	} else if(faceType == wftTriBL) {
		drawEdgeWithSizeLabel(_x2, _y2, _x0, _y0, ofToString(_d02, 2), true);
		drawEdgeWithSizeLabel(_x0, _y0, _x3, _y3, ofToString(_d30, 2), false);
		drawEdgeWithSizeLabel(_x3, _y3, _x2, _y2, ofToString(_d23, 2), false);
	} else if(faceType == wftTriTL) {
		drawEdgeWithSizeLabel(_x3, _y3, _x1, _y1, ofToString(_d13, 2), true);
		drawEdgeWithSizeLabel(_x1, _y1, _x0, _y0, ofToString(_d01, 2), false);
		drawEdgeWithSizeLabel(_x0, _y0, _x3, _y3, ofToString(_d30, 2), false);
	} else if(faceType == wftTriBR) {
		drawEdgeWithSizeLabel(_x1, _y1, _x3, _y3, ofToString(_d13, 2), true);
		drawEdgeWithSizeLabel(_x3, _y3, _x2, _y2, ofToString(_d23, 2), false);
		drawEdgeWithSizeLabel(_x2, _y2, _x1, _y1, ofToString(_d12, 2), false);
	} else if(faceType == wftQuadDLU || faceType == wftQuadDLD) {
		drawEdgeWithSizeLabel(_x0, _y0, _x3, _y3, ofToString(_d30, 2), false);
		drawEdgeWithSizeLabel(_x3, _y3, _x2, _y2, ofToString(_d23, 2), false);
		drawEdgeWithSizeLabel(_x2, _y2, _x1, _y1, ofToString(_d12, 2), false);
		drawEdgeWithSizeLabel(_x1, _y1, _x0, _y0, ofToString(_d01, 2), false);
		if(faceType == wftQuadDLU) {
			drawEdgeWithSizeLabel(_x3, _y3, _x1, _y1, ofToString(_d13, 2), true);
		} else if(faceType == wftQuadDLD) {
			drawEdgeWithSizeLabel(_x0, _y0, _x2, _y2, ofToString(_d02, 2), true);
		}
	}
}

void phdShapeQuadWarp::draw(ofMatrix4x4 & _mat) {

	ofPolyline _poly; vertices.getTransformed(_poly, gimbal.getOTSMatrix() * _mat);

	if(_poly.size() == 4) {
		double _x, _y;

		ofFloatColor _c;
		if(selected) {
			_c = selectedColor.fill;
		} else {
			_c = normalColor.fill;
		}
		_c.a = 0.35;

		double _x0, _y0, _x1, _y1, _x2, _y2, _x3, _y3;
		_x0 = _poly[0].x; _y0 = _poly[0].y;
		_x1 = _poly[1].x; _y1 = _poly[1].y;
		_x2 = _poly[2].x; _y2 = _poly[2].y;
		_x3 = _poly[3].x; _y3 = _poly[3].y;

		// draw source face
		glBegin(GL_TRIANGLES);
			if(faceType == wftQuadDLD || faceType == wftTriTR) {
				glColor4f(_c.r, _c.g, _c.b, _c.a); glVertex3f(_x0, _y0, 0);
				glColor4f(_c.r, _c.g, _c.b, _c.a); glVertex3f(_x2, _y2, 0);
				glColor4f(_c.r, _c.g, _c.b, _c.a); glVertex3f(_x1, _y1, 0);
			}
			if(faceType == wftQuadDLD || faceType == wftTriBL) {
				glColor4f(_c.r, _c.g, _c.b, _c.a); glVertex3f(_x0, _y0, 0);
				glColor4f(_c.r, _c.g, _c.b, _c.a); glVertex3f(_x3, _y3, 0);
				glColor4f(_c.r, _c.g, _c.b, _c.a); glVertex3f(_x2, _y2, 0);
			}
			if(faceType == wftQuadDLU || faceType == wftTriTL) {
				glColor4f(_c.r, _c.g, _c.b, _c.a); glVertex3f(_x0, _y0, 0);
				glColor4f(_c.r, _c.g, _c.b, _c.a); glVertex3f(_x3, _y3, 0);
				glColor4f(_c.r, _c.g, _c.b, _c.a); glVertex3f(_x1, _y1, 0);
			}
			if(faceType == wftQuadDLU || faceType == wftTriBR) {
				glColor4f(_c.r, _c.g, _c.b, _c.a); glVertex3f(_x3, _y3, 0);
				glColor4f(_c.r, _c.g, _c.b, _c.a); glVertex3f(_x2, _y2, 0);
				glColor4f(_c.r, _c.g, _c.b, _c.a); glVertex3f(_x1, _y1, 0);
			}
		glEnd();
	}

	if(selected) {
		drawBorder(_poly);
		drawHitTestInfo(_poly, _mat, true, false, true);
		drawEdgeInfo(_poly);
	} else if(focused) {
		drawBorder(_poly);
		drawHitTestInfo(_poly, _mat, true, false, false);
	} else {
		drawBorder(_poly);
		ofVec2f _center; _center.set(0,0);
		for(int i = 0; i < _poly.size(); i++) { _center.x += _poly[i].x; _center.y += _poly[i].y; }
		_center /= _poly.size();
		ofDrawBitmapString(name, _center.x, _center.y);
	}
}
