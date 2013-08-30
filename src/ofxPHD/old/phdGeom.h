#pragma once

#include "ofMain.h"

// rotate a point
ofVec2f rotatePoint(float x, float y, float cx, float cy, float ccx, float ccy, float sine, float cosine);

// calculate tangents between two circles
void circlesTangent(float _x1, float _y1, float _r1, float _x2, float _y2, float _r2, vector<ofVec2f> & _tangents);

// calculate intersection points between two circles
void circlesIntersection(double x0, double y0, double r0, double x1, double y1, double r1, vector<ofVec2f> & _points);

// calculate triangle points from segment sizes
bool getTriangleFromSegmentSize(double _szA, double _szB, double _szC, double x0, double y0, double _angle, double _scale, ofVec2f & _ptA, ofVec2f & _ptB, ofVec2f & _ptC);

// compute the area of a triangle using Heron's formula
double triareaHeron(double a, double b, double c);

// compute the distance between two 2d points
double dist(double x0, double y0, double z0, double x1, double y1, double z1);

// calculate barycentric coordinates
// triangle 1st vertex: x0,y0,z0
// triangle 2nd vertex: x1,y1,z1
//  triangle 3rd vertex: x2,y2,z2
// point inside triangle: vx, vy,vz
// *u,*v,*w are the coordinates returned
void barycent(double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2, double vx, double vy, double vz, double *u, double *v, double *w);

void setIdentityMatrix(ofMatrix4x4 & _m);
void setRotateMatrixX(ofMatrix4x4 & _m, double angle);
void setRotateMatrixY(ofMatrix4x4 & _m, double angle);
void setRotateMatrixZ(ofMatrix4x4 & _m, double angle);
void setRotateMatrixXYZ(ofMatrix4x4 & _m, double angleX, double angleY, double angleZ);
void setScaleMatrix(ofMatrix4x4 & _m, double scaleX, double scaleY, double scaleZ);
void setTranslateMatrix(ofMatrix4x4 & _m, double moveX, double moveY, double moveZ);

//--------------------------------------------------------------
// Code by Theo from URL above
//--------------------------------------------------------------
// http://www.openframeworks.cc/forum/viewtopic.php?f=9&t=1443
//--------------------------------------------------------------
bool phdPointInsidePolygon(ofPolyline & _points, float px, float py);

double phdPistancePointLineSquared(double x, double y, double x1, double y1, double x2, double y2);

int phdPointNearEdge(ofPolyline & _points, double px, double py, double _dist, bool _closed);

int phdPointOverVertex(ofPolyline & _points, double px, double py);

void getTransformedPolyline(ofPolyline & _src, ofPolyline & _dst, ofMatrix4x4 _mat);

//--------------------------------------------------------------
typedef long long CoordType;
struct hPoint {
	CoordType x, y;
 	bool operator <(const hPoint &p) const {
		return x < p.x || (x == p.x && y < p.y);
	}
};
 
void contourToConvexHull(ofPolyline &src, ofPolyline &dst);
//--------------------------------------------------------------

// area of triangle from its vertices
double triareaVertices(double _ax, double _ay, double _bx, double _by, double _cx, double _cy);

void drawEdgeWithSizeLabel(double _x1, double _y1, double _x2, double _y2, string _label, bool _edge);

void drawGrid(float _xGap, float _yGap, ofTrueTypeFont * _font);

void drawFilledBorderRectangle(float _x1, float _y1, float _w, float _h, ofColor & _fill, ofColor & _border);
