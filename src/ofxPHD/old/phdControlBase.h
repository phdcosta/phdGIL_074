#pragma once

#include "ofMain.h"

class phdControlBase {
protected:
	float x, y, width, height;
	ofRectangle dragArea;
	double mousePosX, mousePosY, clickPosX, clickPosY, dragPosX, dragPosY;
	bool selected, focused, dragging, visible;

public:
	phdControlBase() {
		selected = focused = dragging = visible = false;
		mousePosX = mousePosY = clickPosX = clickPosY = dragPosX = dragPosY = 0.0;
		x = y = 0.0;
		width = height = 10.0;
	}

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

	virtual void setDragging(bool _dragging) { dragging = _dragging; }
	virtual bool getDragging() { return dragging; }

	virtual void setVisible(bool _visible) { visible = _visible; }
	virtual bool getVisible() { return visible; }

	virtual void setPosition(float _x, float _y) { setX(_x); setY(_y); }
	virtual void setDimension(float _width, float _height) { setWidth(_width); setHeight(_height); }

	virtual void setMousePos(float _x, float _y);
	virtual void setClickPos(float _x, float _y);
	virtual void setDragPos(float _x, float _y);

	virtual void getMousePos(float &_x, float &_y);
	virtual void getClickPos(float &_x, float &_y);
	virtual void getDragPos(float &_x, float &_y);

	virtual void updateHitTestInfo(float _x, float _y);
	virtual void populateGui(phdControlBase * _gui);

	virtual void onMouseMoved(ofMouseEventArgs& args);
	virtual void onMousePressed(ofMouseEventArgs& args);
	virtual void onMouseDragged(ofMouseEventArgs& args);
	virtual void onMouseReleased(ofMouseEventArgs& args);

	virtual void update();
	virtual void draw();
};