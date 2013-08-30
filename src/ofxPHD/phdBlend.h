#pragma once

#include "ofMain.h"

class phdBlend {
protected:
	ofShader shader;

public:
	phdBlend();
	void draw(int _blendMode, ofTexture & _tex0, float _factor0, ofTexture & _tex1, float _factor1, float _w, float _h);
	void loadShader();
};