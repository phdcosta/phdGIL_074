#include "phdClipTrack.h"

//--------------------------------------------------------------------------------------------------
phdClipTrack::phdClipTrack() {
	phdGenBase::phdGenBase();
	setup();
}

void phdClipTrack::setWidth(float _width) {
	if(_width != getWidth()) {
		phdGenBase::setWidth(_width);
		allocateCanvas();
	}
}

void phdClipTrack::setHeight(float _height) {
	if(_height != getHeight()) {
		phdGenBase::setHeight(_height);
		allocateCanvas();
	}
}

void phdClipTrack::setup() {
	curClip = NULL;
	oldClip = NULL;
	setWidth(512.0);
	setHeight(384.0);
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

void phdClipTrack::update(phdGenParams * _params) {

	if(curClip != NULL) curClip->fader.update();
	if(oldClip != NULL) oldClip->fader.update();

	bool drawOld = (allocated && oldClip != NULL && oldClip->visual != NULL && oldClip->fader.getValue() != 0.0);
	bool drawCur = (allocated && curClip != NULL && curClip->visual != NULL && curClip->fader.getValue() != 0.0);

	// draw oldClip Visual
	if(drawOld) {
		if(oldClip->isCropped()) {

			float _x, _y, _w, _h;

			// draw on intermediate canvas 
			oldClip->getDrawArea(_x,_y,_w,_h);

			oldDrawCanvas.begin();
				ofEnableAlphaBlending();
				ofClear(0, 0, 0, 0);
			ofClear(0.4, 0.2, 0.7, 0.5);
				oldClip->visual->draw(_x, _y, _w, _h);
			oldDrawCanvas.end();
			
			// crop result
			oldClip->getCropArea(_x, _y, _w, _h);

			_x *= getWidth();
			_y *= getHeight();
			_w *= getWidth();
			_h *= getHeight();

			oldCropCanvas.begin();

				ofEnableAlphaBlending();
				ofClear(0, 0, 0, 0);
			ofClear(0.4, 0.2, 0.7, 0.5);
				
				oldDrawCanvas.getTextureReference().bind();
					glBegin(GL_QUADS);
						glTexCoord2f(     _x,      _y); glVertex2d(     _x,      _y);
						glTexCoord2f(_x + _w,      _y); glVertex2d(_x + _w,      _y);
						glTexCoord2f(_x + _w, _y + _h); glVertex2d(_x + _w, _y + _h);
						glTexCoord2f(     _x, _y + _h); glVertex2d(     _x, _y + _h);
					glEnd();
				oldDrawCanvas.getTextureReference().unbind();

			oldCropCanvas.end();

		} else {

			float _x, _y, _w, _h;

			// draw directly on cropped canvas
			oldClip->getDrawArea(_x,_y,_w,_h);

			oldCropCanvas.begin();
				ofEnableAlphaBlending();
				ofClear(0, 0, 0, 0);
			ofClear(0.4, 0.2, 0.7, 0.5);
				oldClip->visual->draw(_x, _y, _w, _h);
			oldCropCanvas.end();

		}
	} else {
		oldCropCanvas.begin();
			ofEnableAlphaBlending();
			ofClear(0, 0, 0, 0);
			ofClear(0.4, 0.2, 0.7, 0.5);
		oldCropCanvas.end();
	}

	// draw curClip Visual
	if(drawCur) {
		if(curClip->isCropped()) {

			float _x, _y, _w, _h;

			// draw on intermediate canvas 
			curClip->getDrawArea(_x,_y,_w,_h);

			curDrawCanvas.begin();
				ofEnableAlphaBlending();
				ofClear(0, 0, 0, 0);
			ofClear(0.4, 0.2, 0.7, 0.5);
				curClip->visual->draw(_x, _y, _w, _h);
			curDrawCanvas.end();
			
			// crop result
			curClip->getCropArea(_x, _y, _w, _h);

			_x *= getWidth();
			_y *= getHeight();
			_w *= getWidth();
			_h *= getHeight();

			curCropCanvas.begin();

				ofEnableAlphaBlending();
				ofClear(0, 0, 0, 0);
			ofClear(0.4, 0.2, 0.7, 0.5);
				
				curDrawCanvas.getTextureReference().bind();
					glBegin(GL_QUADS);
						glTexCoord2f(     _x,      _y); glVertex2d(     _x,      _y);
						glTexCoord2f(_x + _w,      _y); glVertex2d(_x + _w,      _y);
						glTexCoord2f(_x + _w, _y + _h); glVertex2d(_x + _w, _y + _h);
						glTexCoord2f(     _x, _y + _h); glVertex2d(     _x, _y + _h);
					glEnd();
				curDrawCanvas.getTextureReference().unbind();

			curCropCanvas.end();

		} else {

			float _x, _y, _w, _h;

			// draw directly on cropped canvas
			curClip->getDrawArea(_x,_y,_w,_h);

			curCropCanvas.begin();
				ofEnableAlphaBlending();
				ofClear(0, 0, 0, 0);
			ofClear(0.4, 0.2, 0.7, 0.5);
				curClip->visual->draw(_x, _y, _w, _h);
			curCropCanvas.end();

		}
	} else {
		curCropCanvas.begin();
			ofEnableAlphaBlending();
			ofClear(0, 0);
		curCropCanvas.end();
	}

	if(drawOld && !drawCur) {
		trackCanvas.begin();
			ofClear(0, 0, 0, 0); // mix with black background
			ofClear(0.4, 0.2, 0.7, 0.5);
			ofEnableAlphaBlending();
			ofSetColor(ofColor::white, (int)(255.0 * oldClip->fader.getValue()));
			oldCropCanvas.draw(0, 0);
		trackCanvas.end();
	}

	if(!drawOld && drawCur) {
		trackCanvas.begin();
			ofClear(0, 0, 0, 0); // mix with black background
			ofClear(0.4, 0.2, 0.7, 0.5);
			ofEnableAlphaBlending();
			ofSetColor(ofColor::white, (int)(255.0 * curClip->fader.getValue()));
			curCropCanvas.draw(0, 0);
		trackCanvas.end();
	}

	if(drawOld && drawCur) {
		trackCanvas.begin();
			ofClear(0, 0, 0, 0);  // mix with black background
			ofClear(0.4, 0.2, 0.7, 0.5);
			glEnable (GL_BLEND);  
			glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD); 

			ofSetColor(ofColor::white, (int)(255.0 * oldClip->fader.getValue()));
			oldCropCanvas.draw(0, 0);

			ofSetColor(ofColor::white, (int)(255.0 * curClip->fader.getValue()));
			curCropCanvas.draw(0, 0);

			glDisable(GL_BLEND);

			/*blendShader.draw(3, // 3, 6, 7, 5, 2, 8, 16, 18, 19, 20, 22
				oldCropCanvas.getTextureReference(), oldClip->fader.getValue(),
				curCropCanvas.getTextureReference(), curClip->fader.getValue(),
				trackCanvas.getWidth(), trackCanvas.getHeight());*/

		trackCanvas.end();
	}
}

void phdClipTrack::draw(phdGenParams * _params) {

}

void phdClipTrack::draw(float _x, float _y, float _w, float _h, phdGenParams * _params) {

	ofSetColor(255,255,255,255);
	trackCanvas.draw( _x, _y, _w, _h);

}

// ----------- class specific methods ---------------

void phdClipTrack::allocateCanvas() {
	if(getWidth() > 0 && getHeight() > 0 && (trackCanvas.getWidth() != getWidth() || trackCanvas.getHeight() != getHeight())) {
		curDrawCanvas.allocate(getWidth(), getHeight(), GL_RGBA);
		curCropCanvas.allocate(getWidth(), getHeight(), GL_RGBA);
		oldDrawCanvas.allocate(getWidth(), getHeight(), GL_RGBA);
		oldCropCanvas.allocate(getWidth(), getHeight(), GL_RGBA);

		trackCanvas.allocate(getWidth(), getWidth(), GL_RGBA);
		
		allocated = true;
	}
}

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
			oldClip->fader.fadeOut();
		}

		if(curClip != NULL) {
			curClip->fader.fadeIn();
		}
	}
}

void phdClipTrack::fadeIn() {
	if(oldClip != NULL) oldClip->fader.fadeOut();
	if(curClip != NULL) curClip->fader.fadeIn();
}

void phdClipTrack::fadeOut() {
	if(oldClip != NULL) oldClip->fader.fadeOut();
	if(curClip != NULL) curClip->fader.fadeOut();
}
