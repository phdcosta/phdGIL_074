#include "phdVertices.h"

//--------------------------------------------------------------------------------------------------------------
phdVertices::phdVertices() {
}

ofPoint & phdVertices::operator[] (int _index) { return vertices[_index]; }

int phdVertices::size() {
	return vertices.size();
}

void phdVertices::addVertex(double _x, double _y) {
	vertices.push_back(ofPoint(_x, _y, 0.0));
}

void phdVertices::addVertex(ofPoint _point) {
	vertices.push_back(_point);
}

void phdVertices::setVertex(int _index, double _x, double _y) {
	if(_index > -1 && _index < vertices.size()) {
		vertices[_index].x = _x;
		vertices[_index].y = _y;
	}
}

void phdVertices::insertVertex(int _index, ofPoint _point) {
	if(_index <= vertices.size()) {
		vertices.insert(vertices.begin() + _index, _point);
	} else {
		addVertex(_point);
	}
}

void phdVertices::insertVertex(int _index, double _x, double _y) {
	insertVertex(_index, ofPoint(_x, _y, 0.0));
}

void phdVertices::transform(ofMatrix4x4 _mat) {
	for(int i = 0; i < size(); i++) {
		vertices[i] = vertices[i] * _mat;
	}
}

void phdVertices::setTransformed(ofPolyline _src, ofMatrix4x4 _mat) {
	vertices.clear();
	for(int i = 0; i < _src.size(); i++) {
		vertices.push_back(_src[i] * _mat);
	}
}

void phdVertices::getTransformed(ofPolyline & _dst, ofMatrix4x4 _mat) {
	_dst.clear();
	for(int i = 0; i < size(); i++) {
		_dst.addVertex(vertices[i] * _mat);
	}
}

ofRectangle phdVertices::getBoundingBox() {
	if(size() > 0) {
		double _x1, _y1, _x2, _y2;
		_x1 = _x2 = vertices[0].x;
		_y1 = _y2 = vertices[0].y;
		for(int i = 1; i < size(); i++) {
			if(vertices[i].x < _x1) _x1 = vertices[i].x;
			if(vertices[i].x > _x2) _x2 = vertices[i].x;
			if(vertices[i].y < _y1) _y1 = vertices[i].y;
			if(vertices[i].y > _y2) _y2 = vertices[i].y;
		}
		return ofRectangle(_x1, _y1, _x2-_x1, _y2-_y1);
	}
	return ofRectangle(0,0,0,0);
}
