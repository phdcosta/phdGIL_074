#pragma once

#include "ofMain.h"

#include "phdGenBase.h"
#include "phdClip.h"
#include "phdBlend.h"

//--------------------------------------------------------------------------------------------------------------
class phdClipTrack : public phdGenBase {
private:
	ofFbo curDrawCanvas;
	ofFbo curCropCanvas;
	ofFbo oldDrawCanvas;
	ofFbo oldCropCanvas;
	ofFbo trackCanvas;
	bool allocated;

	phdBlend blendShader;

	void allocateCanvas();

public:
	phdClipList clips;

	phdClip * curClip;
	phdClip * oldClip;

	phdClipTrack();

	phdClassCode getClassCode() { return phdClipTrackClassCode; }
	int getTrackCount() { return 1; }

	void setWidth(float _width);
	void setHeight(float _height);

	void setup();

	void play();
	void pause();
	void stop();

	void update(phdGenParams * _params = NULL);

	void draw(phdGenParams * _params = NULL);
	void draw(float _x, float _y, float _w, float _h, phdGenParams * _params = NULL);

	// ----------- class specific methods ---------------
	void setCurClip(int _index);
	void setCurClip(phdClip * _clip);

	void fadeIn();
	void fadeOut();
};
