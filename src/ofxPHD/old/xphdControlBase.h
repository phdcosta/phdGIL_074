#pragma once

#include "ofMain.h"

#include "phdPopupMenu.h"

class phdControlBase;

static const int linkSize = 10;

//--------------------------------------------------------------------------------------------------------------
class phdControlLink {
protected:
	float x, y;
	string name;
	phdControlBase * linkedControl;

public:
	phdControlLink(phdControlBase * _linkedControl = NULL) {
		linkedControl = _linkedControl;
	}

	void setLinkedControl(phdControlBase * _linkedControl) { _linkedControl = _linkedControl; }

	void setPosition(float _x, float _y) { x = _x; y = _y; }

	bool pointInside(float _x, float _y) {
		//return (_x >= x && _x <= _x + (float)(linkSize * 0.5) && _y >= y && _y <= y + (float)(linkSize * 0.5));
		
		return (ofDistSquared(_x, _y, x, y) <= ofDistSquared(0, 0, (float)(linkSize*0.5), (float)(linkSize*0.5) ) );
	}

	void draw(bool focused = false) {
		ofCircle(x, y, (float)(linkSize * 0.5));
	}
};

//--------------------------------------------------------------------------------------------------------------
enum phdControlLinksDrawMode { ldmHorLeft, ldmHorRight, ldmVerBottom, ldmVerTop };

//--------------------------------------------------------------------------------------------------------------
class phdControlLinks {
protected:
	float x, y;
	phdControlLinksDrawMode drawMode;
	phdControlBase * parent;
	vector<phdControlLink> links;

public:
	int focusedIndex;

	phdControlLinks(phdControlBase * _parent = NULL, phdControlLinksDrawMode _drawMode = ldmVerBottom) {
		parent = _parent;
		focusedIndex = -1;
	}

	phdControlLink * phdControlLinks::operator[](unsigned i) { return &links[i]; }
	inline int size() { return links.size(); }

	phdControlLink * addLink() {
		links.push_back(phdControlLink(NULL));
		return &links[links.size()-1];
	};

	void setDrawMode(phdControlLinksDrawMode _drawMode) {
		drawMode = _drawMode;
		setPosition(x, y);
	}

	void setPosition(float _x, float _y) {

		if(_x == x && _y == y) return;

		x = _x;
		y = _y;

		for(int i = 0; i < links.size(); i++) {
			switch(drawMode) {
			case ldmHorLeft :
				links[i].setPosition(_x - (float)((i + 0.5) * linkSize * 1.25), _y);
				break;
			case ldmHorRight :
				links[i].setPosition(_x + (float)((i + 0.5) * linkSize * 1.25), _y);
				break;
			case ldmVerTop :
				links[i].setPosition(_x, _y - (float)((i + 0.5) * linkSize * 1.25));
				break;
			case ldmVerBottom :
				links[i].setPosition(_x, _y + (float)((i + 0.5) * linkSize * 1.25));
				break;
			}
		}
	}

	bool updateHitTestInfo(float _x, float _y) {
		for(int i = 0; i < links.size(); i++) {
			if(links[i].pointInside(_x, _y)) {
				focusedIndex = i;
				return true;
			}
		}
		focusedIndex = -1;
		return false;
	}

	void draw(float _x, float _y) {

		setPosition(_x, _y);

		ofPushStyle();

		ofNoFill();
		ofSetColor(192, 192, 255, 255);
		for(int i = 0; i < links.size(); i++) {
			if(focusedIndex == i) ofFill();
			links[i].draw();
			if(focusedIndex == i) ofNoFill();
		}
		ofPopStyle();
	}
};

//--------------------------------------------------------------------------------------------------------------
enum phdHitTestInfoType {
	ghtOutside, ghtInside,
	ghtMenu, ghtDragControl,
	ghtVerticalScroll, ghtHorizontalScroll, ghtResize, ghtCommand,
	ghtInputLink, ghtOutputLink
};

//--------------------------------------------------------------------------------------------------------------
class phdControlBase {
protected:
	float x, y, width, height;
	bool selected, focused, dragging, visible;
	phdPopupMenu * popupMenu;

public:
	phdControlLinks inputLinks;
	phdControlLinks outputLinks;

	phdControlBase();
	~phdControlBase() { }

	virtual void setX(float _x) { x = _x; }
	virtual float getX() { return x; }

	virtual void setY(float _y) { y = _y; }
	virtual float getY() { return y; }

	virtual void setWidth(float _width) { width = _width; }
	virtual float getWidth() { return width; }

	virtual void setHeight(float _height) { height = _height; }
	virtual float getHeight() { return height; }

	virtual void setSelected(bool _selected) { selected = _selected; }
	virtual bool getSelected() { return selected; }

	virtual void setFocused(bool _focused) { focused = _focused; }
	virtual bool getFocused() { return focused; }

	virtual void setVisible(bool _visible) { visible = _visible; }
	virtual bool getVisible() { return visible; }

	virtual void setPosition(float _x, float _y) { setX(_x); setY(_y); }
	virtual void setDimension(float _width, float _height) { setWidth(_width); setHeight(_height); }

	virtual void setPopupMenu(phdPopupMenu * _menu) { popupMenu = _menu; }
	virtual phdPopupMenu * getPopupMenu() { return popupMenu; }

	virtual bool updateHitTestInfo(float _x, float _y);
	virtual void populateGui(phdControlBase * _gui);

	virtual void onMouseMoved(ofMouseEventArgs& args);
	virtual void onMousePressed(ofMouseEventArgs& args);
	virtual void onMouseDragged(ofMouseEventArgs& args);
	virtual void onMouseReleased(ofMouseEventArgs& args);

	virtual void update();
	virtual void draw();

	virtual void onPopupClick(phdPopupMenuEventData &_eventData);
};