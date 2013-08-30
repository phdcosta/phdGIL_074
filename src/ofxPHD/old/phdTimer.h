#pragma once

#include "ofMain.h"

//--------------------------------------------------------------------------------------------------------------
class phdTimer {
private:
	float lastTime;
	float startTime;
	float speed;
	float curTime;

public:
	phdTimer();
	inline void setSpeed(float _speed);
	inline float getSpeed();
	void start();
	float getElapsedTotalTime();
	float getElapsedFrameTime();
	void update();
	void setClipRange(float _start, float _end);
};
