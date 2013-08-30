#include "phdClipTrack.h"

//--------------------------------------------------------------------------------------------------
phdClipTrack::phdClipTrack() {
	setup();
}

void phdClipTrack::fadeIn() {
	if(oldClip != NULL) oldClip->fader.fadeOut();
	if(curClip != NULL) curClip->fader.fadeIn();
	fader.fadeIn();
}

void phdClipTrack::fadeOut() {
	if(oldClip != NULL) oldClip->fader.fadeOut();
	if(curClip != NULL) curClip->fader.fadeOut();
	fader.fadeOut();
}

void phdClipTrack::play() {
	if(oldClip != NULL) oldClip->play();
	if(curClip != NULL) curClip->play();
}

void phdClipTrack::pause() {
	if(oldClip != NULL) oldClip->pause();
	if(curClip != NULL) curClip->pause();
}

void phdClipTrack::stop() {
	if(oldClip != NULL) oldClip->stop();
	if(curClip != NULL) curClip->stop();
}

void phdClipTrack::setup() {
	allocated = false;
	curClip = oldClip = NULL;
	blend = NULL;
	setWidth(PHDDEFAULTWIDTH);
	setHeight(PHDDEFAULTHEIGHT);
	setDrawArea(0.0, 0.0, PHDDEFAULTWIDTH, PHDDEFAULTHEIGHT);
	setDrawAngle(0.0);
	setActive(true);
}

void phdClipTrack::update() {

	// update every active fader
	for(int i = 0; i < clips.size(); i++) {
		if(clips[i] != curClip && clips[i] != oldClip && clips[i]->fader.getAction() != pfaNone) {
			clips[i]->update();
		}
	}

	fader.update();
	timer.update();

	if(!getActive()) return;

	// updates oldClip canvas
	if(oldClip != NULL && oldClip->fader.getValue() > 0.0) { oldClip->update(); }

	// updates curClip canvas
	if(curClip != NULL) { curClip->update(); }

	bool drawOld = (
		oldClip != NULL && oldClip->generator != NULL &&
		oldClip->getActive() &&
		oldClip->fader.getValue() != 0.0);

	bool drawCur = (
		curClip != NULL && curClip->generator != NULL &&
		curClip->getActive() &&
		curClip->fader.getValue() != 0.0);

	if(drawOld && !drawCur && curClip != NULL && oldClip != NULL) { // oldClip

		getCanvas().begin();
			if(blend != NULL && PHDUSEBLENDSHADER == 1) {
				if(getCanvas().isAllocated() && oldClip->getCanvas().isAllocated() && curClip->getCanvas().isAllocated()) {
					ofEnableAlphaBlending();
					ofClear(0,0,0,0);
					blend->draw(2, // 3, 6, 7, 5, 2, 8, 16, 18, 19, 20, 22
					oldClip->getCanvas().getTextureReference(), oldClip->fader.getValue(),
					curClip->getCanvas().getTextureReference(), curClip->fader.getValue(),
					getCanvas().getWidth(), getCanvas().getHeight());
				}
			} else {
				ofEnableAlphaBlending();
				ofClear(0, 0, 0, 0);
				ofSetColor(ofColor::white, (int)(255.0 * oldClip->fader.getValue()));

				ofPushMatrix();

				ofTranslate(getDrawX() + getDrawW() / 2.0, getDrawY() + getDrawH() / 2.0);
				ofRotateZ(getDrawAngle());

				oldClip->draw(getDrawX() - getDrawW() / 2.0, getDrawY() - getDrawH() / 2.0, getDrawW(), getDrawH());

				ofPopMatrix();
			}
		getCanvas().end();

	} else if(!drawOld && drawCur && curClip != NULL && oldClip != NULL) { // curClip

		getCanvas().begin();
			if(blend != NULL && PHDUSEBLENDSHADER == 1) {
				if(getCanvas().isAllocated() && oldClip->getCanvas().isAllocated() && curClip->getCanvas().isAllocated()) {
					ofEnableAlphaBlending();
					ofClear(0,0,0,0);
					blend->draw(2, // 3, 6, 7, 5, 2, 8, 16, 18, 19, 20, 22
					oldClip->getCanvas().getTextureReference(), oldClip->fader.getValue(),
					curClip->getCanvas().getTextureReference(), curClip->fader.getValue(),
					getCanvas().getWidth(), getCanvas().getHeight());
				}
			} else {
				ofEnableAlphaBlending();
				ofClear(0, 0, 0, 0);
				ofSetColor(ofColor::white, (int)(255.0 * curClip->fader.getValue()));

				ofPushMatrix();

				ofTranslate(getDrawX() + getDrawW() / 2.0, getDrawY() + getDrawH() / 2.0);
				ofRotateZ(getDrawAngle());

				curClip->draw(getDrawX() - getDrawW() / 2.0, getDrawY() - getDrawH() / 2.0, getDrawW(), getDrawH());

				ofPopMatrix();
			}
		getCanvas().end();

	} else if(drawOld && drawCur && curClip != NULL && oldClip != NULL) { // mixes oldClip and curClip

		getCanvas().begin();

			if(blend != NULL && PHDUSEBLENDSHADER == 1) {
				if(getCanvas().isAllocated() && oldClip->getCanvas().isAllocated() && curClip->getCanvas().isAllocated()) {
					ofEnableAlphaBlending();
					ofClear(0,0,0,0);
					blend->draw(2, // 3, 6, 7, 5, 2, 8, 16, 18, 19, 20, 22
					oldClip->getCanvas().getTextureReference(), oldClip->fader.getValue(),
					curClip->getCanvas().getTextureReference(), curClip->fader.getValue(),
					getCanvas().getWidth(), getCanvas().getHeight());
				}
			} else {

				glEnable (GL_BLEND);  
				glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD); 

				ofClear(0, 0, 0, 0);

				ofPushMatrix();

				ofTranslate(getDrawX() + getDrawW() / 2.0, getDrawY() + getDrawH() / 2.0);
				ofRotateZ(getDrawAngle());

				ofSetColor(ofColor::white, (int)(255.0 * oldClip->fader.getValue()));
				oldClip->draw(getDrawX() - getDrawW() / 2.0, getDrawY() - getDrawH() / 2.0, getDrawW(), getDrawH());

				ofSetColor(ofColor::white, (int)(255.0 * curClip->fader.getValue()));
				curClip->draw(getDrawX() - getDrawW() / 2.0, getDrawY() - getDrawH() / 2.0, getDrawW(), getDrawH());

				ofPopMatrix();

				glDisable(GL_BLEND);
			}
		getCanvas().end();

	} else if(curClip != NULL) {

		getCanvas().begin();
			ofClear(0);
			glEnable (GL_BLEND);  
			glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD); 
			ofPushMatrix();
			ofTranslate(getDrawX() + getDrawW() / 2.0, getDrawY() + getDrawH() / 2.0);
			ofRotateZ(getDrawAngle());
			ofSetColor(ofColor::white, (int)(255.0 * curClip->fader.getValue()));
			curClip->draw(getDrawX() - getDrawW() / 2.0, getDrawY() - getDrawH() / 2.0, getDrawW(), getDrawH());
			ofPopMatrix();
		getCanvas().end();

	}
	
	// apply post processing filters
	postProcessingFilter();
}

void phdClipTrack::draw(float _x, float _y, float _w, float _h) {

//	getCanvas().draw( _x, _y, _w, _h);

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

// ----------- class specific methods ---------------

void phdClipTrack::setCurClip(int _index) {
	
	if(_index > -1 && _index < clips.size()) {
		setCurClip(clips[_index]);
	} else {
		setCurClip(NULL);
	}
}

void phdClipTrack::setCurClip(phdClip * _clip) {
	if(_clip != curClip) {
		oldClip = curClip;
		curClip = _clip;

		if(oldClip != NULL) {
			oldClip->fadeOut();
		}

		if(curClip != NULL) {
			curClip->fadeIn();
			//fader.setValue(1.0 - fader.getValue());
			fader.fadeIn();
		} else {
			fader.fadeOut();
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
phdTrackControl::phdTrackControl() {
	//setClipTrack(_track);

	setPopupMenu(NULL);
	setClipsPopupMenu(NULL);
	setFilterManager(NULL);

	inputLinks.addLink();
	inputLinks.addLink();

	outputLinks.addLink();
}

phdTrackControl::~phdTrackControl() {
	setPopupMenu(NULL);
	setClipsPopupMenu(NULL);
	setFilterManager(NULL);
}

void phdTrackControl::updateHotspotsArea() {

	hotSlider.setup(getX(), getY(), -20, getHeight());
	hotTitleBar.setup(getX(), getY(), getWidth(), -15);

	float _x = getX() + getWidth();
	float _y = getY() - 15.0;

	commands.clear();
	commands.push_back(phdHotspotArea(_x, _y, 15, 15)); _x += 15; // B - force save background
	commands.push_back(phdHotspotArea(_x, _y, 15, 15)); _x += 15; // : - resize to 160x120
	commands.push_back(phdHotspotArea(_x, _y, 15, 15)); _x += 15; // ! - resize to 320x240
	commands.push_back(phdHotspotArea(_x, _y, 15, 15)); _x += 15; // % - resize to 512x384

	_x = getX() - hotSlider.w;
	_y = getY();

	loopSpots.clear();
	loopSpots.push_back(phdHotspotArea(_x+20, _y+getHeight()+2, 15, 15));
	loopSpots.push_back(phdHotspotArea(_x+35, _y+getHeight()+2, 15, 15));
	loopSpots.push_back(phdHotspotArea(_x+50, _y+getHeight()+2, 15, 15));

	hotspots.clear();
	for(int i = 0; i < clipTrack.clips.size(); i++) {
		hotspots.push_back(phdHotspotArea());
		hotspots[hotspots.size()-1].setup( _x, _y, -54, 20);
		_y += 20;
		if(_y > (getY() + getHeight() - 20)) {
			_y = getY();
			_x -= 54;
		}
	}

	if(filterManager != NULL) {
		filterHotspots.clear();
		_x = getX() + getWidth();
		_y = getY();
		for(int i = 0; i < filterManager->size(); i++) {
			filterHotspots.push_back(phdFilterHotspotArea());
			filterHotspots[filterHotspots.size()-1].hotspot.setup( _x, _y, 20, 20);
			filterHotspots[filterHotspots.size()-1].isActive = false;
			filterHotspots[filterHotspots.size()-1].order = -1;
			_y += 20;
			if(_y > (getY() + getHeight() - 20)) {
				_y = getY();
				_x += 20;
			}
		}
		// clip is selected?
		for(int i = 0; i < clipTrack.filters.size(); i++) {
			int _index = filterManager->indexOf(clipTrack.filters[i]);
			if(_index != -1) {
				filterHotspots[_index].isActive = true;
				filterHotspots[_index].order = i;
			}
		}
	}
}

void phdTrackControl::setPosition(float _x, float _y) {
	phdControlBase::setPosition(_x,_y);
	updateHotspotsArea();
}

void phdTrackControl::setDimension(float _width, float _height) {
	phdControlBase::setDimension(_width,_height);
	updateHotspotsArea();
}
/*
void phdTrackControl::setClipTrack(phdClipTrack * _track) {
	if(_track != track) {
		track = _track;
		updateHotspotsArea();
	}
}
*/
void phdTrackControl::update() {

	if(clipTrack.getCurClip() != NULL) {
		clipTrack.getCurClip()->params.mouse[0] = hitTestInfo.paramX * clipTrack.getCurClip()->getWidth();
		clipTrack.getCurClip()->params.mouse[1] = hitTestInfo.paramY * clipTrack.getCurClip()->getHeight();
		clipTrack.getCurClip()->params.resolution[0] = clipTrack.getCurClip()->getWidth();
		clipTrack.getCurClip()->params.resolution[1] = clipTrack.getCurClip()->getHeight();
	}

	clipTrack.update();
}

void phdTrackControl::draw() {
	ofPushStyle();

	ofFill();
	ofSetColor(192,120,64,128);
	if(selected) {
		drawFilledBorderRectangle(hotTitleBar.x, hotTitleBar.y, hotTitleBar.w, hotTitleBar.h, ofColor(120,192,255,200), ofColor(255,255,255,255));
	} else {
		drawFilledBorderRectangle(hotTitleBar.x, hotTitleBar.y, hotTitleBar.w, hotTitleBar.h, ofColor(160,92,220,200), ofColor(255,255,255,255));
	}
	ofSetColor(255,255,255,255);
	ofDrawBitmapString("track:" + name, hotTitleBar.x+4, hotTitleBar.y+12);


	for(int i = 0; i < commands.size(); i++) {
		if(selected) {
			drawFilledBorderRectangle(commands[i].x, commands[i].y, commands[i].w, commands[i].h, ofColor(120,192,255,200), ofColor(255,255,255,255));
		} else {
			drawFilledBorderRectangle(commands[i].x, commands[i].y, commands[i].w, commands[i].h, ofColor(160,92,220,200), ofColor(255,255,255,255));
		}
		ofSetColor(255,255,255,255);
		string _labels = "B:!%";
		ofDrawBitmapString(_labels.substr(i % _labels.length(), 1), commands[i].x + 4, commands[i].y + 11);
	}

	ofNoFill();
	ofSetColor(192,255,192,128);

	ofRect(getX(), getY(), getWidth(), getHeight());

	ofRect(hotSlider.x, hotSlider.y, hotSlider.w, hotSlider.h);
	ofRect(hotSlider.x, hotSlider.y + ((hotSlider.h-5.0) * (1.0-clipTrack.fader.getValue())), hotSlider.w, 5.0);

	if(hitTestInfo.clipIndex > -1 && hitTestInfo.clipIndex < hotspots.size()) {
		ofSetColor(255,128,64,255);
		ofRect(hotspots[hitTestInfo.clipIndex].x+1, hotspots[hitTestInfo.clipIndex].y+1, hotspots[hitTestInfo.clipIndex].w-2, hotspots[hitTestInfo.clipIndex].h-2);
		ofSetColor(192,255,192,128);
	}

	// clip hotspots
	for(int i = 0; i < hotspots.size(); i++) {

		if(i < clipTrack.clips.size() && clipTrack.clips[i] != NULL && clipTrack.clips[i] == clipTrack.getCurClip()) {
			ofSetColor(255,128,64,255);
			ofRect(hotspots[i].x+1, hotspots[i].y+1, hotspots[i].w-2, hotspots[i].h-2);
			ofSetColor(192,255,192,128);
		}
		ofRect(hotspots[i].x, hotspots[i].y, hotspots[i].w, hotspots[i].h);
		ofSetColor(164, 255, 200, 255);
		string _labels = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		ofDrawBitmapString(_labels.substr(i % _labels.length(), 1) + ":" + ofToString(clipTrack.clips[i]->fader.getValue()*10.0,1), hotspots[i].x+3, hotspots[i].y+14);
	}

	if(clipTrack.getCurClip() != NULL) clipTrack.getCurClip()->draw(getX()+2, getY()+2, getWidth()-4, getHeight()-4);

	// filter hotspots
	if(filterManager != NULL) {
		for(int i = 0; i < filterHotspots.size(); i++) {
			if(i < filterManager->size()) {
				if(filterHotspots[i].isActive || hitTestInfo.filterIndex == i) {
					ofSetColor(255,128,64,255);
				} else {
					ofSetColor(192,255,192,128);
				}
				ofRect(filterHotspots[i].hotspot.x+1, filterHotspots[i].hotspot.y+1, filterHotspots[i].hotspot.w-2, filterHotspots[i].hotspot.h-2);
				if(filterHotspots[i].order != -1) {
					ofSetColor(164, 255, 200, 255);
					ofDrawBitmapString(ofToString(filterHotspots[i].order), filterHotspots[i].hotspot.x+3, filterHotspots[i].hotspot.y+14);
				}
			}
		}
	}

	// draw mouse cross
	if(clipTrack.getCurClip() != NULL) {

		float _px = hitTestInfo.paramX;
		float _py = hitTestInfo.paramY;

		if(_px >= 0.0 && _px <= 1.0 && _py >= 0.0 && _py <= 1.0) {

			_px = _px * getWidth() + getX();
			_py = _py * getHeight() + getY();

			ofSetColor(255,255,255,255);
			ofLine(_px-5.0, _py, _px+5.0, _py);
			ofLine(_px, _py-5.0, _px, _py+5.0);
			ofCircle(_px, _py, 5.0);
		}
	}

	inputLinks.draw(getX()-4, getY()-(float)(linkSize*0.75));
	outputLinks.draw(getX()-4, getY()+getHeight()+(float)(linkSize*0.75));

	if(dragging) {
		ofLine(hitTestInfo.mouseX, hitTestInfo.mouseY, hitTestInfo.mouseDownX, hitTestInfo.mouseDownY);
	}

	if(clipTrack.getCurClip() != NULL && clipTrack.getCurClip()->getGenerator() != NULL) {
		phdGenerator * _generator = clipTrack.getCurClip()->getGenerator();
		phdWarper * _warper = _generator->getWarper();
		if(_warper != NULL) {
			double _x0, _y0, _x1, _y1;
			for(int i = 0; i < 3; i++) {
				_warper->getSource(i+0, _x0, _y0);
				_warper->getSource(i+1, _x1, _y1);

				_x0 = _x0 / _generator->getWidth() * getWidth();
				_y0 = _y0 / _generator->getHeight() * getHeight();
				_x1 = _x1 / _generator->getWidth() * getWidth();
				_y1 = _y1 / _generator->getHeight() * getHeight();

				ofLine(getX() + _x0, getY() + _y0, getX() + _x1, getY() + _y1);
			}
			_warper->getSource(0, _x0, _y0);
			_warper->getSource(3, _x1, _y1);
			_x0 = _x0 / _generator->getWidth() * getWidth();
			_y0 = _y0 / _generator->getHeight() * getHeight();
			_x1 = _x1 / _generator->getWidth() * getWidth();
			_y1 = _y1 / _generator->getHeight() * getHeight();

			ofLine(getX() + _x0, getY() + _y0, getX() + _x1, getY() + _y1);
		}
	}

	if(clipTrack.getCurClip() != NULL && clipTrack.getCurClip()->getGenerator() != NULL && loopSpots.size() == 3) {
		string _loopState = clipTrack.getCurClip()->getGenerator()->getProperty("LOOPSTATE");
		if(_loopState == "NORMAL") {
			drawFilledBorderRectangle(loopSpots[0].x, loopSpots[0].y, loopSpots[0].w, loopSpots[0].h, ofColor(120,192,255,200), ofColor(255,255,255,255));
			drawFilledBorderRectangle(loopSpots[1].x, loopSpots[1].y, loopSpots[1].w, loopSpots[1].h, ofColor(160,92,220,200), ofColor(255,255,255,255));
			drawFilledBorderRectangle(loopSpots[2].x, loopSpots[2].y, loopSpots[2].w, loopSpots[2].h, ofColor(160,92,220,200), ofColor(255,255,255,255));
		} else if(_loopState == "PALINDROME") {
			drawFilledBorderRectangle(loopSpots[0].x, loopSpots[0].y, loopSpots[0].w, loopSpots[0].h, ofColor(160,92,220,200), ofColor(255,255,255,255));
			drawFilledBorderRectangle(loopSpots[1].x, loopSpots[1].y, loopSpots[1].w, loopSpots[1].h, ofColor(120,192,255,200), ofColor(255,255,255,255));
			drawFilledBorderRectangle(loopSpots[2].x, loopSpots[2].y, loopSpots[2].w, loopSpots[2].h, ofColor(160,92,220,200), ofColor(255,255,255,255));
		} else if(_loopState == "NONE") {
			drawFilledBorderRectangle(loopSpots[0].x, loopSpots[0].y, loopSpots[0].w, loopSpots[0].h, ofColor(160,92,220,200), ofColor(255,255,255,255));
			drawFilledBorderRectangle(loopSpots[1].x, loopSpots[1].y, loopSpots[1].w, loopSpots[1].h, ofColor(160,92,220,200), ofColor(255,255,255,255));
			drawFilledBorderRectangle(loopSpots[2].x, loopSpots[2].y, loopSpots[2].w, loopSpots[2].h, ofColor(120,192,255,200), ofColor(255,255,255,255));
		}
		if(_loopState == "NORMAL" || _loopState == "PALINDROME" || _loopState == "NONE") {
			ofSetColor(164, 255, 200, 255);
			ofDrawBitmapString("L", loopSpots[0].x + 4, loopSpots[0].y + 12);
			ofDrawBitmapString("P", loopSpots[1].x + 4, loopSpots[1].y + 12);
			ofDrawBitmapString("N", loopSpots[2].x + 4, loopSpots[2].y + 12);
		}
	}

	ofPopStyle();
}

bool phdTrackControl::updateHitTestInfo(float _x, float _y) {

	hitTestInfo.infoType = httOutside;
	hitTestInfo.mouseX = _x;
	hitTestInfo.mouseY = _y;
	hitTestInfo.clipIndex = -1;
	hitTestInfo.filterIndex = -1;
	hitTestInfo.focusedClip = NULL;
	hitTestInfo.linkIndex = -1;
	hitTestInfo.popupIndex = -1;
	hitTestInfo.commandIndex = -1;
	hitTestInfo.loopIndex = -1;

	// popup menu
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		if(getPopupMenu()->updateHitTestInfo(_x, _y)) {
			hitTestInfo.infoType = httPopup;
			hitTestInfo.popupIndex = 0;
			return true;
		}
	}

	// clip popup menu
	if(getClipsPopupMenu() != NULL && getClipsPopupMenu()->getVisible()) {
		if(getClipsPopupMenu()->updateHitTestInfo(_x, _y)) {
			hitTestInfo.infoType = httPopup;
			hitTestInfo.popupIndex = 1;
			return true;
		}
	}

	if(_x >= getX() && _y >= getY() && _x <= getX() + getWidth() && _y <= getY() + getHeight()) {
		hitTestInfo.infoType = httOutput;
	} else if(hotSlider.pointInside(_x, _y)) {
		hitTestInfo.infoType = httSlider;
	} else if(inputLinks.updateHitTestInfo(_x, _y)) {
		hitTestInfo.infoType = httInputLink;
		hitTestInfo.linkIndex = inputLinks.focusedIndex;
	} else if(outputLinks.updateHitTestInfo(_x, _y)) {
		hitTestInfo.infoType =  httOutputLink;
		hitTestInfo.linkIndex = outputLinks.focusedIndex;
	} else if(hotTitleBar.pointInside(_x, _y)) {
		hitTestInfo.infoType = httTitleBar;
	} else {

		for(int i = 0; i < commands.size(); i++) {
			if(commands[i].pointInside(_x, _y)) {
				hitTestInfo.infoType = httCommand;
				hitTestInfo.commandIndex = i;
				return true;
			}
		}

		for(int i = 0; i < hotspots.size(); i++) {
			if(hotspots[i].pointInside(_x, _y)) {
				hitTestInfo.infoType = httClip;
				hitTestInfo.clipIndex = i;
				if(i > -1 && i < clipTrack.clips.size()) hitTestInfo.focusedClip = clipTrack.clips[i];
				return true;
				break;
			}
		}

		for(int i = 0; i < loopSpots.size(); i++) {
			if(loopSpots[i].pointInside(_x, _y)) {
				hitTestInfo.infoType = httLoopMode;
				hitTestInfo.loopIndex = i;
				return true;
			}
		}

		if(filterManager != NULL && filterManager->size() == filterHotspots.size()) {
			for(int i = 0; i < filterManager->size(); i++) {
				if(filterHotspots[i].hotspot.pointInside(_x, _y)) {
					hitTestInfo.infoType = httFilter;
					hitTestInfo.filterIndex = i;
					return true;
					break;
				}
			}
		}
	}

	return (hitTestInfo.infoType != httOutside);
}

void phdTrackControl::onMouseMoved(ofMouseEventArgs& args) {
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMouseMoved(args);
		return;
	}

	if(getClipsPopupMenu() != NULL && getClipsPopupMenu()->getVisible()) {
		getClipsPopupMenu()->onMouseMoved(args);
		return;
	}

	if(!hitTestInfo.dragging) updateHitTestInfo(args.x, args.y);

	if(hitTestInfo.infoType == httOutput) {
	} else if(hitTestInfo.infoType == httClip) {
	} else if(hitTestInfo.infoType == httFilter) {
	} else if(hitTestInfo.infoType == httCommand) {
	} else if(hitTestInfo.infoType == httSlider) {
	} else if(hitTestInfo.infoType == httInputLink) {
	} else if(hitTestInfo.infoType == httOutputLink) {
	} else if(hitTestInfo.infoType == httTitleBar) {
	} else if(hitTestInfo.infoType == httLoopMode) {
	}
}

void phdTrackControl::onMousePressed(ofMouseEventArgs& args) {
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMousePressed(args);
		return;
	}

	if(getClipsPopupMenu() != NULL && getClipsPopupMenu()->getVisible()) {
		getClipsPopupMenu()->onMousePressed(args);
		return;
	}

	hitTestInfo.mouseDownX = args.x;
	hitTestInfo.mouseDownY = args.y;

	if(hitTestInfo.infoType == httOutput) {
	} else if(hitTestInfo.infoType == httClip) {
	} else if(hitTestInfo.infoType == httFilter) {
	} else if(hitTestInfo.infoType == httCommand) {
	} else if(hitTestInfo.infoType == httSlider) {
	} else if(hitTestInfo.infoType == httInputLink) {
	} else if(hitTestInfo.infoType == httOutputLink) {
		dragging = true;
	} else if(hitTestInfo.infoType == httTitleBar) {
	} else if(hitTestInfo.infoType == httLoopMode) {
	}
}

void phdTrackControl::onMouseDragged(ofMouseEventArgs& args) {
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMouseDragged(args);
		return;
	}

	if(getClipsPopupMenu() != NULL && getClipsPopupMenu()->getVisible()) {
		getClipsPopupMenu()->onMouseDragged(args);
		return;
	}

	if(hitTestInfo.infoType == httOutput) {
		if(args.button == 0) {

			hitTestInfo.paramX = ofMap(args.x, getX(), getX() + getWidth(), 0.0, 1.0);
			hitTestInfo.paramY = ofMap(args.y, getY(), getY() + getHeight(), 0.0, 1.0);

			if(clipTrack.getCurClip() != NULL && clipTrack.getCurClip()->getGenerator() != NULL) {
				phdGenerator * _generator = clipTrack.getCurClip()->getGenerator();
				phdWarper * _warper = _generator->getWarper();
				if(_warper != NULL) {
					int _pt = _warper->findNearestSource(hitTestInfo.paramX * _generator->getWidth(), hitTestInfo.paramY * _generator->getHeight());
					if(_pt != -1) {
						double _x = hitTestInfo.paramX * _generator->getWidth();
						double _y = hitTestInfo.paramY * _generator->getHeight();

						if(_x >= 0.0 && _x <= _generator->getWidth() &&	_y >= 0.0 && _y <= _generator->getHeight()) {
							_warper->setSource(_pt, _x, _y);
						}
					}
				}
			}
		}
	} else if(hitTestInfo.infoType == httClip) {
	} else if(hitTestInfo.infoType == httFilter) {
	} else if(hitTestInfo.infoType == httCommand) {
	} else if(hitTestInfo.infoType == httSlider) {
	} else if(hitTestInfo.infoType == httInputLink) {
	} else if(hitTestInfo.infoType == httOutputLink && hitTestInfo.linkIndex != -1) {
	} else if(hitTestInfo.infoType == httTitleBar) {

		setPosition(getX() + args.x - hitTestInfo.mouseX, getY() + args.y - hitTestInfo.mouseY);

		hitTestInfo.mouseX = args.x;
		hitTestInfo.mouseY = args.y;

	} else if(hitTestInfo.infoType == httLoopMode) {
	}

	hitTestInfo.mouseX = args.x;
	hitTestInfo.mouseY = args.y;
}

void phdTrackControl::onMouseReleased(ofMouseEventArgs& args) {

	dragging = false;

	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMouseReleased(args);
		return;
	}

	if(getClipsPopupMenu() != NULL && getClipsPopupMenu()->getVisible()) {
		getClipsPopupMenu()->onMouseReleased(args);
		return;
	}

	if(hitTestInfo.infoType == httOutput) {

	} else if(hitTestInfo.infoType == httClip) {
		if(args.button == 0) {
			if(hitTestInfo.clipIndex > -1 && hitTestInfo.clipIndex < clipTrack.clips.size()) {
				clipTrack.setCurClip(hitTestInfo.clipIndex);
				if(clipTrack.getCurClip() != NULL) {
					hitTestInfo.paramX = clipTrack.getCurClip()->params.mouse[0] / clipTrack.getCurClip()->getWidth();
					hitTestInfo.paramY = clipTrack.getCurClip()->params.mouse[1] / clipTrack.getCurClip()->getHeight();
				}
			}
		} else if(getClipsPopupMenu() != NULL && args.button == 2 && hitTestInfo.focusedClip != NULL) {
			getClipsPopupMenu()->setOwner(this);
			getClipsPopupMenu()->popup(args.x, args.y, (void*)hitTestInfo.focusedClip, "CLIP");
			//ofAddListener(getClipsPopupMenu()->popEvent, this, &phdTrackControl::onClipsPopupClick);
		}
	} else if(hitTestInfo.infoType == httFilter) {

		if(args.button == 0) {
			if(filterManager != NULL && filterHotspots.size() == filterManager->size() &&
				hitTestInfo.filterIndex > -1 && hitTestInfo.filterIndex < filterManager->size()) {

				if(filterHotspots[hitTestInfo.filterIndex].isActive) {
					clipTrack.filters.popItem((*filterManager)[hitTestInfo.filterIndex]); // retira da lista
				} else {
					clipTrack.filters.pushItem((*filterManager)[hitTestInfo.filterIndex]); // insere na lista
				}

				filterHotspots[hitTestInfo.filterIndex].isActive = !filterHotspots[hitTestInfo.filterIndex].isActive;

				updateHotspotsArea();
			}
		}

	} else if(hitTestInfo.infoType == httCommand) {

		if(args.button == 0) {
			if(hitTestInfo.commandIndex == 0) { // B
				if(clipTrack.getCurClip() != NULL) clipTrack.getCurClip()->params.updateBackground = true;
			} else if(hitTestInfo.commandIndex == 1) { // :
				setDimension(160,120);
				updateHotspotsArea();
			} else if(hitTestInfo.commandIndex == 2) { // !
				setDimension(320,240);
				updateHotspotsArea();
			} else if(hitTestInfo.commandIndex == 3) { // %
				setDimension(512,384);
				updateHotspotsArea();
			}
		}

	} else if(hitTestInfo.infoType == httSlider) {

	} else if(hitTestInfo.infoType == httInputLink) {

		if(args.button == 0 && hitTestInfo.linkIndex == 0) {

			if(clipTrack.getCurClip() != NULL) clipTrack.getCurClip()->params.updateBackground = true;

		}

	} else if(hitTestInfo.infoType == httOutputLink && hitTestInfo.linkIndex != -1) {
		phdControlManager * _manager = getManager();
		if(_manager != NULL) {
			for(int i = 0; i < _manager->size(); i++) {
				phdControlBase * _control = (*_manager)[i];
				if(_control != NULL && _control->getClassName() == "PHDDESIGNER") {
					phdDesigner * _designer = (phdDesigner*) _control;
					if(_designer->inputLinks.updateHitTestInfo(args.x, args.y)) {
						_designer->inputLinks.linkIndexedToRemoteLink(
							_designer->inputLinks.focusedIndex,
							outputLinks[hitTestInfo.linkIndex]
						);
						_designer->setInputClip(getClipTrack());
					} else {
					}
				}
			}
		}
	} else if(hitTestInfo.infoType == httTitleBar) {

		if(args.button == 0) {

		} else if(getPopupMenu() != NULL && args.button == 2) {
			getPopupMenu()->setOwner(this);
			getPopupMenu()->popup(args.x, args.y, (void*)this, "TRACK");
			//ofAddListener(getPopupMenu()->popEvent, this, &phdTrackControl::onPopupClick);
		}

	} else if(hitTestInfo.infoType == httLoopMode) {

		if(args.button == 0) {
			if(clipTrack.getCurClip() != NULL && clipTrack.getCurClip()->getGenerator() != NULL) {
				if(hitTestInfo.loopIndex == 0) {
					clipTrack.getCurClip()->getGenerator()->setSetupString("LOOPSTATE|NORMAL");
				} else if(hitTestInfo.loopIndex == 1) {
					clipTrack.getCurClip()->getGenerator()->setSetupString("LOOPSTATE|PALINDROME");
				} else if(hitTestInfo.loopIndex == 2) {
					clipTrack.getCurClip()->getGenerator()->setSetupString("LOOPSTATE|NONE");
				}
			}
		}
	}
}

void phdTrackControl::onPopupClick(phdPopupMenuEventData &_eventData) {
	if(_eventData.menu != NULL) {
		ofRemoveListener(_eventData.menu->popEvent, this, &phdTrackControl::onPopupClick);
	}
}

void phdTrackControl::onClipsPopupClick(phdPopupMenuEventData &_eventData) {
	if(_eventData.menu != NULL) {
		ofRemoveListener(_eventData.menu->popEvent, this, &phdTrackControl::onClipsPopupClick);
	}
}

string phdTrackControl::getSetupString() {
	return "CLIP|" + getClassName() + "|" + getControlName() + "|" + ofToString(getX()) + "|" +
		ofToString(getY()) + "|" + ofToString(getWidth()) + "|" + ofToString(getHeight()) + "|";
}


void phdTrackControl::saveToStrings(vector<string> & _strings, string _title) {
	
	_strings.push_back(getSetupString() + "\n");

	for(int i = 0; i < clipTrack.clips.size(); i++) {
		if(clipTrack.clips[i] != NULL) {
			_strings.push_back(_title + "|" + clipTrack.clips[i]->generator->getSetupString() + "\n");
		}
	}
}
