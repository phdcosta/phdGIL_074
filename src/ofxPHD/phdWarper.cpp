#pragma once

#include "phdWarper.h"

//
// 3x3 matrix manipulation routines for
// projection warping from a modified Theo example on the OF forums:
// http://threeblindmiceandamonkey.com/?p=31
//

// multiply matrix: c = a * b
void multiplyMatrix(double a[3][3], double b[3][3], double c[3][3])
{
    c[0][0] = a[0][0]*b[0][0] + a[0][1]*b[1][0] + a[0][2]*b[2][0];
    c[0][1] = a[0][0]*b[0][1] + a[0][1]*b[1][1] + a[0][2]*b[2][1];
    c[0][2] = a[0][0]*b[0][2] + a[0][1]*b[1][2] + a[0][2]*b[2][2];
    c[1][0] = a[1][0]*b[0][0] + a[1][1]*b[1][0] + a[1][2]*b[2][0];
    c[1][1] = a[1][0]*b[0][1] + a[1][1]*b[1][1] + a[1][2]*b[2][1];
    c[1][2] = a[1][0]*b[0][2] + a[1][1]*b[1][2] + a[1][2]*b[2][2];
    c[2][0] = a[2][0]*b[0][0] + a[2][1]*b[1][0] + a[2][2]*b[2][0];
    c[2][1] = a[2][0]*b[0][1] + a[2][1]*b[1][1] + a[2][2]*b[2][1];
    c[2][2] = a[2][0]*b[0][2] + a[2][1]*b[1][2] + a[2][2]*b[2][2];
}

// determinant of a 2x2 matrix
double det2(double a, double b, double c, double d)
{
    return (a*d - b*c);
}

// adjoint matrix: b = adjoint(a); returns determinant(a)
double adjointMatrix(double a[3][3], double b[3][3])
{
    b[0][0] = det2(a[1][1], a[1][2], a[2][1], a[2][2]);
    b[1][0] = det2(a[1][2], a[1][0], a[2][2], a[2][0]);
    b[2][0] = det2(a[1][0], a[1][1], a[2][0], a[2][1]);
    b[0][1] = det2(a[2][1], a[2][2], a[0][1], a[0][2]);
    b[1][1] = det2(a[2][2], a[2][0], a[0][2], a[0][0]);
    b[2][1] = det2(a[2][0], a[2][1], a[0][0], a[0][1]);
    b[0][2] = det2(a[0][1], a[0][2], a[1][1], a[1][2]);
    b[1][2] = det2(a[0][2], a[0][0], a[1][2], a[1][0]);
    b[2][2] = det2(a[0][0], a[0][1], a[1][0], a[1][1]);
    return a[0][0]*b[0][0] + a[0][1]*b[0][1] + a[0][2]*b[0][2];
}

#define MATRIX_TOLERANCE 1e-13
#define MATRIX_ZERO(x) ((x)<MATRIX_TOLERANCE && (x)>-MATRIX_TOLERANCE)

// calculate matrix for unit square to quad mapping
void mapSquareToQuad(double quad[4][2],  // vertices of quadrilateral
                     double SQ[3][3])    // square->quad transform
{
    double px, py;

    px = quad[0][0]-quad[1][0]+quad[2][0]-quad[3][0];
    py = quad[0][1]-quad[1][1]+quad[2][1]-quad[3][1];

    if (MATRIX_ZERO(px) && MATRIX_ZERO(py))
    {
        SQ[0][0] = quad[1][0]-quad[0][0];
        SQ[1][0] = quad[2][0]-quad[1][0];
        SQ[2][0] = quad[0][0];
        SQ[0][1] = quad[1][1]-quad[0][1];
        SQ[1][1] = quad[2][1]-quad[1][1];
        SQ[2][1] = quad[0][1];
        SQ[0][2] = 0.;
        SQ[1][2] = 0.;
        SQ[2][2] = 1.;
        return;
    }
    else
    {
        double dx1, dx2, dy1, dy2, del;

        dx1 = quad[1][0]-quad[2][0];
        dx2 = quad[3][0]-quad[2][0];
        dy1 = quad[1][1]-quad[2][1];
        dy2 = quad[3][1]-quad[2][1];
        del = det2(dx1,dx2, dy1,dy2);

        SQ[0][2] = det2(px,dx2, py,dy2)/del;
        SQ[1][2] = det2(dx1,px, dy1,py)/del;
        SQ[2][2] = 1.;
        SQ[0][0] = quad[1][0]-quad[0][0]+SQ[0][2]*quad[1][0];
        SQ[1][0] = quad[3][0]-quad[0][0]+SQ[1][2]*quad[3][0];
        SQ[2][0] = quad[0][0];
        SQ[0][1] = quad[1][1]-quad[0][1]+SQ[0][2]*quad[1][1];
        SQ[1][1] = quad[3][1]-quad[0][1]+SQ[1][2]*quad[3][1];
        SQ[2][1] = quad[0][1];
    }
}

// calculate matrix for general quad to quad mapping
void mapQuadToQuad( double in[4][2],    // starting quad
                    double out[4][2],   // target quad
                    double ST[3][3])    // the matrix (returned)
{
    double quad[4][2], MS[3][3];
    double SM[3][3], MT[3][3];

    quad[0][0] = in[0][0]; quad[0][1] = in[0][1];
    quad[1][0] = in[1][0]; quad[1][1] = in[1][1];
    quad[2][0] = in[2][0]; quad[2][1] = in[2][1];
    quad[3][0] = in[3][0]; quad[3][1] = in[3][1];
    mapSquareToQuad(quad, MS);
    adjointMatrix(MS, SM);

    quad[0][0] = out[0][0] ; quad[0][1] = out[0][1] ;
    quad[1][0] = out[1][0] ; quad[1][1] = out[1][1] ;
    quad[2][0] = out[2][0] ; quad[2][1] = out[2][1] ;
    quad[3][0] = out[3][0] ; quad[3][1] = out[3][1] ;
    mapSquareToQuad(quad, MT);

    multiplyMatrix(SM, MT, ST);
}

/// QUAD WARPER

//--------------------------------------------------------------
phdWarper::phdWarper() {
	srcParalelogram = dstParalelogram = false;
	setSrcQuad(0,0, 1024, 0, 1024, 768, 0, 768);
	setDstQuad(0,0, 1024, 0, 1024, 768, 0, 768);
}

/// set/get the warp points
/// index: 0 - upper left, 1 - upper right, 2 - lower right, 3 - lower left 
void phdWarper::setTarget(unsigned int index, double _x, double _y) {
	if(index > 3) return;
	_dst[index][0] = _x;
	_dst[index][1] = _y;

	/*int a, b, c, d;

	if(dstParalelogram) {
		if(index < 3) { 
			_dst[3][0] = _dst[2][0] + (_dst[0][0] - _dst[1][0]); // d.x = c.x + (a.x - b.x)
			_dst[3][1] = _dst[2][1] + (_dst[0][1] - _dst[1][1]); // d.y = c.y + (a.y - b.y)
		} else {
			_dst[1][0] = _dst[2][0] + (_dst[0][0] - _dst[3][0]); // b.x = c.x + (a.x - d.x)
			_dst[1][1] = _dst[2][1] + (_dst[0][1] - _dst[3][1]); // b.y = c.y + (a.y - d.y)
		}
	}*/

	changed = true;
}

void phdWarper::setSource(unsigned int index, double _x, double _y) {
	if(index > 3) return;
	_src[index][0] = _x;
	_src[index][1] = _y;

	/*if(sides == 3) { // restrict point 4 --- paralelogram rule
		_src[3][0] = _src[2][0] + (_src[0][0] - _src[1][0]); // d.x = c.x + (a.x - b.x)
		_src[3][1] = _src[2][1] + (_src[0][1] - _src[1][1]); // d.y = c.y + (a.y - b.y)
	}*/

	/*if(srcParalelogram) {
		if(index < 3) { 
			_src[3][0] = _src[2][0] + (_src[0][0] - _src[1][0]); // d.x = c.x + (a.x - b.x)
			_src[3][1] = _src[2][1] + (_src[0][1] - _src[1][1]); // d.y = c.y + (a.y - b.y)
		} else {
			_src[1][0] = _src[2][0] + (_src[0][0] - _src[3][0]); // b.x = c.x + (a.x - d.x)
			_src[1][1] = _src[2][1] + (_src[0][1] - _src[3][1]); // b.y = c.y + (a.y - d.y)
		}
	}*/

	changed = true;
}

void phdWarper::getSource(unsigned int index, double &_x, double &_y) {
	if(index > 3) return;
	_x = _src[index][0];
	_y = _src[index][1];
}

void phdWarper::getTarget(unsigned int index, double &_x, double &_y) {
	if(index > 3) return;
	_x = _dst[index][0];
	_y = _dst[index][1];
}

void phdWarper::setSrcQuad(double _x1, double _y1, double _x2, double _y2, double _x3, double _y3, double _x4, double _y4) {
	setSource(0, _x1, _y1);
	setSource(1, _x2, _y2);
	setSource(2, _x3, _y3);
	setSource(3, _x4, _y4);
}

void phdWarper::setDstQuad(double _x1, double _y1, double _x2, double _y2, double _x3, double _y3, double _x4, double _y4) {
	setTarget(0, _x1, _y1);
	setTarget(1, _x2, _y2);
	setTarget(2, _x3, _y3);
	setTarget(3, _x4, _y4);
}

void phdWarper::warpPoint(float inX, float inY, float & outX, float & outY) {

	if(changed) { calcWarpMatrix(); }

	GLfloat z = 0.0;
	GLfloat r0 = 0.0; GLfloat r1 = 0.0; GLfloat r2 = 0.0; GLfloat r3 = 0.0;
	r0 = (float)(inX * glWarpMatrix[0] + inY*glWarpMatrix[4] + z*glWarpMatrix[8]  + 1.0*glWarpMatrix[12]);
	r1 = (float)(inX * glWarpMatrix[1] + inY*glWarpMatrix[5] + z*glWarpMatrix[9]  + 1.0*glWarpMatrix[13]);
	r2 = (float)(inX * glWarpMatrix[2] + inY*glWarpMatrix[6] + z*glWarpMatrix[10] + 1.0*glWarpMatrix[14]);
	r3 = (float)(inX * glWarpMatrix[3] + inY*glWarpMatrix[7] + z*glWarpMatrix[11] + 1.0*glWarpMatrix[15]);

	outX = outY = 0.0;

	if(r3 != 0.0) {
		outX = (float)(r0 / r3);
		outY = (float)(r1 / r3);
	}
}

void phdWarper::invWarpPoint(float inX, float inY, float & outX, float & outY) {

	if(changed) { calcWarpMatrix(); }

	GLfloat z = 0.0;
	GLfloat r0 = 0.0; GLfloat r1 = 0.0; GLfloat r2 = 0.0; GLfloat r3 = 0.0;
	r0 = (float)(inX * glInvWarpMatrix[0] + inY*glInvWarpMatrix[4] + z*glInvWarpMatrix[8]  + 1.0*glInvWarpMatrix[12]);
	r1 = (float)(inX * glInvWarpMatrix[1] + inY*glInvWarpMatrix[5] + z*glInvWarpMatrix[9]  + 1.0*glInvWarpMatrix[13]);
	r2 = (float)(inX * glInvWarpMatrix[2] + inY*glInvWarpMatrix[6] + z*glInvWarpMatrix[10] + 1.0*glInvWarpMatrix[14]);
	r3 = (float)(inX * glInvWarpMatrix[3] + inY*glInvWarpMatrix[7] + z*glInvWarpMatrix[11] + 1.0*glInvWarpMatrix[15]);

	outX = outY = 0.0;

	if(r3 != 0.0) {
		outX = (float)(r0 / r3);
		outY = (float)(r1 / r3);
	}
}

//--------------------------------------------------------------
// calculate transformation matrix
//--------------------------------------------------------------
// projection warping from a modified Theo example on the OF forums:
// http://threeblindmiceandamonkey.com/?p=31
//--------------------------------------------------------------
void phdWarper::resetWarpMatrix() {
	// we set it to the default - 0 translation
	// and 1.0 scale for x y z and w
	for(int i = 0; i < 16; i++) {
		if(i % 5 != 0) {
			glWarpMatrix[i] = 0.0;
			glInvWarpMatrix[i] = 0.0;
		} else {
			glWarpMatrix[i] = 1.0;
			glInvWarpMatrix[i] = 1.0;
		}
	}
}

void phdWarper::calcWarpMatrix() {

	if(!changed) return; // dont need any update

	// perform the warp calculation
	resetWarpMatrix();

	// perform the warp calculation SRC to DST
	mapQuadToQuad(_src, _dst, _warpMatrix);

	// copy the values
	glWarpMatrix[0]	= _warpMatrix[0][0];
	glWarpMatrix[1]	= _warpMatrix[0][1];
	glWarpMatrix[3]	= _warpMatrix[0][2];

	glWarpMatrix[4]	= _warpMatrix[1][0];
	glWarpMatrix[5]	= _warpMatrix[1][1];
	glWarpMatrix[7]	= _warpMatrix[1][2];

	glWarpMatrix[12] = _warpMatrix[2][0];
	glWarpMatrix[13] = _warpMatrix[2][1];
	glWarpMatrix[15] = _warpMatrix[2][2];

	// perform the warp calculation DST to SRC
	mapQuadToQuad(_dst, _src, _warpMatrix);

	// copy the values
	glInvWarpMatrix[0]	= _warpMatrix[0][0];
	glInvWarpMatrix[1]	= _warpMatrix[0][1];
	glInvWarpMatrix[3]	= _warpMatrix[0][2];

	glInvWarpMatrix[4]	= _warpMatrix[1][0];
	glInvWarpMatrix[5]	= _warpMatrix[1][1];
	glInvWarpMatrix[7]	= _warpMatrix[1][2];

	glInvWarpMatrix[12] = _warpMatrix[2][0];
	glInvWarpMatrix[13] = _warpMatrix[2][1];
	glInvWarpMatrix[15] = _warpMatrix[2][2];

	changed = false;
}

int phdWarper::findNearestSource(double _x, double _y) {
	int _index = -1;
	double _near = 1000000.0;
	for(int i = 0; i < sides; i++) {
		double _dist = 0.0;
		_dist = sqrt((_src[i][0]-_x)*(_src[i][0]-_x) + (_src[i][1]-_y)*(_src[i][1]-_y));
		if(_dist < _near && _dist < 20.0) {
			_near = _dist;
			_index = i;
		}
	}	
	return _index;
}

int phdWarper::findNearestTarget(double _x, double _y) {
	int _index = -1;
	double _near = 1000000.0;
	for(int i = 0; i < 4; i++) { // always search 4 points on target
		double _dist = 0.0;
		_dist = sqrt((_dst[i][0]-_x)*(_dst[i][0]-_x) + (_dst[i][1]-_y)*(_dst[i][1]-_y));
		if(_dist < _near && _dist < 20.0) {
			_near = _dist;
			_index = i;
		}
	}		
	return _index;
}

void phdWarper::getDstBounds(double &_x, double & _y, double & _w, double & _h) {

	double _x1 = _dst[0][0];
	double _y1 = _dst[0][1];
	double _x2 = _dst[0][0];
	double _y2 = _dst[0][1];

	for(int i = 1; i < sides; i++) {
		if(_x1 > _dst[i][0]) _x1 = _dst[i][0];
		if(_y1 > _dst[i][1]) _y1 = _dst[i][1];
		if(_x2 < _dst[i][0]) _x2 = _dst[i][0];
		if(_y2 < _dst[i][1]) _y2 = _dst[i][1];
	}

	_x = _x1; _y = _y1; _w = _x2-_x1;	_h = _y2-_y1;
}

void phdWarper::getSrcBounds(double &_x, double & _y, double & _w, double & _h) {

	double _x1 = _src[0][0];
	double _y1 = _src[0][1];
	double _x2 = _src[0][0];
	double _y2 = _src[0][1];

	for(int i = 1; i < sides; i++) {
		if(_x1 > _src[i][0]) _x1 = _src[i][0];
		if(_y1 > _src[i][1]) _y1 = _src[i][1];
		if(_x2 < _src[i][0]) _x2 = _src[i][0];
		if(_y2 < _src[i][1]) _y2 = _src[i][1];
	}

	_x = _x1; _y = _y1; _w = _x2-_x1;	_h = _y2-_y1;
}

void phdWarper::setSides(int _sides) {
	sides = _sides;
	if(sides < 3) sides = 3;
	if(sides > 4) sides = 4;
}

int phdWarper::getSides() {
	return sides;
}

void phdWarper::setup(double _width, double _height, int _sides) {
	setSides(_sides);
	setSrcQuad( 0, 0, _width, 0, _width, _height, 0, _height);
	setDstQuad( 0, 0, _width, 0, _width, _height, 0, _height);
}

void phdWarper::setup(double _srcW, double _srcH, double _dstW, double _dstH) {
	sides = 4;
	setSrcQuad(0, 0, _srcW, 0, _srcW, _srcH, 0, _srcH);
	setDstQuad(0, 0, _dstW, 0, _dstW, _dstH, 0, _dstH);
}

void phdWarper::setAsString(string & _setup) {

	vector<string> items = ofSplitString(_setup, "|", true, true);

	if(items.size() > 17 && items[0] == "W") {

		setSides(ofToInt(items[1]));

		double x1, y1, x2, y2, x3, y3, x4, y4;

		x1 = ofToFloat(items[2]);
		y1 = ofToFloat(items[3]);

		x2 = ofToFloat(items[4]);
		y2 = ofToFloat(items[5]);

		x3 = ofToFloat(items[6]);
		y3 = ofToFloat(items[7]);

		x4 = ofToFloat(items[8]);
		y4 = ofToFloat(items[9]);

		setSrcQuad(x1,y1,x2,y2,x3,y3,x4,y4);

		x1 = ofToFloat(items[10]);
		y1 = ofToFloat(items[11]);

		x2 = ofToFloat(items[12]);
		y2 = ofToFloat(items[13]);

		x3 = ofToFloat(items[14]);
		y3 = ofToFloat(items[15]);

		x4 = ofToFloat(items[16]);
		y4 = ofToFloat(items[17]);

		setDstQuad(x1,y1,x2,y2,x3,y3,x4,y4);
	}
}

string phdWarper::getAsString() {

	double x, y;
	string _str;

	_str = "W|";

	_str += ofToString(getSides()) + "|";

	getSource(0, x, y); _str += ofToString(x) + "|" + ofToString(y) + "|";
	getSource(1, x, y); _str += ofToString(x) + "|" + ofToString(y) + "|";
	getSource(2, x, y); _str += ofToString(x) + "|" + ofToString(y) + "|";
	getSource(3, x, y); _str += ofToString(x) + "|" + ofToString(y) + "|";

	getTarget(0, x, y); _str += ofToString(x) + "|" + ofToString(y) + "|";
	getTarget(1, x, y); _str += ofToString(x) + "|" + ofToString(y) + "|";
	getTarget(2, x, y); _str += ofToString(x) + "|" + ofToString(y) + "|";
	getTarget(3, x, y); _str += ofToString(x) + "|" + ofToString(y) + "|";

	_str += "\n";
	
	return _str;
}

void phdWarper::begin() {
	if(changed) calcWarpMatrix();
	glPushMatrix();
	glMultMatrixf(glWarpMatrix);
}

void phdWarper::end() {
	glPopMatrix();
}

void phdWarper::invBegin() {
	if(changed) calcWarpMatrix();
	glPushMatrix();
	glMultMatrixf(glInvWarpMatrix);
}

void phdWarper::invEnd() {
	glPopMatrix();
}
