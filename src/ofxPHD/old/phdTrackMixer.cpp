#include "phdTrackMixer.h"

//--------------------------------------------------------------------------------------------------

phdTrackMixer::phdTrackMixer() {
	allocated = false;
	setup();
}

void phdTrackMixer::setWidth(float _width) {
	if(_width != getWidth()) {
		phdGenBase::setWidth(_width);
		allocateCanvas();
	}
}

void phdTrackMixer::setHeight(float _height) {
	if(_height != getHeight()) {
		phdGenBase::setHeight(_height);
		allocateCanvas();
	}
}

void phdTrackMixer::setup() {
	setWidth(512.0);
	setHeight(384.0);
}

void phdTrackMixer::play() {
	trackA.play();
	trackB.play();
}

void phdTrackMixer::pause() {
	trackA.pause();
	trackB.pause();
}

void phdTrackMixer::stop() {
	trackA.stop();
	trackB.stop();
}

void phdTrackMixer::update(phdGenParams * _params) {
	trackA.update();
	trackB.update();
}

void phdTrackMixer::draw(phdGenParams * _params) {

}

void phdTrackMixer::draw(float _x, float _y, float _w, float _h, phdGenParams * _params) {

}

// ----------- class specific methods ---------------

void phdTrackMixer::allocateCanvas() {

	if(getWidth() > 0 && getHeight() > 0 && (mixerCanvas.getWidth() != getWidth() || mixerCanvas.getHeight() != getHeight())) {

		mixerCanvas.allocate(getWidth(), getWidth(), GL_RGBA);
		allocated = true;
	}
}

void phdTrackMixer::fadeIn() {
	trackA.fadeOut();
	trackB.fadeIn();
}

void phdTrackMixer::fadeOut() {
	trackA.fadeOut();
	trackB.fadeOut();
}
