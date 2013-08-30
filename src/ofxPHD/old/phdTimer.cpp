#include "phdTimer.h"

//--------------------------------------------------------------------------------------------------------------
phdTimer::phdTimer() {
	startTime = ofGetElapsedTimef();
	lastTime = startTime;
	curTime = 0.0;
	speed = 1.0;
}

void phdTimer::setSpeed(float _speed) {
	speed = _speed;
}

float phdTimer::getSpeed() {
	return speed;
}

void phdTimer::start() {
	startTime = ofGetElapsedTimef();
	lastTime = startTime;
	curTime = 0.0;
}

float phdTimer::getElapsedTotalTime() {
	lastTime = ofGetElapsedTimef();
	return lastTime - startTime;
}

float phdTimer::getElapsedFrameTime() {
	float _oldLast = lastTime;
	lastTime = ofGetElapsedTimef();
	return lastTime - _oldLast;
}

void phdTimer::update() {
	float _oldLast = lastTime;
	lastTime = ofGetElapsedTimef();
	curTime += (lastTime - _oldLast) * speed;
}

void phdTimer::setClipRange(float _start, float _end) {

}

