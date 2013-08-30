#include "phdGenParams.h"

//--------------------------------------------------------------------------------------------------
phdGenParams::phdGenParams() {

	time = 0.0;

	resolution[0] = 320.0; resolution[1] = 240.0;
	
	mouse[0] = 0.0; mouse[1] = 0.0;

	surfaceSize[0] = 320.0; surfaceSize[1] = 240.0;

	clrBase[0] = 1.0; clrBase[1] = 1.0; clrBase[2] = 1.0; clrBase[3] = 1.0;

	clrLow[0] = 1.0; clrLow[1] = 1.0; clrLow[2] = 1.0; clrLow[3] = 1.0;

	clrHigh[0] = 1.0; clrHigh[1] = 1.0; clrHigh[2] = 1.0; clrHigh[3] = 1.0;

	params[0] = 0.0; params[1] = 0.0; params[2] = 0.0; params[3] = 0.0;

	pos[0] = 0.0; pos[1] = 0.0;

	scale[0] = 1.0; scale[1] = 1.0;

	fftA[0] = 0.0; fftA[1] = 0.0; fftA[2] = 0.0; fftA[3] = 0.0;

	fftB[0] = 0.0; fftB[1] = 0.0; fftB[2] = 0.0; fftB[3] = 0.0;

	tex0 = NULL;
	size0[0] = 1.0; size0[1] = 1.0;

	tex1 = NULL;
	size1[0] = 1.0; size1[1] = 1.0;

	updateBackground = false;
	faderPos = 0;
	canvas = NULL;
	grabber = NULL;
	clipW = 1.0;
	clipH = 1.0;
}
