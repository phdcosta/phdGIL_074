#pragma once

#include "ofMain.h"

#include "phdGenFbo.h"
#include "phdKinectGrabber.h"
#include "camFilter\src\ofxCameraFilter.h"

//--------------------------------------------------------------------------------------------------------------
class phdGenAura : public phdGenFbo {
protected:
	phdKinectGrabber * grabber; 
	ofxCameraFilter camFilter;

public:
	phdGenAura() {
		phdGenerator::phdGenerator();
		grabber = NULL;
		setup();
	}

	string getClassName() { return "PHDGENAURA"; }
	phdGenerator * createInstanceSameClass() { return new phdGenAura(); }

	void setup() {
		allocated = true;
		setDimension(320, 240);
		camFilter.setup(320, 240);
		camFilter.setAberrationAmount(0.7);
		camFilter.setNoiseAmount(0.0);
		camFilter.setBlurScale(5.0);
	}

	void update(phdGenParams * _params = NULL) {
		if(allocated) {
			if(_params != NULL && _params->grabber != NULL) {
				grabber = (phdKinectGrabber*) _params->grabber;
				grabber->update(_params);

				//setDimension(grabber->getWidth(), grabber->getHeight());

				camFilter.begin();
					ofSetColor(255,255,255,255);
					//grabber->getTracker().fullMask.draw(0, 0);
					grabber->getTracker().fullMask.bind();
					glBegin(GL_QUADS);
						glTexCoord2f(0,  0);
						glVertex2f(0, 0);

						glTexCoord2f(grabber->getTracker().masksW, 0);
						glVertex2f(getWidth(), 0);

						glTexCoord2f(grabber->getTracker().masksW, grabber->getTracker().masksH);
						glVertex2f(getWidth(), getHeight());

						glTexCoord2f(0,  grabber->getTracker().masksH);
						glVertex2f(0, getHeight());
					glEnd();
					grabber->getTracker().fullMask.unbind();
				camFilter.end();

				internalCanvas.begin();
					camFilter.draw();
				internalCanvas.end();
			}
		} else {
			allocateCanvas();
		}
	}

	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL) {
		if(allocated && grabber != NULL) {
			double _x, _y;
			//grabber->inputWarper.begin();
			//internalCanvas.draw(x,y,w,h);
			//grabber->inputWarper.end();
			phdWarper * _warper = grabber->getWarper();
			if(_warper != NULL) {
				_warper->invBegin();
					internalCanvas.getTextureReference().bind();
					glBegin(GL_QUADS);
						_warper->getSource(0, _x, _y);
						glTexCoord2f(_x,  _y);
						glVertex2f(x, y);

						_warper->getSource(1, _x, _y);
						glTexCoord2f(_x,  _y);
						glVertex2f(x+w, y);

						_warper->getSource(2, _x, _y);
						glTexCoord2f(_x,  _y);
						glVertex2f(x+w, y+h);

						_warper->getSource(3, _x, _y);
						glTexCoord2f(_x,  _y);
						glVertex2f(x, y+h);
					glEnd();
	 				internalCanvas.getTextureReference().unbind();
				_warper->end();
			}
		}
	}

	void setSetupString(string _setupString) {
	}

	string getSetupString() {
		string _result = "GENERATOR|" + getClassName() + "|"; 
		if(grabber != NULL) _result += "GRABBER|" + grabber->getClassName() + "|";
		return _result;
	}
};