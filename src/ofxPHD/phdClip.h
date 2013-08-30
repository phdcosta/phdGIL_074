#pragma once

#include "ofMain.h"

#include "phdFader.h"
#include "phdTimer.h"
#include "phdGenParams.h"
#include "phdGenerator.h"
#include "phdGrabberBase.h"
#include "phdFilter.h"

//--------------------------------------------------------------------------------------------------------------
class phdClipList;

//--------------------------------------------------------------------------------------------------------------
class phdClip {
protected:
	float drawX, drawY, drawW, drawH, drawA;	// generator draw input coords and angle

	float internalWidth, internalHeight;
	ofFbo internalCanvas;
	bool allocated;
	void allocateCanvas();

	bool isActive;
	bool startedIn, startedOut;

public:
	phdClipList * parent;

	phdGenerator * generator;	// who is generating image for this clip
	phdGrabberBase * grabber;	// who is generating data for this clip

	phdGenParams params;
	phdFilterList filters;		// post processing filters for this clip

	phdFader fader;
	phdTimer timer;
	//phdAudioSpectrum audio

	phdClip(phdGenerator * _generator = NULL);
	~phdClip() { filters.clear(); }

	void setGenerator(phdGenerator * _generator);
	phdGenerator * getGenerator();

	bool getAllocated() { return internalCanvas.isAllocated(); }

	virtual int getTrackCount() { return 0; }  // track count for ui control

	virtual void setActive(bool _active) { isActive = _active; }
	virtual bool getActive() { return isActive && internalCanvas.isAllocated(); }

	virtual void setWidth(float _width) { internalWidth = _width; allocateCanvas(); }
	virtual float getWidth() { return internalWidth; }//internalCanvas.getWidth(); }

	virtual void setHeight(float _height) { internalHeight = _height; allocateCanvas(); }
	virtual float getHeight() { return internalHeight; } //internalCanvas.getHeight(); }

	virtual void setDrawX(float _drawX) { drawX = _drawX; }
	virtual float getDrawX() { return drawX; }

	virtual void setDrawY(float _drawY) { drawY = _drawY; }
	virtual float getDrawY() { return drawY; }

	virtual void setDrawW(float _drawW) { drawW = _drawW; }
	virtual float getDrawW() { return drawW; }

	virtual void setDrawH(float _drawH) { drawH = _drawH; }
	virtual float getDrawH() { return drawH; }

	virtual void setDrawAngle(float _drawA) { drawA = fmod(_drawA, 360.0f); }
	virtual float getDrawAngle() { return drawA; }

	virtual void setDrawArea(float _x, float _y, float _w, float _h);
	virtual void getDrawArea(float & _x, float & _y, float & _w, float & _h) {
		_x = getDrawX(); _y = getDrawY(); _w = getDrawW(); _h = getDrawH();
	}

	virtual ofFbo & getCanvas() { return internalCanvas; }

	virtual void fadeIn();
	virtual void fadeOut();

	virtual void play();
	virtual void pause();
	virtual void stop();

	virtual void setup();
	virtual void update();
	virtual void postProcessingFilter();
	virtual void draw(float _x, float _y, float _w, float _h);

};

//--------------------------------------------------------------------------------------------------------------
class phdClipList {
protected:
	vector<phdClip*> items;

public:
	phdClipList();
	~phdClipList();

	phdClip * phdClipList::operator[](unsigned i) { return items[i]; }
	int phdClipList::size() { return items.size(); }

	void freeItems();
	void clear();
	int indexOf(phdClip * _item);
	phdClip * addItem(phdClip * _item);
	void delItem(phdClip * _item);
	void delItem(int _index);
};
