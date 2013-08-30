#pragma once

#include "ofMain.h"

//--------------------------------------------------------------------------------------------------------------
class phdTimer {
protected:
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

//--------------------------------------------------------------------------------------------------------------
class phdTimelineRange {
public:
	float x, y, w, h;
	float duration;
	float startPos;
	float endPos;
	float curPos;
	float speed;
	float lastUpdate;

	phdTimelineRange() {
		lastUpdate = ofGetElapsedTimef();
	}

	void update() {
		float _now = ofGetElapsedTimef();
		if(lastUpdate < _now) {
			float _elapsed = _now - lastUpdate;
			curPos += _elapsed * speed;
			if(curPos < startPos || curPos > endPos) {
				if(speed > 0.0) {

				}
			}
		}
	}

	void draw() {
	}
};
