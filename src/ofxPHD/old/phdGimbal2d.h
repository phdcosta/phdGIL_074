#pragma once

#include "ofMain.h"

#include "phdGeom.h"

//--------------------------------------------------------------------------------------------------------------
class phdGimbal2d {
public:
	double w, h;
	double cx, cy;
	double angle;
	double sx, sy;
	
	phdGimbal2d();
	void translate(double _dx, double _dy);
	void rotate(double _da);
	void resize(double _sx, double _sy);
	ofMatrix4x4 getOTSMatrix();
	ofMatrix4x4 getSTOMatrix();
	void updateByRectangle(ofRectangle _rect);
	void getKeyPoints(ofPolyline & _keys);
	void draw(ofMatrix4x4 & _mat);
};
