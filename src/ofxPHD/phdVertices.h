#pragma once

#include "ofMain.h"

//--------------------------------------------------------------------------------------------------------------
class phdVertices {
public:
	vector<ofPoint> vertices;

	phdVertices();
	ofPoint & phdVertices::operator[] (int _index);
	int size();
	void addVertex(double _x, double _y);
	void addVertex(ofPoint _point);
	void setVertex(int _index, double _x, double _y);
	void insertVertex(int _index, ofPoint _point);
	void insertVertex(int _index, double _x, double _y);
	void transform(ofMatrix4x4 _mat);
	void setTransformed(ofPolyline _src, ofMatrix4x4 _mat);
	void getTransformed(ofPolyline & _dst, ofMatrix4x4 _mat);
	ofRectangle getBoundingBox();
};

