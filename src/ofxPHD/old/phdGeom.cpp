#pragma once

#include "phdGeom.h"

ofVec2f rotatePoint(float x, float y, float cx, float cy, float ccx, float ccy, float sine, float cosine) {
	float ax = x - cx;
	float ay = y - cy;
	return ofVec2f(ccx + ax*cosine + ay*sine, ccy + ax*-sine + ay*cosine);
}

void circlesTangent(float _x1, float _y1, float _r1, float _x2, float _y2, float _r2, vector<ofVec2f> & _tangents) {
	double x1 = 200.0;
	double y1 = 200.0;
	double r1 = 50;

	double x2 = 250;
	double y2 = 200;
	double r2 = 30;

	double d_sq = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
	if (d_sq <= (r1-r2)*(r1-r2)) return;
 
	double d = sqrt(d_sq);
	double vx = (x2 - x1) / d;
	double vy = (y2 - y1) / d;
 
	double res[4][4];
	int i = 0;

	ofNoFill();
	ofSetColor(255,255,255,255);
	ofCircle(x1,y1,r1);
	ofCircle(x2,y2,r2);
	ofFill();

	for (int sign1 = +1; sign1 >= -1; sign1 -= 2) {
		double c = (r1 - sign1 * r2) / d;
		if(c*c > 1.0) continue;
		double h = sqrt(max(0.0, 1.0 - c*c));
		for (int sign2 = +1; sign2 >= -1; sign2 -= 2) {
			double nx = vx * c - sign2 * h * vy;
			double ny = vy * c + sign2 * h * vx;
			double a[4];
			a[0] = x1 + r1 * nx;
			a[1] = y1 + r1 * ny;
			a[2] = x2 + sign1 * r2 * nx;
			a[3] = y2 + sign1 * r2 * ny;

			_tangents.push_back(ofVec2f(a[0], a[1]));
			_tangents.push_back(ofVec2f(a[2], a[3]));
		}
	}
}

void circlesIntersection(double x0, double y0, double r0, double x1, double y1, double r1, vector<ofVec2f> & _points) {

	double xi, yi, xi_prime, yi_prime;
	double a, dx, dy, d, h, rx, ry;
	double x2, y2;

	dx = x1 - x0;
	dy = y1 - y0;

	d = hypot(dx,dy);

	if (d > (r0 + r1)) { return; }
	if (d < fabs(r0 - r1)) { return; }

	a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d) ;

	x2 = x0 + (dx * a/d);
	y2 = y0 + (dy * a/d);

	h = sqrt((r0*r0) - (a*a));

	rx = -dy * (h/d);
	ry = dx * (h/d);

	xi = x2 + rx;
	xi_prime = x2 - rx;
	yi = y2 + ry;
	yi_prime = y2 - ry;

	_points.push_back(ofVec2f(xi, yi));
	_points.push_back(ofVec2f(xi_prime, yi_prime));
}

bool getTriangleFromSegmentSize(double _szA, double _szB, double _szC, double x0, double y0, double _angle, double _scale, ofVec2f & _ptA, ofVec2f & _ptB, ofVec2f & _ptC) {

	double r0 = _szB * _scale;
	double x1 = 0 + _szA * _scale;
	double y1 = 0;
	double r1 = _szC * _scale;

	vector<ofVec2f> _points;

	circlesIntersection(0, 0, r0, x1, y1, r1, _points);

	if(_points.size() == 0) return false; // dont intersect --- sides dont make a triangle
	
	_ptA.set(0, 0);
	_ptB.set(x1, y1);
	_ptC.set(_points[1].x, _points[1].y);

	_ptC.x = _ptB.x + (_ptA.x - _ptC.x);
	_ptC.y = _ptB.y + (_ptA.y - _ptC.y);

	double _sin = sin(ofDegToRad(_angle));
	double _cos = cos(ofDegToRad(_angle));

	ofVec2f _pivot = (_ptA + _ptB + _ptC) / 3.0;
	
	_ptA -= _pivot;
	_ptB -= _pivot;
	_ptC -= _pivot;

	_ptA = rotatePoint(_ptA.x, _ptA.y, 0, 0, 0, 0, _sin, _cos);
	_ptB = rotatePoint(_ptB.x, _ptB.y, 0, 0, 0, 0, _sin, _cos);
	_ptC = rotatePoint(_ptC.x, _ptC.y, 0, 0, 0, 0, _sin, _cos);

	_ptA += ofVec2f(x0,y0);
	_ptB += ofVec2f(x0,y0);
	_ptC += ofVec2f(x0,y0);

	return true;
}

// compute the area of a triangle using Heron's formula
double triareaHeron(double a, double b, double c)
{
    double s = (a + b + c)/2.0;
    double area=sqrt(fabs(s*(s-a)*(s-b)*(s-c)));
    return area;     
}

// compute the distance between two 2d points
double dist(double x0, double y0, double z0, double x1, double y1, double z1)
{
    double a = x1 - x0;   
    double b = y1 - y0;
    double c = z1 - z0;
    return sqrt(a*a + b*b + c*c);
}

// calculate barycentric coordinates
// triangle 1st vertex: x0,y0,z0
// triangle 2nd vertex: x1,y1,z1
//  triangle 3rd vertex: x2,y2,z2
// point inside triangle: vx, vy,vz
// *u,*v,*w are the coordinates returned
void barycent(double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2,
                         double vx, double vy, double vz,
                         double *u, double *v, double *w)
{
    // compute the area of the big triangle
    double a = dist(x0, y0, z0, x1, y1, z1);
    double b = dist(x1, y1, z1, x2, y2, z2);
    double c = dist(x2, y2, z2, x0, y0, z0);
    double totalarea = triareaHeron(a, b, c);
        
    // compute the distances from the outer vertices to the inner vertex
    double length0 = dist(x0, y0, z0, vx, vy, vz);        
    double length1 = dist(x1, y1, z1, vx, vy, vz);        
    double length2 = dist(x2, y2, z2, vx, vy, vz);        
    
    // divide the area of each small triangle by the area of the big triangle
    *u = triareaHeron(b, length1, length2)/totalarea;
    *v = triareaHeron(c, length0, length2)/totalarea;
    *w = triareaHeron(a, length0, length1)/totalarea;          
}

void setIdentityMatrix(ofMatrix4x4 & _m) {
	_m(0,0) = 1.0; _m(0,1) = 0.0; _m(0,2) = 0.0; _m(0,3) = 0.0;
	_m(1,0) = 0.0; _m(1,1) = 1.0; _m(1,2) = 0.0; _m(1,3) = 0.0;
	_m(2,0) = 0.0; _m(2,1) = 0.0; _m(2,2) = 1.0; _m(2,3) = 0.0;
	_m(3,0) = 0.0; _m(3,1) = 0.0; _m(3,2) = 0.0; _m(3,3) = 1.0;
};

void setRotateMatrixX(ofMatrix4x4 & _m, double angle) {
	double sine = sin(angle);
	double cosine = cos(angle);
	setIdentityMatrix(_m);
	_m(1,1) = cosine; _m(1,2) = -sine;
	_m(2,1) = sine;   _m(2,2) = cosine;
};

void setRotateMatrixY(ofMatrix4x4 & _m, double angle) {
	double sine = sin(angle);
	double cosine = cos(angle);
	setIdentityMatrix(_m);
	_m(0,0) = cosine; _m(0,2) = sine;
	_m(2,0) = -sine;  _m(2,2) = cosine;
};

void setRotateMatrixZ(ofMatrix4x4 & _m, double angle) {
	double sine = sin(angle);
	double cosine = cos(angle);
	setIdentityMatrix(_m);
	_m(0,0) = cosine; _m(0,1) = -sine;
	_m(1,0) = sine;   _m(1,1) = cosine;
};

void setRotateMatrixXYZ(ofMatrix4x4 & _m, double angleX, double angleY, double angleZ) {
	ofMatrix4x4 _mx; setRotateMatrixX(_mx, angleX);
	ofMatrix4x4 _my; setRotateMatrixX(_my, angleY);
	ofMatrix4x4 _mz; setRotateMatrixX(_mz, angleZ);
	_m = _mx * _my * _mz;
};

void setScaleMatrix(ofMatrix4x4 & _m, double scaleX, double scaleY, double scaleZ) {
	setIdentityMatrix(_m);
	_m(0,0) = scaleX;
	_m(1,1) = scaleY;
	_m(2,2) = scaleZ;
};

void setTranslateMatrix(ofMatrix4x4 & _m, double moveX, double moveY, double moveZ) {
	setIdentityMatrix(_m);
	_m(3,0) = moveX;
	_m(3,1) = moveY;
	_m(3,2) = moveZ;
};

//--------------------------------------------------------------
// Code by Theo from URL above
//--------------------------------------------------------------
// http://www.openframeworks.cc/forum/viewtopic.php?f=9&t=1443
//--------------------------------------------------------------
bool phdPointInsidePolygon(ofPolyline & _points, float px, float py) {
	int N = _points.size();
	if (N == 0) return false;
	int counter = 0;
	int i;
	double xinters;
	ofPoint p1, p2;

	p1 = _points[0];
	for (int i = 1; i <= N; i++) {
		p2 = _points[i % N];
		if (py > MIN(p1.y,p2.y)) {
			if (py <= MAX(p1.y,p2.y)) {
				if (px <= MAX(p1.x,p2.x)) {
					if (p1.y != p2.y) {
						xinters = (py-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
						if (p1.x == p2.x || px <= xinters) counter++;
					}
				}
			}
		}
		p1 = p2;
	}

	if (counter % 2 == 0)
		return false;
	else
		return true;
}

double phdDistancePointLineSquared(double x, double y, double x1, double y1, double x2, double y2) {
	double A = x - x1;
	double B = y - y1;
	double C = x2 - x1;
	double D = y2 - y1;
	double dot = A * C + B * D;
	double len_sq = C * C + D * D;
	double param = dot / len_sq;
	double xx,yy;
	if(param < 0) {
		xx = x1;
		yy = y1;
	} else if(param > 1) {
		xx = x2;
		yy = y2;
	} else {
		xx = x1 + param * C;
		yy = y1 + param * D;
	}
	return ((x - xx) * (x - xx)) + ((y - yy) * (y - yy));
}

int phdPointNearEdge(ofPolyline & _points, double px, double py, double _dist, bool _closed) {

	int nPoints = _points.size();

	if(nPoints < 2) return -1;

	for(int i = 0; i < nPoints-1; i++) {
		if(phdDistancePointLineSquared(px, py, _points[i].x, _points[i].y, _points[i+1].x, _points[i+1].y) < _dist*_dist) return i;
	}
	if(_closed && nPoints > 2) {
		if(phdDistancePointLineSquared(px, py, _points[0].x, _points[0].y, _points[nPoints-1].x, _points[nPoints-1].y) < _dist*_dist) return nPoints-1;
	}

	return -1;
}

int phdPointOverVertex(ofPolyline & _points, double px, double py) {
	for(int i = 0; i < _points.size(); i++) {
		if(fabs(_points[i].x - px) < 5 && fabs(_points[i].y - py) < 5) {
			return i;
		}
	}
	return -1;
}

void getTransformedPolyline(ofPolyline & _src, ofPolyline & _dst, ofMatrix4x4 _mat) {
	_dst = _src;
	for(int i = 0; i < _dst.size(); i++) {
		_dst[i] = _dst[i] * _mat;
	}
}

// 2D cross product.
// Return a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
static CoordType cross(const hPoint &O, const hPoint &A, const hPoint &B)
{
	return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

void contourToConvexHull(ofPolyline &src, ofPolyline &dst) {

	dst.clear();

	vector<hPoint> P(src.size());
	for(int i = 0; i < src.size(); i++) {
		P[i].x = src[i].x;
		P[i].y = src[i].y;
	}

	int n = src.size(), k = 0;
	vector<hPoint> H(2*n);
 
	// Sort points lexicographically
	sort(P.begin(), P.end());
 
	// Build lower hull
	for (int i = 0; i < n; i++) {
		while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
		H[k++] = P[i];
	}
 
	// Build upper hull
	for (int i = n-2, t = k+1; i >= 0; i--) {
		while (k >= t && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
		H[k++] = P[i];
	}
 
	H.resize(k);

	for(int i = 0; i < H.size(); i++) { dst.addVertex(H[i].x + 500, H[i].y); }
}

double triareaVertices(double _ax, double _ay, double _bx, double _by, double _cx, double _cy) {
	return fabs(_ax*(_by-_cy) + _bx*(_cy-_ay) + _cx*(_ay-_by)) / 2.0;
}

void drawEdgeWithSizeLabel(double _x1, double _y1, double _x2, double _y2, string _label, bool _edge) {
	double _cx = (_x1+_x2) / 2.0;
	double _cy = (_y1+_y2) / 2.0;
	double _a  = atan2(_y2-_y1,_x2-_x1);

	if(_edge) ofLine(_x1,_y1,_x2,_y2);
	if(_a >= 0.0 && _a < PI*0.5) {  // x-  y+
		ofDrawBitmapString(_label, _cx - 80, _cy + 15);
	} else if(_a >= PI*0.5 && _a < PI) {  // x+  y-
		ofDrawBitmapString(_label, _cx - 80, _cy - 20);
	} else if(_a >= -PI && _a < -PI*0.5) {  // x-  y-
		ofDrawBitmapString(_label, _cx + 10, _cy - 20);
	} else if(_a >= -PI*0.5 && _a < 0.0) {  // x-  y+
		ofDrawBitmapString(_label, _cx + 10, _cy + 15);
	}
}

void drawGrid(float _xGap, float _yGap, ofTrueTypeFont * _font) {

	ofSetLineWidth(3);
	ofSetColor(255,255,255,128);
	if(_font != NULL) {
		for(int x = 0; x <= 1024; x += _xGap * 5) {
			for(int y = 0; y <= 768; y += _yGap * 5) {
				if(_font != NULL) _font->drawString(ofToString(y/64,0)+":"+ofToString(x/64,0), x+16, y+40);
			}
		}
	}

	ofSetLineWidth(2);
	ofSetColor(255,255,255,96);
	for(int x = 0; x <= 1024; x += _xGap) { ofLine(x, 0, x, 768); }
	for(int y = 0; y <= 768;  y += _yGap) { ofLine(0, y, 1024, y); }

	ofSetColor(255,255,255,255);
	for(int x = 0; x <= 1024; x += _xGap * 5) { ofLine(x, 0, x, 768); }
	for(int y = 0; y <= 768;  y += _yGap * 5) { ofLine(0, y, 1024, y); }

	ofSetColor(255,255,255,255);
	ofSetLineWidth(3);
	ofLine(0,0,1024,0);
	ofLine(1024,0,1024,768);
	ofLine(1024,768,0,768);
	ofLine(0,768,0,0);
	ofSetLineWidth(1);
}

void drawFilledBorderRectangle(float _x1, float _y1, float _w, float _h, ofColor & _fill, ofColor & _border) {
	ofFill();
	ofSetColor(_fill);
	ofRect(_x1,_y1,_w,_h);
	ofSetColor(_border);
	glBegin(GL_LINES);
		glVertex2f(_x1,_y1);		glVertex2f(_x1+_w,_y1);
		glVertex2f(_x1+_w,_y1);		glVertex2f(_x1+_w,_y1+_h);
		glVertex2f(_x1+_w,_y1+_h);	glVertex2f(_x1,_y1+_h);
		glVertex2f(_x1,_y1+_h);		glVertex2f(_x1,_y1);
	glEnd();
}