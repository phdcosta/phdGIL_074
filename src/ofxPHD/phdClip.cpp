#include "phdClip.h"

//--------------------------------------------------------------------------------------------------
void phdClip::allocateCanvas() {
	if(internalWidth > 0 && internalHeight > 0 &&
		(!internalCanvas.isAllocated() ||
		internalCanvas.getWidth() != getWidth() || internalCanvas.getHeight() != getHeight())) {
		internalCanvas.allocate(getWidth(), getHeight(), GL_RGBA);
		allocated = true;
	}
}

//--------------------------------------------------------------------------------------------------
phdClip::phdClip(phdGenerator * _generator) {
	setup();
	setGenerator(_generator);
	startedIn = startedOut = false;
	grabber = NULL;
}

void phdClip::setGenerator(phdGenerator * _generator) {
	if(_generator != generator) {
		if(generator != NULL && generator->isExclusive()) {
			generator->deallocate();
			delete generator; // delete exclusive generators
		}
		generator = _generator;
	}
}

phdGenerator * phdClip::getGenerator() {
	return generator;
}

void phdClip::setDrawArea(float _drawX, float _drawY, float _drawW, float _drawH) {
	setDrawX(_drawX);
	setDrawY(_drawY);
	setDrawW(_drawW);
	setDrawH(_drawH);
}

void phdClip::fadeIn() {
	fader.fadeIn();
	startedIn = true;
}

void phdClip::fadeOut() {
	fader.fadeOut();
	startedOut = (fader.getValue() > 0.0);
}

void phdClip::play() {
	if(generator != NULL) generator->play();
}

void phdClip::pause() {
	if(generator != NULL) generator->pause();
}

void phdClip::stop() {
	if(generator != NULL) generator->stop();
}

void phdClip::setup() {
	generator = NULL;
	allocated = false;
	setWidth(PHDDEFAULTWIDTH);
	setHeight(PHDDEFAULTHEIGHT);
	setDrawArea(0.0, 0.0, PHDDEFAULTWIDTH, PHDDEFAULTHEIGHT);
	setDrawAngle(0.0);
	setActive(true);
}

void phdClip::update() {

	fader.update();
	timer.update();

	if(!getActive()) return;

	params.time = ofGetElapsedTimef();
	params.faderPos = fader.getValue();
	params.grabber = grabber;
	params.clipW = getWidth();
	params.clipH = getHeight();

	if(generator != NULL) {

		if(startedIn && fader.getValue() > 0) {
			startedIn = false;
			play();
		}

		if(startedOut && fader.getValue() == 0) {
			startedOut = false;
			pause();
		}

		generator->update(&params);

		getCanvas().begin();
			ofEnableAlphaBlending();
			ofSetColor(255,255,255,255);
			ofClear(0,0,0,0);

			ofPushMatrix();

			ofTranslate(getDrawX() + getDrawW() / 2.0, getDrawY() + getDrawH() / 2.0);
			ofRotateZ(getDrawAngle());

			generator->draw(getDrawX() - getDrawW() / 2.0, getDrawY() - getDrawH() / 2.0, getDrawW(), getDrawH(), &params);

			ofPopMatrix();
		getCanvas().end();

		// apply post processing filters
		postProcessingFilter();
	}
}

void phdClip::postProcessingFilter() {
	if(getCanvas().isAllocated()) {
		params.canvas = &getCanvas();
		for(int i = 0; i < filters.size(); i++) {
			if(filters[i] != NULL) filters[i]->update(&params);
		}
	}
}

void phdClip::draw(float _x, float _y, float _w, float _h) {

	//getCanvas().draw(_x, _y, _w, _h);

	ofSetColor(ofColor::white);

	getCanvas().getTextureReference().bind();
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(_x, _y, 0);
			glTexCoord2f(getCanvas().getWidth(), 0);
			glVertex3f(_x+_w, _y, 0);
			glTexCoord2f(getCanvas().getWidth(), getCanvas().getHeight());
			glVertex3f(_x+_w, _y+_h, 0);
			glTexCoord2f(0, getCanvas().getHeight());
			glVertex3f(_x, _y+_h, 0);
		glEnd();
	getCanvas().getTextureReference().unbind();
}

//--------------------------------------------------------------------------------------------------------------
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

void phdClipList::delItem(phdClip * _item) {
	delItem(indexOf(_item));
}

void phdClipList::delItem(int _index) {
	if(_index > -1 && _index < items.size()) {
		delete items[_index];
		items.erase(items.begin() + _index);
	}
}
