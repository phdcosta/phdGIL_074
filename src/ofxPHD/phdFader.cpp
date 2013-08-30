#include "phdFader.h"

//--------------------------------------------------------------------------------------------------

phdFader::phdFader() {
	duration = 3.0;
	value = 0.0;
	action = pfaNone;
	bRecalculate = false;
}

void phdFader::fadeIn(bool _restart) {
	if(_restart) value = 0.0;
	action = pfaFadeIn;
	bRecalculate = true;
}

void phdFader::fadeOut(bool _restart) {
	if(_restart) value = 1.0;
	action = pfaFadeOut;
	bRecalculate = true;
}

void phdFader::pause() {
	action = pfaNone;
}

void phdFader::update() {
	
	if(action == pfaNone) return;

	float _now = ofGetElapsedTimef();

	if(bRecalculate) {

		bRecalculate = false;

		startTime = _now;

		if(action == pfaFadeIn) {
			endTime = _now + ((1.0 - value) * duration);
		} else if(action == pfaFadeOut) {
			endTime = _now + (value * duration);
		}
	}

	if(action == pfaFadeIn) {
		value = 1.0 - ((endTime - _now) / duration);
	} else if(action == pfaFadeOut) {
		value = ((endTime - _now) / duration);
	}

	if(value < 0.0) {
		value = 0.0;
		action = pfaNone;
	} else if(value > 1.0) {
		value = 1.0;
		action = pfaNone;
	}
}

