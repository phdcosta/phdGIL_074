#pragma once

#include "ofMain.h"

#include "phdClassesCode.h"
#include "phdGenParams.h"

//--------------------------------------------------------------------------------------------------------------
class phdGenBase {
private:
	bool paused;
	bool updated;
	float width;
	float height;

public:
	phdGenBase();

	virtual phdClassCode getClassCode() { return phdGenBaseClassCode; }
	virtual int getTrackCount() { return 0; }

	virtual void setWidth(float _width);
	virtual void setHeight(float _height);

	virtual float getWidth();
	virtual float getHeight();

	virtual void setFlagUpdate();

	virtual void play();
	virtual void pause();
	virtual void stop();

	virtual void setup();
	virtual void update(phdGenParams * _params = NULL);
	virtual void draw(phdGenParams * _params = NULL);
	virtual void draw(float x, float y, float w, float h, phdGenParams * _params = NULL);
};

class phdGenRectangle : public phdGenBase {
private:
	ofImage image;

public:
	phdGenRectangle() {
		phdGenBase::phdGenBase();
		setup();
	}

	void setup() {
		image.loadImage("data/base.jpeg");
	}

	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL) {
		ofSetColor(255,255,255,255);
		image.draw(x+w/10., y+h/10., w-w/10., h-h/10.);
	}
};

class phdGenEllipse : public phdGenBase {
private:
	ofImage image;

public:
	phdGenEllipse() {
		phdGenBase::phdGenBase();
		setup();
	}

	void setup() {
		image.loadImage("data/target.jpeg");
	}

	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL) {
		ofSetColor(255,255,255,255);
		image.draw(x+w/10., y+h/10., w-w/10., h-h/10.);
	}
};