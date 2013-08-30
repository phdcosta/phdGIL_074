#pragma once

#define PHDDEFAULTWIDTH 512.0
#define PHDDEFAULTHEIGHT 384.0

#define PHDUSEBLENDSHADER 1

class phdHotspotArea {
public:
	float x, y, w, h;
	bool selected;

	phdHotspotArea() { x = y = w = h = 0.0; selected = false; }

	phdHotspotArea(float _x, float _y, float _w, float _h, bool _selected = false) {
		setup(_x,_y,_w,_h, _selected);
	}

	void setup(float _x, float _y, float _w, float _h, bool _selected = false) {

		y = _y; h = _h;
		if(_h < 0) {
			h = fabs(_h);
			y = _y - h;
		}

		x = _x; w = _w;
		if(_w < 0) {
			w = fabs(_w);
			x = _x - w;
		}

		selected = _selected;
	}

	virtual bool pointInside(double _x, double _y) {
		return (_x >= x && _x <= x + w && _y > y && _y <= y + h);
	}
};
