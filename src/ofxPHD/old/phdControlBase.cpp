#include "phdControlBase.h"

//----------------------------------------------------------------------------------------------------------------------

void phdControlBase::setMousePos(float _x, float _y) {
	if(_x >= getX() && _x < getX() + getWidth() && _y >= getY() && _y < getY() + getHeight()) {
		mousePosX = _x - getX();
		mousePosY = _y - getY();
	}
}

void phdControlBase::setClickPos(float _x, float _y) {
	if(_x >= getX() && _x < getX() + getWidth() && _y >= getY() && _y < getY() + getHeight()) {
		clickPosX = _x - getX();
		clickPosY = _y - getY();
	}
}

void phdControlBase::setDragPos(float _x, float _y) {
	dragPosX = _x - getX();
	if(dragPosX < 0) dragPosX = 0;
	if(dragPosX > getWidth()) dragPosX = getWidth();
	dragPosY = _y - getY();
	if(dragPosY < 0) dragPosY = 0;
	if(dragPosY > getHeight()) dragPosY = getHeight();
}

void phdControlBase::getMousePos(float &_x, float &_y) {
	_x = mousePosX; _y = mousePosY;
}

void phdControlBase::getClickPos(float &_x, float &_y) {
	_x = clickPosX; _y = clickPosY;
}

void phdControlBase::getDragPos(float &_x, float &_y) {
	_x = dragPosX; _y = dragPosY;
}

void phdControlBase::updateHitTestInfo(float _x, float _y) {
}

void phdControlBase::populateGui(phdControlBase * _gui) {
}

void phdControlBase::onMouseMoved(ofMouseEventArgs& args) {
	setMousePos(args.x, args.y);
}

void phdControlBase::onMousePressed(ofMouseEventArgs& args) {
	dragging = false;;
	setMousePos(args.x, args.y);
	setClickPos(args.x, args.y);
	setDragPos(args.x, args.y);
	dragArea.set(clickPosX, clickPosY, 0, 0);
}

void phdControlBase::onMouseDragged(ofMouseEventArgs& args) {
	setMousePos(args.x, args.y);
	setDragPos(args.x, args.y);

	dragArea.x = clickPosX < dragPosX ? clickPosX : dragPosX;
	dragArea.y = clickPosY < dragPosY ? clickPosY : dragPosY;
	dragArea.width = fabs(dragPosX - clickPosX);
	dragArea.height = fabs(dragPosY - clickPosY);

	dragging = dragging || (dragArea.width > 0 || dragArea.height > 0);
}

void phdControlBase::onMouseReleased(ofMouseEventArgs& args) {
	setMousePos(args.x, args.y);
	dragArea.set(0,0,0,0);
	dragging = false;
}

void phdControlBase::update() {
}

void phdControlBase::draw() {

	ofPushStyle();

	ofNoFill();
	ofSetColor(255,255,255,255);
	if(selected) ofSetColor(255,255,0,255);
	if(focused && !selected) ofSetColor(255,0,100,255);
	ofRect(getX(), getY(), getWidth(), getHeight());

	ofSetColor(255,255,255,255);
	if(dragArea.width > 0 || dragArea.height > 0) ofRect(getX() + dragArea.x, getY() + dragArea.y, dragArea.width, dragArea.height);

	ofDrawBitmapString("mouseX:"+ofToString(mousePosX, 0)+" mouseY:"+ofToString(mousePosY, 0), getX(), getY() + 15);
	ofDrawBitmapString("x:"+ofToString(dragArea.x,1) + " y:"+ofToString(dragArea.y,1) + " w:"+ofToString(dragArea.width,1) + " h:"+ofToString(dragArea.height,1), getX(), getY() + 30);
	ofDrawBitmapString("dx:"+ofToString(dragPosX-clickPosX, 0)+" dy:"+ofToString(dragPosY-clickPosY, 0), getX(), getY() + 45);

	ofPopStyle();
}