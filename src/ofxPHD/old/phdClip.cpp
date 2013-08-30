#include "phdClip.h"

//--------------------------------------------------------------------------------------------------
phdClip::phdClip(phdGenBase * _visual) {
	setVisual(_visual);
	setCropArea(0.0, 0.0, 1.0, 1.0);
	setDrawArea(0.0, 0.0, sizeX, sizeY);
	setIsVisual(true);
	setup();
}

int phdClip::getTrackCount() {
	if(visual != NULL) return visual->getTrackCount();
	return 0;
}

void phdClip::setVisual(phdGenBase * _visual) {
	if(_visual != visual) {

		visual = _visual;

		if(visual != NULL) {
			setSize(visual->getWidth(), visual->getHeight());
		} else {
			setSize(0.0, 0.0);
		}
	}
}

bool phdClip::getIsVisual() {
	return isVisual;
}

void phdClip::setIsVisual(bool _isVisual) {
	isVisual = _isVisual;
}

void phdClip::setSizeX(float _sizeX) {
	sizeX = _sizeX;
}

void phdClip::setSizeY(float _sizeY) {
	sizeY = _sizeY;
}

void phdClip::setSize(float _sizeX, float _sizeY) {
	setSizeX(_sizeX);
	setSizeY(_sizeY);
}

void phdClip::getSize(float & _sizeX, float & _sizeY) {
	_sizeX = sizeX;
	_sizeY = sizeY;
}

void phdClip::setDrawX(float _drawX) {
	drawX = _drawX;
}

void phdClip::setDrawY(float _drawY) {
	drawY = _drawY;
}

void phdClip::setDrawW(float _drawW) {
	drawW = _drawW;
}

void phdClip::setDrawH(float _drawH) {
	drawH = _drawH;
}

void phdClip::setDrawArea(float _drawX, float _drawY, float _drawW, float _drawH) {
	drawX = _drawX;
	drawY = _drawY;
	drawW = _drawW;
	drawH = _drawH;
}

void phdClip::getDrawArea(float & _drawX, float & _drawY, float & _drawW, float & _drawH) {
	_drawX = drawX;
	_drawY = drawY;
	_drawW = drawW;
	_drawH = drawH;
}

void phdClip::setCropX(float _cropX) {
	cropX = _cropX;
}

void phdClip::setCropY(float _cropY) {
	cropY = _cropY;
}

void phdClip::setCropW(float _cropW) {
	cropW = _cropW;
}

void phdClip::setCropH(float _cropH) {
	cropH = _cropH;
}

void phdClip::setCropArea(float _cropX, float _cropY, float _cropW, float _cropH) {
	cropX = ofClamp(_cropX, 0.0, 1.0);
	cropY = ofClamp(_cropY, 0.0, 1.0);
	cropW = ofClamp(_cropW, 0.0, 1.0);
	cropH = ofClamp(_cropH, 0.0, 1.0);
}

void phdClip::getCropArea(float & _cropX, float & _cropY, float & _cropW, float & _cropH) {
	_cropX = cropX;
	_cropY = cropY;
	_cropW = cropW;
	_cropH = cropH;
}

bool phdClip::isCropped() {
	return (cropX != 0.0 || cropY != 0 || cropW != 1.0 || cropH != 1.0);
}

void phdClip::setup() {

}

void phdClip::fadeIn() {
	fader.fadeIn();
}

void phdClip::fadeOut() {
	fader.fadeOut();
}

void phdClip::play() {
	if(visual != NULL) visual->play();
}

void phdClip::pause() {
	if(visual != NULL) visual->pause();
}

void phdClip::stop() {
	if(visual != NULL) visual->stop();
}

void phdClip::update() {

	fader.update();
	timer.update();

	params.time = ofGetElapsedTimef();

	if(visual != NULL) visual->update(&params);
}

void phdClip::draw(float _x, float _y, float _w, float _h) {
	if(visual != NULL) visual->draw(_x, _y, _w, _h, &params);
}

void phdClip::drawCroppedArea(float _x, float _y, float _w, float _h, float _cx, float _cy, float _cw, float _ch) {
	//if(visual != NULL) visual->drawCroppedArea(_x, _y, _w, _h, _cx, _cy, _cw, _ch, &params);
}

//--------------------------------------------------------------------------------------------------------------
phdClipList::phdClipList() {
}

phdClipList::~phdClipList() {
	freeItems();
}

void phdClipList::freeItems() {
	for (vector<phdClip*>::iterator i = items.begin(); i != items.end(); i++) {
		delete(*i); (*i) = NULL;
	}
	items.clear();
}

void phdClipList::clear() {
	items.clear();
}

int phdClipList::indexOf(phdClip * _item) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i] == _item) return i;
	}
	return -1;
}

phdClip * phdClipList::addItem(phdClip * _item) {
	int _index = indexOf(_item);
	if(_index == -1) {
		items.push_back(_item);
		_item->parent = this;
		return _item;
	}
	return items[_index];
}
