#include "phdGimbal2d.h"

//--------------------------------------------------------------------------------------------------------------
phdGimbal2d::phdGimbal2d() {
	w = h = cx = cy = angle = 0.0; sx = sy = 1.0;
}
	
void phdGimbal2d::translate(double _dx, double _dy) {
	cx += _dx; cy += _dy;
}

void phdGimbal2d::rotate(double _da) {
	angle += _da;
}

void phdGimbal2d::resize(double _sx, double _sy) {
	sx *= _sx; sy *= _sy;
}

ofMatrix4x4 phdGimbal2d::getOTSMatrix() {
	ofMatrix4x4 ms; ms.scale(sx, sy, 1.0);
	ofMatrix4x4 mr; mr.rotate(angle, 0.0, 0.0, 1.0);
	ofMatrix4x4 mt; mt.translate(cx, cy, 0.0);
	return ms * mr * mt;
}

ofMatrix4x4 phdGimbal2d::getSTOMatrix() {
	ofMatrix4x4 ms; ms.scale(1.0/sx, 1.0/sy, 1.0);
	ofMatrix4x4 mr; mr.rotate(-angle, 0.0, 0.0, 1.0);
	ofMatrix4x4 mt; mt.translate(-cx, -cy, 0.0);
	return mt * mr * ms;
}

void phdGimbal2d::updateByRectangle(ofRectangle _rect) {
	w = _rect.width;
	h = _rect.height;
	cx = _rect.x + (w / 2.0);
	cy = _rect.y + (h / 2.0);
	angle = 0.0;
	sx = w / 2.0;
	sy = h / 2.0;
}

void phdGimbal2d::getKeyPoints(ofPolyline & _keys) {
	_keys.clear();
	_keys.addVertex( 1.00, 0.0);
	_keys.addVertex( 0.75, 0.0);
	_keys.addVertex( 0.00, 0.0);
	_keys.addVertex( 0.00,-1.0);
	getTransformedPolyline(_keys, _keys, getOTSMatrix());
}

void phdGimbal2d::draw(ofMatrix4x4 & _mat) {
	ofPolyline _keys; getKeyPoints(_keys); getTransformedPolyline(_keys,_keys,_mat);
	if(_keys.size() > 1) {
		glBegin(GL_LINES);
		for(int i = 0; i < _keys.size()-1; i++) {
			glVertex2f(_keys[i].x, _keys[i].y);
			glVertex2f(_keys[i+1].x, _keys[i+1].y);
		}
		glEnd();
		ofFill();
		ofRect(_keys[0].x-3, _keys[0].y-3, 6, 6);	// drag to scale width
		ofCircle(_keys[1].x, _keys[1].y, 4);		// drag to rotate
		ofCircle(_keys[2].x, _keys[2].y, 5);		// drag to translate
		ofRect(_keys[3].x-3, _keys[3].y-3, 6, 6);	// drag to scale height
	}
}

