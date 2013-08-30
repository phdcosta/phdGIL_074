#include "phdGrabberBase.h"

//--------------------------------------------------------------------------------------------------
string phdGrabberBase::getSetupString() {
		
	string _result = "GRABBER|" + getClassName() + "|";

	return _result;
}

//--------------------------------------------------------------------------------------------------
phdGrabberManager::phdGrabberManager() {
	curGrabberIndex = -1;
	curGrabber = NULL;
	usingEvents = false;
}

phdGrabberManager::~phdGrabberManager() {

	curGrabber = NULL;
	curGrabberIndex = -1;

	freeItems();
}

void phdGrabberManager::freeItems() {
	for (vector<phdGrabberBase*>::iterator i = items.begin(); i != items.end(); i++) {
		delete(*i); (*i) = NULL;
	}
	items.clear();
}

void phdGrabberManager::clear() {
	items.clear();
}

phdGrabberBase * phdGrabberManager::registerGrabber(phdGrabberBase * _item) {

	if(_item == NULL) return NULL;

	int _index = indexOf(_item);

	if(_index == -1) { // is a new grabber

		phdGrabberBase * _result = findByClassName(_item->getClassName());

		if(_result != NULL) return _result; // there is a grabber of that class

		items.push_back(_item);
		items[items.size()-1]->setManager((phdGrabberManager*) this);

		return _item;
	}
	return items[_index];
}

phdGrabberBase * phdGrabberManager::getGrabber(string _className) {
	return findByClassName(_className);
}

phdGrabberBase * phdGrabberManager::getGrabber(int _index) {
	if(_index > -1 && _index < items.size()) {
		return items[_index];
	}
	return NULL;
}

int phdGrabberManager::indexOf(phdGrabberBase * _item) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i] == _item) return i;
	}
	return -1;
}

phdGrabberBase * phdGrabberManager::findByClassName(string _className) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i]->getClassName() == _className) return items[i];
	}
	return NULL;
}

//--------------------------------------------------------------------------------------------------------------
void phdGrabberManager::setCurGrabber(phdGrabberBase * _curGrabber) {
	if(_curGrabber != curGrabber) {
		setCurGrabberIndex(indexOf(_curGrabber));
	}
}

phdGrabberBase * phdGrabberManager::getCurGrabber() {
	return curGrabber;
}

void phdGrabberManager::setCurGrabberIndex(int _index) {
	if(_index > -1 && _index < items.size()) {
		curGrabberIndex = _index;
		curGrabber = items[curGrabberIndex];
	} else {
		curGrabberIndex = -1;
		curGrabber = NULL;
	}
}

int phdGrabberManager::getCurGrabberIndex() {
	return curGrabberIndex;
}

void phdGrabberManager::draw() {
	if(curGrabber != NULL) {

		curGrabber->draw(0, 0, curGrabber->getWidth(), curGrabber->getHeight());

		double _x0, _y0, _x1, _y1;

		ofSetColor(255,255,255,255);

		curGrabber->getWarper()->getSource(0, _x0, _y0);
		curGrabber->getWarper()->getSource(1, _x1, _y1);
		ofLine(_x0, _y0, _x1, _y1);
		ofCircle(_x0, _y0, 5);

		curGrabber->getWarper()->getSource(1, _x0, _y0);
		curGrabber->getWarper()->getSource(2, _x1, _y1);
		ofLine(_x0, _y0, _x1, _y1);
		ofCircle(_x0, _y0, 5);

		curGrabber->getWarper()->getSource(2, _x0, _y0);
		curGrabber->getWarper()->getSource(3, _x1, _y1);
		ofLine(_x0, _y0, _x1, _y1);
		ofCircle(_x0, _y0, 5);

		curGrabber->getWarper()->getSource(3, _x0, _y0);
		curGrabber->getWarper()->getSource(0, _x1, _y1);
		ofLine(_x0, _y0, _x1, _y1);	ofCircle(_x0, _y0, 5);

		ofDrawBitmapString(curGrabber->getClassName(), _x1 + 30, _y1 + 30);

		if(hitTestInfo.infoType == gmhtWarpInputVertex && hitTestInfo.vertexIndex != -1) {
			ofSetColor(0,0,255,255);
			curGrabber->getWarper()->getSource(hitTestInfo.vertexIndex, _x0, _y0);
			ofCircle(_x0, _y0, 5);
		}

		ofSetColor(255,255,255,255);
	}
}

bool phdGrabberManager::updateHitTestInfo(float _x, float _y) {

	hitTestInfo.infoType = gmhtOutside;
	hitTestInfo.vertexIndex = -1;

	if(curGrabber != NULL) {
		int v = curGrabber->getWarper()->findNearestSource(_x, _y);
		if(v != -1) {
			hitTestInfo.infoType = gmhtWarpInputVertex;
			hitTestInfo.vertexIndex = v;
		}
	}

	return (hitTestInfo.infoType != gmhtOutside);
}

void phdGrabberManager::mouseMoved(ofMouseEventArgs& args) {
	updateHitTestInfo(args.x, args.y);
}

void phdGrabberManager::mousePressed(ofMouseEventArgs& args) {
	hitTestInfo.dragging = true;
	hitTestInfo.clickX = args.x;
	hitTestInfo.clickY = args.y;
}

void phdGrabberManager::mouseDragged(ofMouseEventArgs& args) {
	if(hitTestInfo.dragging && hitTestInfo.infoType == gmhtWarpInputVertex && curGrabber != NULL) {
		curGrabber->getWarper()->setSource(hitTestInfo.vertexIndex, args.x, args.y);
		curGrabber->getWarper()->setDstQuad(0.0, 0.0, 320.0, 0.0, 320.0, 240.0, 0.0, 240.0);
	}
}

void phdGrabberManager::mouseReleased(ofMouseEventArgs& args) {
	hitTestInfo.dragging = false;
	hitTestInfo.clickX = -1;
	hitTestInfo.clickY = -1;
}

void phdGrabberManager::enableEvents() {
	if(!usingEvents) {
		ofAddListener(ofEvents().mouseMoved, this, &phdGrabberManager::mouseMoved);
		ofAddListener(ofEvents().mousePressed, this, &phdGrabberManager::mousePressed);
		ofAddListener(ofEvents().mouseDragged, this, &phdGrabberManager::mouseDragged);
		ofAddListener(ofEvents().mouseReleased, this, &phdGrabberManager::mouseReleased);
		usingEvents = true;
	}
}

void phdGrabberManager::disableEvents() {
	if(usingEvents) {
		ofRemoveListener(ofEvents().mouseMoved, this, &phdGrabberManager::mouseMoved);
		ofRemoveListener(ofEvents().mousePressed, this, &phdGrabberManager::mousePressed);
		ofRemoveListener(ofEvents().mouseDragged, this, &phdGrabberManager::mouseDragged);
		ofRemoveListener(ofEvents().mouseReleased, this, &phdGrabberManager::mouseReleased);
		usingEvents = false;
	}
}

//--------------------------------------------------------------------------------------------------------------
phdMouseGrabber::phdMouseGrabber() {
	usingEvents = false; enableEvents();
}

phdMouseGrabber::~phdMouseGrabber() {
	disableEvents();
}

void phdMouseGrabber::setup() {
}

void phdMouseGrabber::update(phdGenParams * _params) {
}

void phdMouseGrabber::draw(phdGenParams * _params) {
	ofCircle(x, y, 3);
}

void phdMouseGrabber::draw(float _x, float _y, float _w, float _h, phdGenParams * _params) {
	ofCircle(_x + x * _w, _y + y * _h, 3);
}

void phdMouseGrabber::getNumericData(vector<float> & _results, vector<string> & _names) {
	_names.clear(); _results.clear();
	_names.push_back("X"); _results.push_back(x);
	_names.push_back("Y"); _results.push_back(y);
	_names.push_back("BUTTON"); _results.push_back(button);
}

void phdMouseGrabber::getStringData(vector<string> & _results, vector<string> & _names) {
	_names.clear(); _results.clear();
	_names.push_back("X"); _results.push_back(ofToString(x,2));
	_names.push_back("Y"); _results.push_back(ofToString(y,2));
	_names.push_back("BUTTON"); _results.push_back(ofToString(button));
}

void phdMouseGrabber::mouseMoved(ofMouseEventArgs& args) {
	x = ofMap(args.x, 0.0, ofGetWidth(), 0.0, 1.0);
	y = ofMap(args.y, 0.0, ofGetHeight(), 0.0, 1.0);
	button = args.button;
}

void phdMouseGrabber::mousePressed(ofMouseEventArgs& args) {
	x = ofMap(args.x, 0.0, ofGetWidth(), 0.0, 1.0);
	y = ofMap(args.y, 0.0, ofGetHeight(), 0.0, 1.0);
	button = args.button;
}

void phdMouseGrabber::mouseDragged(ofMouseEventArgs& args) {
	x = ofMap(args.x, 0.0, ofGetWidth(), 0.0, 1.0);
	y = ofMap(args.y, 0.0, ofGetHeight(), 0.0, 1.0);
	button = args.button;
}

void phdMouseGrabber::mouseReleased(ofMouseEventArgs& args) {
	x = ofMap(args.x, 0.0, ofGetWidth(), 0.0, 1.0);
	y = ofMap(args.y, 0.0, ofGetHeight(), 0.0, 1.0);
	button = args.button;
}

void phdMouseGrabber::enableEvents() {
	if(!usingEvents) {
		ofAddListener(ofEvents().mouseMoved, this, &phdMouseGrabber::mouseMoved);
		ofAddListener(ofEvents().mousePressed, this, &phdMouseGrabber::mousePressed);
		ofAddListener(ofEvents().mouseDragged, this, &phdMouseGrabber::mouseDragged);
		ofAddListener(ofEvents().mouseReleased, this, &phdMouseGrabber::mouseReleased);
		usingEvents = true;
	}
}

void phdMouseGrabber::disableEvents() {
	if(usingEvents) {
		ofRemoveListener(ofEvents().mouseMoved, this, &phdMouseGrabber::mouseMoved);
		ofRemoveListener(ofEvents().mousePressed, this, &phdMouseGrabber::mousePressed);
		ofRemoveListener(ofEvents().mouseDragged, this, &phdMouseGrabber::mouseDragged);
		ofRemoveListener(ofEvents().mouseReleased, this, &phdMouseGrabber::mouseReleased);
		usingEvents = false;
	}
}

//--------------------------------------------------------------------------------------------------
phdKeyGrabber::phdKeyGrabber() {
	usingEvents = false; enableEvents();
}

phdKeyGrabber::~phdKeyGrabber() {
	disableEvents();
}

void phdKeyGrabber::setup() {
}

void phdKeyGrabber::update(phdGenParams * _params) {
}

void phdKeyGrabber::draw(phdGenParams * _params) {
}

void phdKeyGrabber::draw(float _x, float _y, float _w, float _h, phdGenParams * _params) {
}

void phdKeyGrabber::getNumericData(vector<float> & _results, vector<string> & _names) {
}

void phdKeyGrabber::getStringData(vector<string> & _results, vector<string> & _names) {
}

void phdKeyGrabber::keyPressed(ofKeyEventArgs& eventArgs) {
}

void phdKeyGrabber::keyReleased(ofKeyEventArgs& eventArgs) {
}

void phdKeyGrabber::enableEvents() {
	if(!usingEvents) {
		ofAddListener(ofEvents().keyPressed, this, &phdKeyGrabber::keyPressed);
		ofAddListener(ofEvents().keyReleased, this, &phdKeyGrabber::keyReleased);
		usingEvents = true;
	}
}

void phdKeyGrabber::disableEvents() {
	if(usingEvents) {
		ofRemoveListener(ofEvents().keyPressed, this, &phdKeyGrabber::keyPressed);
		ofRemoveListener(ofEvents().keyReleased, this, &phdKeyGrabber::keyReleased);
		usingEvents = false;
	}
}

