#pragma once

#include "ofMain.h"

//--------------------------------------------------------------------------------------------------------------
class phdGenParams {
public:
	double time;
	double resolution[2];
	double mouse[2];
	double surfaceSize[2];
	double clrBase[4];
	double clrLow[4];
	double clrHigh[4];
	double params[4];
	double pos[2];
	double scale[2];
	double fftA[4];
	double fftB[4];

	ofTexture * tex0;
	double size0[2];

	ofTexture * tex1;
	double size1[2];

	phdGenParams();
};
