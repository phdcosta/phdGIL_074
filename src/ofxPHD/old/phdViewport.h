#pragma once

#include "ofMain.h"

//--------------------------------------------------------------------------------------------------------------
class phdViewport {
public:
	bool showGrid;

	ofPoint screenTopLeft;
	ofPoint screenBottomRight;
	ofPoint clientSize;
	ofPoint worldViewMin;
	ofPoint worldViewMax;
	ofPoint worldViewSize;
	ofPoint worldSize;
	ofPoint worldMin;
	ofPoint worldMax;
	ofPoint center;
	ofPoint scaleCS_WVS;
	ofPoint scaleWS_WVS;

	ofPoint verScrollPos;
	ofPoint horScrollPos;

	ofMatrix4x4 wtc, wts; // worldToClient  // worldToScreen
	ofMatrix4x4 ctw, stw; // clientToWorld  // screenToWorld
	ofMatrix4x4 cts, stc; // clientToScreen // screenToClient
	ofPoint mousePos;

	ofFbo * canvas;	// world canvas
	bool checkBounds;	// limit to 0, 0, worldWidth, worldHeight

	phdViewport();
	phdViewport(float _x, float _y, float _w, float _h, ofFbo * _canvas, float _worldWidth, float _worldHeight);
	~phdViewport();

	void setup(float _x, float _y, float _w, float _h, ofFbo * _canvas, float _worldWidth, float _worldHeight);
	void setCanvas(ofFbo * _canvas, float _w, float _h);

	void updateMatrices();

	ofPoint screenToClient(ofPoint _value) { return _value * stc; }
	ofPoint clientToScreen(ofPoint _value) { return _value * cts; }

	ofPoint worldToClient(ofPoint _value) { return _value * wtc; }
	ofPoint clientToWorld(ofPoint _value) { return _value * ctw; }

	ofPoint screenToWorld(ofPoint _value) { return _value * stw; }
	ofPoint worldToScreen(ofPoint _value) { return _value * wts; }

	void setScreenToWorldMousePos(ofPoint _mousePos) { 
		mousePos = screenToWorld(_mousePos);
		if(checkBounds) {
			if(mousePos.x < 0 || mousePos.y < 0 || mousePos.x > worldSize.x || mousePos.y > worldSize.y) {
				mousePos.x = mousePos.y = -1;
			}
		}
	}
	ofPoint getWorldToScreenMousePos() { return worldToScreen(mousePos); }

	ofPoint getFromGlobalToViewport(float _x, float _y) {
		float _u = _x * scaleCS_WVS.x + screenTopLeft.x - worldViewMin.x * scaleCS_WVS.x;
		float _v = _y * scaleCS_WVS.y + screenTopLeft.y - worldViewMin.y * scaleCS_WVS.y;
		_u = MAX(_u, screenTopLeft.x); _u = MIN(_u, screenBottomRight.x);
		_v = MAX(_v, screenTopLeft.y); _v = MIN(_v, screenBottomRight.y);
		return ofPoint(_u, _v);
	}

	void zoom(float _zoom) { setScale(scaleCS_WVS * _zoom); }
	void zoomIn() { setScale(scaleCS_WVS * 1.10); }
	void zoomOut() { setScale(scaleCS_WVS / 1.10); }
	void fitAll() { setWorldViewBounds(ofPoint(0,0), ofPoint(worldSize.x, worldSize.y)); translate(-worldViewMin); }
	void fitWidth() { setWorldViewBounds(ofPoint(0,0), ofPoint(worldSize.x, worldSize.y * scaleCS_WVS.y)); translate(-worldViewMin); }
	void fitHeight() { setWorldViewBounds(ofPoint(0,0), ofPoint(worldSize.x * scaleCS_WVS.x, worldSize.y)); translate(-worldViewMin); }

	void setCenter(ofPoint _value);
	void setMax(ofPoint _value);
	void setMin(ofPoint _value);
	void setScale(ofPoint _value);

	void setScreenArea(ofPoint _scrTopLeft, ofPoint _scrBottomRight);
	void setWorldViewBounds(ofPoint _worldViewMin, ofPoint _worldViewMax);

	void translate(ofPoint _value);

	void updateScrollBarsPos();
	void setHorScrollBarPos(float _pos);
	void setVerScrollBarPos(float _pos);

	void drawLine(ofPoint _pointA, ofPoint _pointB, ofMatrix4x4 & _mat);
	void drawPolyline(ofPolyline & _polyline, ofMatrix4x4 & _mat);
	void drawRectangle(ofPoint _pointA, ofPoint _pointB, ofMatrix4x4 & _mat);

	void drawGrid(float _gridX, float _gridY);
	void draw();

	void mouseMoved(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
};
