#include "phdMixer.h"

//--------------------------------------------------------------------------------------------------

phdMixer::phdMixer() {
	setup();
}

void phdMixer::fadeIn() {
	fader.fadeIn();
}

void phdMixer::fadeOut() {
	fader.fadeOut();
}

void phdMixer::play() {
	if(clipA != NULL) clipA->play();
	if(clipB != NULL) clipB->play();
}

void phdMixer::pause() {
	if(clipA != NULL) clipA->pause();
	if(clipB != NULL) clipB->pause();
}

void phdMixer::stop() {
	if(clipA != NULL) clipA->stop();
	if(clipB != NULL) clipB->stop();
}

void phdMixer::setup() {
	clipA = clipB = NULL;
	setWidth(PHDDEFAULTWIDTH);
	setHeight(PHDDEFAULTHEIGHT);
	setDrawArea(0.0, 0.0, PHDDEFAULTWIDTH, PHDDEFAULTHEIGHT);
	setDrawAngle(0.0);
	setActive(true);
}

void phdMixer::update() {

	fader.update();
	timer.update();

	if(!getActive()) return;

	if(clipA != NULL) { clipA->fader.update(); }
	if(clipB != NULL) { clipB->fader.update(); }

	bool drawA = (
		clipA != NULL && clipA->generator != NULL &&
		clipA->getActive() &&
		clipA->fader.getValue() != 0.0);

	bool drawB = (
		clipB != NULL && clipB->generator != NULL &&
		clipB->getActive() &&
		clipB->fader.getValue() != 0.0);

	// updates clipA canvas
	if(drawA) {
		clipA->update();
	}

	// updates clipB canvas
	if(drawB) {
		clipB->update();
	}

	if(drawA && !drawB) { // clipA

		getCanvas().begin();
			ofEnableAlphaBlending();
			ofClear(0, 0, 0, 0);
			ofSetColor(ofColor::white, (int)(255.0 * clipA->fader.getValue()));

			ofPushMatrix();

			ofTranslate(getDrawX() + getDrawW() / 2.0, getDrawY() + getDrawH() / 2.0);
			ofRotateZ(getDrawAngle());

			clipA->draw(getDrawX() - getDrawW() / 2.0, getDrawY() - getDrawH() / 2.0, getDrawW(), getDrawH());

			ofPopMatrix();

		getCanvas().end();

	} else if(!drawA && drawB) { // clipB

		getCanvas().begin();
			ofEnableAlphaBlending();
			ofClear(0, 0, 0, 0);
			ofSetColor(ofColor::white, (int)(255.0 * clipB->fader.getValue()));

			ofPushMatrix();

			ofTranslate(getDrawX() + getDrawW() / 2.0, getDrawY() + getDrawH() / 2.0);
			ofRotateZ(getDrawAngle());

			clipB->draw(getDrawX() - getDrawW() / 2.0, getDrawY() - getDrawH() / 2.0, getDrawW(), getDrawH());

			ofPopMatrix();
		getCanvas().end();

	} else if(drawA && drawB) { // mixes clipA and clipB

		getCanvas().begin();
			glEnable (GL_BLEND);  
			glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD); 

			ofClear(0, 0, 0, 0);

			ofPushMatrix();

			ofTranslate(getDrawX() + getDrawW() / 2.0, getDrawY() + getDrawH() / 2.0);
			ofRotateZ(getDrawAngle());

			ofSetColor(ofColor::white, (int)(255.0 * clipA->fader.getValue()));
			clipA->draw(getDrawX() - getDrawW() / 2.0, getDrawY() - getDrawH() / 2.0, getDrawW(), getDrawH());

			ofSetColor(ofColor::white, (int)(255.0 * clipB->fader.getValue()));
			clipB->draw(getDrawX() - getDrawW() / 2.0, getDrawY() - getDrawH() / 2.0, getDrawW(), getDrawH());

			ofPopMatrix();

			glDisable(GL_BLEND);

			/*blendShader.draw(3, // 3, 6, 7, 5, 2, 8, 16, 18, 19, 20, 22
				oldCropCanvas.getTextureReference(), clipA->fader.getValue(),
				curCropCanvas.getTextureReference(), clipB->fader.getValue(),
				trackCanvas.getWidth(), trackCanvas.getHeight());*/

		getCanvas().end();
	}
}

void phdMixer::draw(float _x, float _y, float _w, float _h) {

	getCanvas().draw( _x, _y, _w, _h);

}

// ----------- class specific methods ---------------
