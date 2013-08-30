#include "phdClipBase.h"

//--------------------------------------------------------------------------------------------------
phdClipBase::phdClipBase() {
	track = NULL;
	setVisual(NULL);
	setCropArea(0.0, 0.0, 1.0, 1.0);
	setDrawArea(0.0, 0.0, sizeX, sizeY);
	setIsVisual(true);
	setup();
}

phdClipBase::phdClipBase(phdGenBase * _visual = NULL, phdClipTrack * _track = NULL) {
	track = _track;
	setVisual(_visual);
	setCropArea(0.0, 0.0, 1.0, 1.0);
	setDrawArea(0.0, 0.0, sizeX, sizeY);
	setIsVisual(true);
	setup();
}

void phdClipBase::setVisual(phdGenBase * _visual) {
	if(_visual != visual) {

		visual = _visual;

		if(visual != NULL) {
			setSize(visual->getWidth(), visual->getHeight());
		} else {
			setSize(0.0, 0.0);
		}
	}
}

void phdClipBase::play() {
	if(visual != NULL) visual->play();
}

void phdClipBase::pause() {
	if(visual != NULL) visual->pause();
}

void phdClipBase::stop() {
	if(visual != NULL) visual->stop();
}

bool phdClipBase::getIsVisual() {
	return isVisual;
}

void phdClipBase::setIsVisual(bool _isVisual) {
	isVisual = _isVisual;
}

void phdClipBase::setSizeX(float _sizeX) {
	sizeX = _sizeX;
}

void phdClipBase::setSizeY(float _sizeY) {
	sizeY = _sizeY;
}

void phdClipBase::setSize(float _sizeX, float _sizeY) {
	setSizeX(_sizeX);
	setSizeY(_sizeY);
}

void phdClipBase::getSize(float & _sizeX, float & _sizeY) {
	_sizeX = sizeX;
	_sizeY = sizeY;
}

void phdClipBase::setDrawX(float _drawX) {
	drawX = _drawX;
}

void phdClipBase::setDrawY(float _drawY) {
	drawY = _drawY;
}

void phdClipBase::setDrawW(float _drawW) {
	drawW = _drawW;
}

void phdClipBase::setDrawH(float _drawH) {
	drawH = _drawH;
}

void phdClipBase::setDrawArea(float _drawX, float _drawY, float _drawW, float _drawH) {
	drawX = _drawX;
	drawY = _drawY;
	drawW = _drawW;
	drawH = _drawH;
}

void phdClipBase::getDrawArea(float & _drawX, float & _drawY, float & _drawW, float & _drawH) {
	_drawX = drawX;
	_drawY = drawY;
	_drawW = drawW;
	_drawH = drawH;
}

void phdClipBase::setCropX(float _cropX) {
	cropX = _cropX;
}

void phdClipBase::setCropY(float _cropY) {
	cropY = _cropY;
}

void phdClipBase::setCropW(float _cropW) {
	cropW = _cropW;
}

void phdClipBase::setCropH(float _cropH) {
	cropH = _cropH;
}

void phdClipBase::setCropArea(float _cropX, float _cropY, float _cropW, float _cropH) {
	cropX = ofClamp(_cropX, 0.0, 1.0);
	cropY = ofClamp(_cropY, 0.0, 1.0);
	cropW = ofClamp(_cropW, 0.0, 1.0);
	cropH = ofClamp(_cropH, 0.0, 1.0);
}

void phdClipBase::getCropArea(float & _cropX, float & _cropY, float & _cropW, float & _cropH) {
	_cropX = cropX;
	_cropY = cropY;
	_cropW = cropW;
	_cropH = cropH;
}

bool phdClipBase::isCropped() {
	return (cropX != 0.0 || cropY != 0 || cropW != 1.0 || cropH != 1.0);
}

void phdClipBase::setup() {

}

void phdClipBase::fadeIn() {
	fader.fadeIn();
}

void phdClipBase::fadeOut() {
	fader.fadeOut();
}

void phdClipBase::play() {
	if(visual != NULL) visual->play();
}

void phdClipBase::pause() {
	if(visual != NULL) visual->pause();
}

void phdClipBase::stop() {
	if(visual != NULL) visual->stop();
}

void phdClipBase::update() {

	fader.update();
	timer.update();

	if(visual != NULL) visual->update(&params);
}

void phdClipBase::draw(float _x, float _y, float _w, float _h) {
	if(visual != NULL) visual->draw(_x, _y, _w, _h, &params);
}

void phdClipBase::drawCroppedArea(float _x, float _y, float _w, float _h, float _cx, float _cy, float _cw, float _ch) {
	if(visual != NULL) visual->drawCroppedArea(_x, _y, _w, _h, _cx, _cy, _cw, _ch, &params);
}
