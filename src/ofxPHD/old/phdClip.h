#pragma once

#include "ofMain.h"

#include "phdFader.h"
#include "phdTimer.h"
#include "phdGenParams.h"
#include "phdGenBase.h"

//--------------------------------------------------------------------------------------------------------------
class phdClipList;

//--------------------------------------------------------------------------------------------------------------
class phdClip {
private:
	float cropX, cropY, cropW, cropH;	//   0.0 -> 1.0   // destination canvas crop area
	float drawX, drawY, drawW, drawH;	//   0.0 -> 1.0   // destination canvas draw area
	float sizeX, sizeY;					//   0.0 -> +oo   // visual size on destination canvas
	bool isVisual;

	float alpha;

public:
	phdClipList * parent;

	phdGenBase * visual;
	phdFader fader;
	phdTimer timer;
	phdGenParams params;

	phdClip(phdGenBase * _visual = NULL);

	void setVisual(phdGenBase * _visual);

	int getTrackCount();

	inline bool getIsVisual();
	inline void setIsVisual(bool _isVisual);

	inline void setSizeX(float _sizeX);
	inline void setSizeY(float _sizeY);

	void setSize(float _sizeX, float _sizeY);
	void getSize(float & _sizeX, float & _sizeY);

	inline void setDrawX(float _drawX);
	inline void setDrawY(float _drawY);
	inline void setDrawW(float _drawW);
	inline void setDrawH(float _drawH);

	void setDrawArea(float _drawX, float _drawY, float _drawW, float _drawH);
	void getDrawArea(float & _drawX, float & _drawsY, float & _drawW, float & _drawH);

	inline void setCropX(float _cropX);
	inline void setCropY(float _cropY);
	inline void setCropW(float _cropW);
	inline void setCropH(float _cropH);

	void setCropArea(float _cropX, float _cropY, float _cropW, float _cropH);
	void getCropArea(float & _cropX, float & _cropY, float & _cropW, float & _cropH);

	bool isCropped();

	void setup();

	void fadeIn();
	void fadeOut();

	void play();
	void pause();
	void stop();

	void update();

	void draw(float _x, float _y, float _w, float _h);

	void drawCroppedArea(float _x, float _y, float _w, float _h, float _cx, float _cy, float _cw, float _ch);
};

//--------------------------------------------------------------------------------------------------------------
class phdClipList {
private:
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
};
