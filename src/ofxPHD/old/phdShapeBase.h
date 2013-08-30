#pragma once

#include "ofMain.h"

#include "phdHitTestInfo.h"
#include "phdGimbal2d.h"
#include "phdVertices.h"

//--------------------------------------------------------------------------------------------------------------
class phdShapeColorSchema {
public:
	ofColor fill;
	ofColor border;
	ofColor gimbal;
	ofColor vertex;

	void set(ofColor _fill, ofColor _border, ofColor _gimbal, ofColor _vertex) {
		fill   = _fill;
		border = _border;
		gimbal = _gimbal;
		vertex = _vertex;
	}
};

//--------------------------------------------------------------------------------------------------------------
class phdShapeBase {
public:
	string name;
	phdHitTestInfo hitTestInfo;
	phdGimbal2d gimbal;
	phdVertices vertices;
	bool closed, selected, focused, updated;
	bool canMoveVertex, canOpenClose;
	int maxVertices;
	ofPoint clickPos;
	ofPoint dragPos;

	phdShapeColorSchema normalColor;
	phdShapeColorSchema selectedColor;

	phdShapeBase();
	void setSelected(bool _value);
	void setFocused(bool _value);
	void setVertices(ofPolyline & _points);
	void getScreenVertices(ofPolyline & _points);
	void setRotationPoint(double _x, double _y);
	void setAngle(double _ax, double _ay);
	void setWidthScale(double _x, double _y);
	void setHeightScale(double _x, double _y);
	virtual void setVertex(int _index, double _x, double _y);
	void translate(double _dx, double _dy);
	void rotate(double _da);
	void resize(double _sx, double _sy);
	void addVertex(double _x, double _y);
	void insertVertex(int _index, double _x, double _y);

	bool updateHitTestInfo(double _x, double _y);
	
	virtual void shapeMouseMoved(ofMouseEventArgs& args);
	virtual void shapeMousePressed(ofMouseEventArgs& args);
	virtual void shapeMouseDragged(ofMouseEventArgs& args);
	virtual void shapeMouseReleased(ofMouseEventArgs& args);

	virtual void drawFace(ofPolyline & _coords, ofPolyline _texels);
	virtual void drawBorder(ofPolyline & _coords);
	void drawEdgeInfo(ofPolyline & _coords);
	virtual void drawHitTestInfo(ofPolyline & _coords, ofMatrix4x4 & _mat, bool _gimbal, bool _edge, bool _vertex);

	virtual void update();
	virtual void draw(ofMatrix4x4 & _mat);

	void drawStatus() {

		string _status = "";
		_status += canMoveVertex ? "moveVertex" : "";
		_status += (maxVertices == -1 || vertices.size() < maxVertices) ? " addVertex" : "";
		_status += " maxVertices:" + ofToString(maxVertices);
		_status += " vertices.size():" + ofToString(vertices.size());

		ofDrawBitmapString(_status, 20, ofGetHeight()-30);

		ofPolyline _coords; vertices.getTransformed(_coords, gimbal.getOTSMatrix());

		for(int i = 0; i < _coords.size(); i++) {
			double _edgeSize = 0.0;
			ofPoint _labelPos; _labelPos.set(0.0);
			if(i == _coords.size()-1) {
				if(closed) {
					double _x1 = _coords[_coords.size()-1].x;
					double _y1 = _coords[_coords.size()-1].y;
					double _x2 = _coords[0].x;
					double _y2 = _coords[0].y;
					drawEdgeWithSizeLabel(_x1,_y1,_x2,_y2,"L:"+ofToString(i), true);
				}
			} else {
				double _x1 = _coords[i].x;
				double _y1 = _coords[i].y;
				double _x2 = _coords[i+1].x;
				double _y2 = _coords[i+1].y;
				drawEdgeWithSizeLabel(_x1,_y1,_x2,_y2,"L:"+ofToString(i), true);
			}
		}
	}
};
