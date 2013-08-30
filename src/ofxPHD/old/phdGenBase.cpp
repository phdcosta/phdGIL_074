#include "phdGenBase.h"

//--------------------------------------------------------------------------------------------------------------
phdGenBase::phdGenBase() {
	setup();
}

void phdGenBase::setWidth(float _width) {
	width = _width;
}

float phdGenBase::getWidth() {
	return 320.0;
}

void phdGenBase::setHeight(float _height) {
	height = _height;
}

float phdGenBase::getHeight() {
	return 240.0;
}

void phdGenBase::setFlagUpdate() {
	updated = false;
}

void phdGenBase::play() {
	paused = false;
}

void phdGenBase::pause() {
	paused = true;
}

void phdGenBase::stop() {
	paused = true;
}

void phdGenBase::setup() {
	paused = true;
}

void phdGenBase::update(phdGenParams * _params) {
	if(!paused && !updated) {
		// do all updates if not paused and not updates yet
	}
}

void phdGenBase::draw(phdGenParams * _params) {
	draw( 0.0, 0.0, getWidth(), getHeight(), _params);
}

void phdGenBase::draw(float _x, float _y, float _w, float _h, phdGenParams * _params) {
	ofSetColor(ofColor::white, 255);
	ofLine(_x, _y, _w, _h);
	ofLine(_x, _h, _w, _y);
}

