#pragma once

#include "ofMain.h"

#include "phdGenBase.h"
#include "phdClipTrack.h"

class phdTrackMixer : public phdGenBase {
private:
	ofFbo mixerCanvas;
	bool allocated;
	void allocateCanvas();

public:
	phdClipTrack trackA;
	phdClipTrack trackB;

	phdTrackMixer();

	phdClassCode getClassCode() { return phdTrackMixerClassCode; }
	int getTrackCount() { return 2; }

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

	void fadeIn();
	void fadeOut();
};