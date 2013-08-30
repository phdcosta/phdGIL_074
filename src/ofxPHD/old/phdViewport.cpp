#include "phdViewport.h"

//--------------------------------------------------------------------------------------------------------------
phdViewport::phdViewport() {
	checkBounds = false;
	setup(0,0,640,480,NULL,640,480);
}

phdViewport::phdViewport(float _x, float _y, float _w, float _h, ofFbo * _canvas, float _worldWidth, float _worldHeight) {
	checkBounds = false;
	setup(_x, _y, _w, _h, _canvas, _worldWidth, _worldHeight);
}

phdViewport::~phdViewport() {
	canvas = NULL;
}

void phdViewport::setup(float _x, float _y, float _w, float _h, ofFbo * _canvas, float _worldWidth, float _worldHeight) {
	setCanvas(_canvas, _worldWidth, _worldHeight);
	setScreenArea(ofPoint(_x,_y), ofPoint(_x+_w,_y+_h));
}

void phdViewport::setCanvas(ofFbo * _canvas, float _w, float _h) {

	canvas = _canvas;
	
	if(canvas != NULL) {
		worldSize = ofPoint(canvas->getWidth(),canvas->getHeight());
	} else {
		worldSize = ofPoint(_w,_h);
	}
	worldMin = ofPoint(0,0);
	worldMax = worldSize;
	setWorldViewBounds(worldMin, worldMax);
}

void phdViewport::updateMatrices() {

	const bool geometry = false; // false is good for bitmap coords / true is good for geometric coords

	if(!geometry) { // 
		ofMatrix4x4 mwct; mwct.translate(-worldViewMin.x, -worldViewMin.y, 0.0);	// world to client translate
		ofMatrix4x4 mwcs; mwcs.scale(scaleCS_WVS.x, scaleCS_WVS.y, 1.0);			// world to client scale
		ofMatrix4x4 mcst; mcst.translate(screenTopLeft.x, screenTopLeft.y, 0.0);	// client to screen translate
		ofMatrix4x4 mcss; mcss.scale(scaleWS_WVS.x, scaleWS_WVS.y, 1.0);

		wtc = mwct * mwcs;		// worldToClient
		cts = mcst /** mcss*/;				// clientToScreen
		wts = wtc * cts;		// worldToScreen

		ofMatrix4x4 mcwt; mcwt.translate(worldViewMin.x, worldViewMin.y, 0.0);		// client to world translate
		ofMatrix4x4 mcws; mcws.scale(1.0/scaleCS_WVS.x, 1.0/scaleCS_WVS.y, 1.0);	// client to world scale
		ofMatrix4x4 msct; msct.translate(-screenTopLeft.x, -screenTopLeft.y, 0.0);	// screen to client translate
		ofMatrix4x4 mscs; mcss.scale(1.0/scaleWS_WVS.x, 1.0/scaleWS_WVS.y, 1.0);

		ctw = mcws * mcwt;		// clienToWorld
		stc = msct /* mscs*/;				// screenToClient
		stw = stc * ctw;		// screenToWorld
	} else {
		ofMatrix4x4 mwct; mwct.translate(-worldViewMin.x, -worldViewMin.y, 0.0);	// world to client translate
		ofMatrix4x4 mwcs; mwcs.scale(scaleCS_WVS.x, -scaleCS_WVS.y, 1.0);			// world to client scale
		ofMatrix4x4 mcst; mcst.translate(screenTopLeft.x, screenTopLeft.y, 0.0);	// client to screen translate

		wtc = mwct * mwcs;		// worldToClient
		cts = mcst;				// clientToScreen
		wts = wtc * cts;		// worldToScreen

		ofMatrix4x4 mcwt; mcwt.translate(worldViewMin.x, worldViewMin.y, 0.0);		// client to world translate
		ofMatrix4x4 mcws; mcws.scale(1.0/scaleCS_WVS.x, 1.0/-scaleCS_WVS.y, 1.0);	// client to world scale
		ofMatrix4x4 msct; msct.translate(-screenTopLeft.x, -screenTopLeft.y, 0.0);	// screen to client translate

		ctw = mcws * mcwt;		// clienToWorld
		stc = msct;				// screenToClient
		stw = stc * ctw;		// screenToWorld
	}
}

void phdViewport::setCenter(ofPoint _value) {

	center = _value;

	float _cw = screenBottomRight.x - screenTopLeft.x; // client width and height
	float _ch = screenBottomRight.y - screenTopLeft.y;

	ofPoint _worldViewMin;
	ofPoint _worldViewMax;

	_worldViewMin.x = center.x - (_cw / _value.x) / 2.0;
	_worldViewMin.y = center.y - (_ch / _value.y) / 2.0;

	_worldViewMax.x = center.x + (_cw / _value.x) / 2.0;
	_worldViewMax.y = center.y + (_ch / _value.y) / 2.0;

	setWorldViewBounds(_worldViewMin, _worldViewMax);
}

void phdViewport::setMax(ofPoint _value) {
	setWorldViewBounds(worldViewMin, _value);
}

void phdViewport::setMin(ofPoint _value) {
	setWorldViewBounds(_value, worldViewMax);
}

void phdViewport::setScale(ofPoint _value) {

	float _cw = screenBottomRight.x - screenTopLeft.x; // client width and height
	float _ch = screenBottomRight.y - screenTopLeft.y;

	ofPoint _worldViewMin = worldViewMin;
	ofPoint _worldViewMax = worldViewMin + ofPoint(_cw / _value.x, _ch / _value.y);

	setWorldViewBounds(_worldViewMin, _worldViewMax);
}

void phdViewport::setScreenArea(ofPoint _scrTopLeft, ofPoint _scrBottomRight) {

	float _w = fabs(_scrBottomRight.x - _scrTopLeft.x);
	float _h = fabs(_scrBottomRight.y - _scrTopLeft.y);

	if(_w == 0 || _h == 0) return;

	screenTopLeft = _scrTopLeft;
	screenBottomRight = _scrBottomRight;

	horScrollPos.set(screenTopLeft.x, screenBottomRight.y);
	verScrollPos.set(screenBottomRight.x, screenTopLeft.y);

	clientSize.set(_w,_h);

	setWorldViewBounds(worldViewMin, worldViewMax);
}

void phdViewport::setWorldViewBounds(ofPoint _worldViewMin, ofPoint _worldViewMax) {

	float _cw = screenBottomRight.x - screenTopLeft.x; // client width and height
	float _ch = screenBottomRight.y - screenTopLeft.y;

	float _oww = worldViewMax.x - worldViewMin.x; // old world width and height
	float _owh = worldViewMax.y - worldViewMin.y;

	float _nww = _worldViewMax.x - _worldViewMin.x; // new world width and height
	float _nwh = _worldViewMax.y - _worldViewMin.y;

	center.x = (_worldViewMax.x + _worldViewMin.x) / 2.0;
	center.y = (_worldViewMax.y + _worldViewMin.y) / 2.0;

	if(checkBounds) { // check bounds

		worldViewMin.x = MAX(_worldViewMin.x, 0);
		worldViewMin.y = MAX(_worldViewMin.y, 0);
		worldViewMax.x = MIN(_worldViewMax.x, worldMax.x);
		worldViewMax.y = MIN(_worldViewMax.y, worldMax.y);

		_nww = worldViewMax.x - worldViewMin.x; // new world width and height
		_nwh = worldViewMax.y - worldViewMin.y;

		center.x = (worldViewMax.x + worldViewMin.x) / 2.0;
		center.y = (worldViewMax.y + worldViewMin.y) / 2.0;
	}

	scaleCS_WVS.x = 1.0; if(_nww > 0) scaleCS_WVS.x = fabs(_cw / _nww);
	scaleCS_WVS.y = 1.0; if(_nwh > 0) scaleCS_WVS.y = fabs(_ch / _nwh);
	if(scaleCS_WVS.x < scaleCS_WVS.y) scaleCS_WVS.y = scaleCS_WVS.x; else scaleCS_WVS.x = scaleCS_WVS.y;

	scaleWS_WVS.x = 1.0; if(_nww > 0) scaleWS_WVS.x = fabs(worldSize.x / _nww);
	scaleWS_WVS.y = 1.0; if(_nwh > 0) scaleWS_WVS.y = fabs(worldSize.y / _nwh);
	if(scaleWS_WVS.x < scaleWS_WVS.y) scaleWS_WVS.y = scaleWS_WVS.x; else scaleWS_WVS.x = scaleWS_WVS.y;

	worldViewMin.x = center.x - (_cw / scaleCS_WVS.x) / 2.0;
	worldViewMin.y = center.y - (_ch / scaleCS_WVS.y) / 2.0;

	worldViewMax.x = center.x + (_cw / scaleCS_WVS.x) / 2.0;
	worldViewMax.y = center.y + (_ch / scaleCS_WVS.y) / 2.0;

	worldViewSize = worldViewMax - worldViewMin;

	updateMatrices();

	updateScrollBarsPos();
}

void phdViewport::translate(ofPoint _value) {

	setWorldViewBounds(worldViewMin + _value, worldViewMax + _value);

}

void phdViewport::updateScrollBarsPos() {

	ofPoint _invisible = worldSize-worldViewSize;
	ofPoint _position = worldViewMin / _invisible;

	float _x = screenTopLeft.x + (screenBottomRight.x-screenTopLeft.x) * _position.x;
	float _y = screenBottomRight.y;

	_x = MAX(_x, screenTopLeft.x); _x = MIN(_x, screenBottomRight.x);
	horScrollPos.set(_x,_y);
		
	_x = screenBottomRight.x;
	_y = screenTopLeft.y + (screenBottomRight.y-screenTopLeft.y) * _position.y;

	_y = MAX(_y, screenTopLeft.y); _y = MIN(_y, screenBottomRight.y);
	verScrollPos.set(_x,_y);
}

void phdViewport::setHorScrollBarPos(float _pos) {
	float _invisible = worldSize.x-worldViewSize.x;
	float _position = (_pos-screenTopLeft.x) / (screenBottomRight.x-screenTopLeft.x);
	ofPoint _worldViewMin = ofPoint(_invisible * _position, worldViewMin.y);
	setWorldViewBounds(_worldViewMin, _worldViewMin + worldViewSize);
}

void phdViewport::setVerScrollBarPos(float _pos) {
	float _invisible = worldSize.y-worldViewSize.y;
	float _position = (_pos-screenTopLeft.y) / (screenBottomRight.y-screenTopLeft.y);
	ofPoint _worldViewMin = ofPoint(worldViewMin.x, _invisible * _position);
	setWorldViewBounds(_worldViewMin, _worldViewMin + worldViewSize);
}

void phdViewport::drawLine(ofPoint _pointA, ofPoint _pointB, ofMatrix4x4 & _mat) {
	ofPoint _pa = _pointA * _mat;
	ofPoint _pb = _pointB * _mat;
	ofLine(_pa, _pb);
}

void phdViewport::drawPolyline(ofPolyline & _polyline, ofMatrix4x4 & _mat) {
	for(int i = 0; i < _polyline.size()-1; i++) {
		drawLine(_polyline[i], _polyline[i+1], _mat);
	}
	if(_polyline.isClosed() && _polyline.size() > 2) drawLine(_polyline[_polyline.size()-1], _polyline[0], _mat);
}

void phdViewport::drawRectangle(ofPoint _pointA, ofPoint _pointB, ofMatrix4x4 & _mat) {
	drawLine(_pointA, ofPoint(_pointB.x, _pointA.y), _mat);
	drawLine(ofPoint(_pointB.x, _pointA.y), _pointB, _mat);
	drawLine(_pointB, ofPoint(_pointA.x, _pointB.y), _mat);
	drawLine(ofPoint(_pointA.x, _pointB.y), _pointA, _mat);
}

void phdViewport::drawGrid(float _gridX, float _gridY) {

	ofSetColor(255,255,255,100);

	ofPoint _a = worldViewMin;
	ofPoint _b = worldViewMax;
	ofPoint _s = ofPoint(_gridX,_gridY,0);
	ofPoint _i = _a / _s; _i = ofPoint((int)_i.x,(int)_i.y,0.0);
	ofPoint _c = _s * _i;
	while (_c.x < _b.x) {
		drawLine(ofPoint(_c.x, _a.y), ofPoint(_c.x, _b.y), wts);
		_c += _s;
	}
	_c = _s * _i;
	while (_c.y < _b.y) {
		drawLine(ofPoint(_a.x, _c.y), ofPoint(_b.x, _c.y), wts);
		_c += _s;
	}
	ofSetColor(255,0,0,255);
	if(worldMin.x < 0 && worldMax.x > 0) { drawLine(ofPoint(0.0, worldViewMin.y), ofPoint(0.0, worldViewMax.y), wts); } // x axis
	if(worldMin.y < 0 && worldMax.y > 0) { drawLine(ofPoint(worldViewMin.x, 0.0), ofPoint(worldViewMax.x, 0.0), wts); } // y axis
	ofSetColor(255,255,255,255);
}

void phdViewport::draw() {

	ofSetColor(255,255,255,255);

	ofPoint _aw = worldViewMin;
	if(_aw.x < 0) _aw.x = 0; if(_aw.x > worldSize.x) _aw.x = worldSize.x;
	if(_aw.y < 0) _aw.y = 0; if(_aw.y > worldSize.y) _aw.y = worldSize.y;
	ofPoint _ac = _aw * wts;

	ofPoint _bw = worldViewMax;
	if(_bw.x < 0) _bw.x = 0; if(_bw.x > worldSize.x) _bw.x = worldSize.x;
	if(_bw.y < 0) _bw.y = 0; if(_bw.y > worldSize.y) _bw.y = worldSize.y;
	ofPoint _bc = _bw * wts;

	// draw a portion of visual->canvas adjusting scale e position to the viewport
	if(canvas != NULL) {
		canvas->getTextureReference().bind();
		glBegin(GL_QUADS);
			glTexCoord2f(_aw.x, _aw.y); glVertex2d(_ac.x, _ac.y);
			glTexCoord2f(_bw.x, _aw.y); glVertex2d(_bc.x, _ac.y);
			glTexCoord2f(_bw.x, _bw.y); glVertex2d(_bc.x, _bc.y);
			glTexCoord2f(_aw.x, _bw.y); glVertex2d(_ac.x, _bc.y);
		glEnd();
		canvas->getTextureReference().unbind();
	}

	// scrool bars
	if(worldSize.x > worldViewSize.x || worldSize.y > worldViewSize.y) {
		updateScrollBarsPos();
		ofSetColor(255,255,255,255);
		ofCircle(horScrollPos.x, horScrollPos.y, 6);
		ofCircle(verScrollPos.x, verScrollPos.y, 6);
	}

	// border line
	ofSetColor(255,255,255,255);
	ofDrawBitmapString("x:" + ofToString(mousePos.x,2) + " y:" + ofToString(mousePos.y,2), screenTopLeft.x+3, screenBottomRight.y-10);
	ofLine(screenTopLeft.x, screenTopLeft.y, screenBottomRight.x, screenTopLeft.y);
	ofLine(screenTopLeft.x, screenTopLeft.y, screenTopLeft.x, screenBottomRight.y);
	ofLine(screenBottomRight.x, screenTopLeft.y, screenBottomRight.x, screenBottomRight.y);
	ofLine(screenTopLeft.x, screenBottomRight.y, screenBottomRight.x, screenBottomRight.y);

	if(showGrid) drawGrid(100, 100);
}

void phdViewport::mouseMoved(ofMouseEventArgs& args) {
	setScreenToWorldMousePos(ofPoint(args.x, args.y));
}

void phdViewport::mousePressed(ofMouseEventArgs& args) {
	setScreenToWorldMousePos(ofPoint(args.x, args.y));
}

void phdViewport::mouseDragged(ofMouseEventArgs& args) {
	setScreenToWorldMousePos(ofPoint(args.x, args.y));
}

void phdViewport::mouseReleased(ofMouseEventArgs& args) {
	setScreenToWorldMousePos(ofPoint(args.x, args.y));
}
