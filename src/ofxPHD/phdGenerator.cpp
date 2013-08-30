#include "phdGenerator.h"

//--------------------------------------------------------------------------------------------------------------
phdGenerator::phdGenerator() {
	setup();
}

void phdGenerator::deallocate() {
	stop();
}

void phdGenerator::play() {
	isPlaying = true;
}

void phdGenerator::pause() {
	isPlaying = false;
}

void phdGenerator::stop() {
	isPlaying = false;
}

void phdGenerator::setup() {
	isPlaying = false;
}

void phdGenerator::update(phdGenParams * _params) {
	if(isPlaying && needsUpdate) {
		// do all updates if not paused and not updates yet
	}
}

void phdGenerator::draw(phdGenParams * _params) {
	draw( 0.0, 0.0, getWidth(), getHeight(), _params);
}

void phdGenerator::draw(float _x, float _y, float _w, float _h, phdGenParams * _params) {
	ofSetColor(ofColor::white, 255);
	ofLine(_x, _y, _w, _h);
	ofLine(_x, _h, _w, _y);
}

void phdGenerator::setupFromFile(string _fileName) {
	// use this to load resources from file
}

void phdGenerator::saveToStrings(vector<string> & _strings) {
	// save generator params as strings
}

void phdGenerator::setSetupString(string _setupString) {
	// configure generator params from string
}

string phdGenerator::getSetupString() {
	// return string with params
	return "CLASSNAME:" + getClassName() + ":";
}

//--------------------------------------------------------------------------------------------------------------
phdGeneratorList::phdGeneratorList() {
}

phdGeneratorList::~phdGeneratorList() {
	freeItems();
}

void phdGeneratorList::freeItems() {
	for (vector<phdGenerator*>::iterator i = items.begin(); i != items.end(); i++) {
		delete(*i); (*i) = NULL;
	}
	items.clear();
}

void phdGeneratorList::clear() {
	items.clear();
}

int phdGeneratorList::indexOf(phdGenerator * _item) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i] == _item) return i;
	}
	return -1;
}

phdGenerator * phdGeneratorList::addItem(phdGenerator * _item) {
	int _index = indexOf(_item);
	if(_index == -1) {
		items.push_back(_item);
		return _item;
	}
	return items[_index];
}

//--------------------------------------------------------------------------------------------------------------
phdGeneratorManager::phdGeneratorManager() {
}

phdGeneratorManager::~phdGeneratorManager() {
	freeItems();
}

void phdGeneratorManager::freeItems() {
	for (vector<phdGenerator*>::iterator i = items.begin(); i != items.end(); i++) {
		delete(*i); (*i) = NULL;
	}
	items.clear();
}

void phdGeneratorManager::clear() {
	items.clear();
}

phdGenerator * phdGeneratorManager::registerGenerator(phdGenerator * _item) {

	if(_item == NULL) return NULL;

	int _index = indexOf(_item);

	if(_index == -1) { // is a new grabber

		phdGenerator * _result = findByClassName(_item->getClassName());

		if(_result != NULL) return _result; // there is a grabber of that class

		items.push_back(_item);
		items[items.size()-1]->setManager((phdGeneratorManager*) this);

		return _item;
	}
	return items[_index];
}

phdGenerator * phdGeneratorManager::getGenerator(string _className) {
	return findByClassName(_className);
}

phdGenerator * phdGeneratorManager::getGenerator(int _index) {
	if(_index > -1 && _index < items.size()) {
		return items[_index];
	}
	return NULL;
}

int phdGeneratorManager::indexOf(phdGenerator * _item) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i] == _item) return i;
	}
	return -1;
}

phdGenerator * phdGeneratorManager::findByClassName(string _className) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i]->getClassName() == _className) return items[i];
	}
	return NULL;
}
