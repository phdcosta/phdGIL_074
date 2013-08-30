#pragma once

#include "ofMain.h"

#include "phdDesigner.h"

//--------------------------------------------------------------------------------------------------------------
class phdDesignerFbo : public phdDesigner {
public:
	ofFbo canvas;
	phdDesignerFbo();
	phdDesignerFbo(float _x, float _y, float _viewWidth, float _viewHeight, ofFbo * _canvas, float _worldWidth, float _worldHeight);
	~phdDesignerFbo();
	virtual void setup(float _x, float _y, float _viewWidth, float _viewHeight, ofFbo * _canvas, float _worldWidth, float _worldHeight);
	virtual ofFbo * getCanvasPtr();
};
