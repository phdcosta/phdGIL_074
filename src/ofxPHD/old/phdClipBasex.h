#pragma once

#include "ofMain.h"

#include "phdFader.h"
#include "phdTimer.h"
#include "phdGenParams.h"
#include "phdGenBase.h"
#include "phdClipTrack.h"

class phdClipBase {
private:
	float cropX, cropY, cropW, cropH;	//   0.0 -> 1.0   // destination canvas crop area
	float drawX, drawY, drawW, drawH;	//   0.0 -> 1.0   // destination canvas draw area
	float sizeX, sizeY;					//   0.0 -> +oo   // visual size on destination canvas
	bool isVisual;

public:
	phdClipTrack * track;
	phdGenBase * visual;
	phdFader fader;
	phdTimer timer;
	phdGenParams params;

	phdClipBase();
	phdClipBase(phdGenBase * _visual, phdClipTrack * _track);

	void setVisual(phdGenBase * _visual);

	inline bool getIsVisual() { return isVisual; }
	inline void setIsVisual(bool _isVisual) { isVisual = _isVisual; }

	inline void setSizeX(float _sizeX);
	inline void setSizeY(float _sizeY);

	virtual void setSize(float _sizeX, float _sizeY);
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

	inline bool isCropped();

	virtual void setup();

	virtual void fadeIn();
	virtual void fadeOut();

	virtual bool isMultiple() { return false; }
	virtual int getRows() { return 1; }

	virtual void play();
	virtual void pause();
	virtual void stop();

	virtual void update();

	virtual void draw(float _x, float _y, float _w, float _h);

	virtual void drawCroppedArea(float _x, float _y, float _w, float _h, float _cx, float _cy, float _cw, float _ch);
};