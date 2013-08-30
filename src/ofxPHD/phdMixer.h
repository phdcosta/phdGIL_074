#pragma once

#include "ofMain.h"

#include "phdClip.h"

class phdMixer : public phdClip {
protected:

public:
	phdClip * clipA;
	phdClip * clipB;

	phdMixer();

	int getTrackCount() { return 2; }

	void fadeIn();
	void fadeOut();

	void play();
	void pause();
	void stop();

	void setup();
	void update();
	void draw(float _x, float _y, float _w, float _h);
};