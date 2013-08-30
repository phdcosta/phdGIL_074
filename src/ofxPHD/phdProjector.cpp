#include "phdProjector.h"

//--------------------------------------------------------------------------------------------------------------
phdProjectorControl::phdProjectorControl() {
	drawMode = pdmBoth;
}

phdProjectorControl::~phdProjectorControl() {
}

void phdProjectorControl::setSource(float _x, float _y, float _w, float _h) {

	warper.setSrcQuad(_x, _y, _x + _w, _y, _x + _w, _y + _h, _x, _y + _h);

}

bool phdProjectorControl::updateHitTestInfo(float _x, float _y) {

	hitTestInfo.infoType = pwtOutside;
	hitTestInfo.mouseX = _x;
	hitTestInfo.mouseY = _y;

	hitTestInfo.vertex = warper.findNearestTarget(_x, _y);

	if(hitTestInfo.vertex != -1) {

		hitTestInfo.infoType = pwtVertex;

	} else {
		ofPolyline _poly;
		for(int i = 0; i < 4; i++) {
			double _x, _y;
			warper.getTarget(i, _x, _y);
			_poly.addVertex(_x, _y);
		}
		if(phdPointInsidePolygon(_poly, _x, _y)) hitTestInfo.infoType = pwtInside;
	}

	return (hitTestInfo.infoType != pwtOutside);
}

void phdProjectorControl::update() {
}

void phdProjectorControl::draw() {

	ofPushStyle();
	ofPushMatrix();

	if(drawMode == pdmContents || drawMode == pdmBoth) {

		double _xs, _ys, _xt, _yt;

		//ofSetColor(0,0,0,0);
		warper.begin();
			glBegin(GL_TRIANGLES);
				// top right triangle
				warper.getSource(0, _xs, _ys);	glTexCoord2d(_xs, _ys);// glColor4f(1.0,1.0,1.0,1.0);
				warper.getTarget(0, _xt, _yt);	glVertex2d(_xs, _ys);

				warper.getSource(3, _xs, _ys);	glTexCoord2d(_xs, _ys);// glColor4f(1.0,1.0,1.0,1.0);
				warper.getTarget(3, _xt, _yt);	glVertex2d(_xs, _ys);

				warper.getSource(2, _xs, _ys);	glTexCoord2d(_xs, _ys);// glColor4f(1.0,1.0,1.0,1.0);
				warper.getTarget(2, _xt, _yt);	glVertex2d(_xs, _ys);

				// left bottom triangle
				warper.getSource(0, _xs, _ys);	glTexCoord2d(_xs, _ys);// glColor4f(1.0,1.0,1.0,1.0);
				warper.getTarget(0, _xt, _yt);	glVertex2d(_xs, _ys);
					
				warper.getSource(1, _xs, _ys);	glTexCoord2d(_xs, _ys);// glColor4f(1.0,1.0,1.0,1.0);
				warper.getTarget(1, _xt, _yt);	glVertex2d(_xs, _ys);
					
				warper.getSource(2, _xs, _ys);	glTexCoord2d(_xs, _ys);// glColor4f(1.0,1.0,1.0,1.0);
				warper.getTarget(2, _xt, _yt);	glVertex2d(_xs, _ys);
			glEnd();
		warper.end();
		ofSetColor(255,255,255,255);
	}

	if(drawMode == pdmWarper || drawMode == pdmBoth) {

		double _x0, _y0, _x1, _y1;

		ofDisableAlphaBlending();
		ofSetColor(255,255,255,255);

		warper.getTarget(0, _x0, _y0);	warper.getTarget(1, _x1, _y1);	ofLine(_x0, _y0, _x1, _y1); ofCircle(_x0, _y0, 5);
		warper.getTarget(1, _x0, _y0);	warper.getTarget(2, _x1, _y1);	ofLine(_x0, _y0, _x1, _y1); ofCircle(_x0, _y0, 5);
		warper.getTarget(2, _x0, _y0);	warper.getTarget(3, _x1, _y1);	ofLine(_x0, _y0, _x1, _y1); ofCircle(_x0, _y0, 5);
		warper.getTarget(3, _x0, _y0);	warper.getTarget(0, _x1, _y1);	ofLine(_x0, _y0, _x1, _y1); ofCircle(_x0, _y0, 5);

		if(hitTestInfo.infoType == pwtVertex && hitTestInfo.vertex != -1) {
			ofSetColor(0,0,255,255);
			warper.getTarget(hitTestInfo.vertex, _x0, _y0);
			ofCircle(_x0, _y0, 5);
		}
		ofSetColor(255,255,255,255);
		ofEnableAlphaBlending();
	}

	ofPopMatrix();
	ofPopStyle();
}

void phdProjectorControl::onMouseMoved(ofMouseEventArgs& args) {
	updateHitTestInfo(args.x, args.y);
}

void phdProjectorControl::onMousePressed(ofMouseEventArgs& args) {

	hitTestInfo.mouseDownX = args.x;
	hitTestInfo.mouseDownY = args.y;

	if(hitTestInfo.infoType == pwtVertex) {

		hitTestInfo.dragging = true;

	} else if(hitTestInfo.infoType == pwtInside) {

		hitTestInfo.dragging = true;

	} else if(hitTestInfo.infoType == pwtEdge) {
	} else if(hitTestInfo.infoType == pwtOutside) {
	}
}

void phdProjectorControl::onMouseDragged(ofMouseEventArgs& args) {

	if(hitTestInfo.infoType == pwtVertex) {

		if(hitTestInfo.dragging && hitTestInfo.vertex != -1) {
			warper.setTarget(hitTestInfo.vertex, args.x, args.y);
		}

	} else if(hitTestInfo.infoType == pwtInside) {

		if(hitTestInfo.dragging) {
			for(int i = 0; i < 4; i++) {
				double _x, _y;
				warper.getTarget(i, _x, _y);
				warper.setTarget(i, _x + args.x - hitTestInfo.mouseX, _y + args.y - hitTestInfo.mouseY);
			}
		}

	} else if(hitTestInfo.infoType == pwtEdge) {
	} else if(hitTestInfo.infoType == pwtOutside) {
	}

	hitTestInfo.mouseX = args.x;
	hitTestInfo.mouseY = args.y;
}

void phdProjectorControl::onMouseReleased(ofMouseEventArgs& args) {
	hitTestInfo.dragging = false;
}

void phdProjectorControl::onPopupClick(phdPopupMenuEventData &_eventData) {
}

string phdProjectorControl::getSetupString() {

	return getClassName() + "|" + "SCREEN_" + warper.getAsString();		
}

void phdProjectorControl::saveToStrings(vector<string> & _strings, string _title) {

	_strings.push_back("PROJECTOR|" + getSetupString());

}

void phdProjectorControl::setupFromString(string _setupString) {

	vector<string> _items = ofSplitString(ofToUpper(_setupString), "|", true, true);

	int _size = _items.size() - 1;
	for(int i = 0; i < _size; i++) { // works in "NAME|VALUE|"
		if(_items[i] == "DRAWMODE" && i < _size) {
			if(_items[i+1] == "PDMCONTENTS") {
				drawMode = pdmContents;
			} else if(_items[i+1] == "PDMBOTH") {
				drawMode = pdmBoth;
			} else {
				drawMode = pdmWarper;
			}
		}
	}
}