#include "phdFilter.h"

//--------------------------------------------------------------------------------------------------
void phdFilter::allocateCanvas() {
	if(internalWidth > 0 && internalHeight > 0 &&
		(!internalCanvas.isAllocated() ||
		internalCanvas.getWidth() != getWidth() || internalCanvas.getHeight() != getHeight())) {
		internalCanvas.allocate(getWidth(), getHeight(), GL_RGBA);
		allocated = true;
	}
}

//--------------------------------------------------------------------------------------------------
phdFilter::phdFilter() {
	setup();
	fileName = "";
}

void phdFilter::setup() {
	setWidth(PHDDEFAULTWIDTH);
	setHeight(PHDDEFAULTHEIGHT);
}

void phdFilter::setupFromFile(string _fileName) {
	if(fileName != _fileName) {
		fileName =_fileName;
		shader.load("filters/" + _fileName);
	}
}

void phdFilter::update(phdGenParams * _params) {

	if(allocated && internalCanvas.isAllocated()) {

		if(_params != NULL && _params->canvas != NULL && _params->canvas->isAllocated()) {

			// copy source canvas contents
			internalCanvas.begin();
				_params->canvas->getTextureReference().bind();
					glBegin(GL_QUADS);
						glTexCoord2f(0, 0);
						glVertex3f(0, 0, 0);

						glTexCoord2f(_params->canvas->getWidth(), 0);
						glVertex3f(getWidth(), 0, 0);

						glTexCoord2f(_params->canvas->getWidth(), _params->canvas->getHeight());
						glVertex3f(getWidth(), getHeight(), 0);

						glTexCoord2f(0, _params->canvas->getHeight());
						glVertex3f(0, getHeight(), 0);
					glEnd();
				_params->canvas->getTextureReference().unbind();
			internalCanvas.end();

			// apply filter and draw on canvas source
			_params->canvas->begin();

				shader.begin();

					_params->time = ofGetElapsedTimef();
					_params->resolution[0] = 512; _params->resolution[1] = 384;
					_params->mouse[0] = ofGetMouseX(); _params->mouse[1] = ofGetMouseY();
					_params->surfaceSize[0] = 512; _params->surfaceSize[1] = 384;
					_params->clrBase[0] = 0.2; _params->clrBase[1] = 0.2; _params->clrBase[2] = 0.2; _params->clrBase[3] = 0.2;
					_params->clrLow[0] = 0.0; _params->clrLow[1] = 0.0; _params->clrLow[2] = 0.0; _params->clrLow[3] = 0.0;
					_params->clrHigh[0] = 1.0; _params->clrHigh[1] = 1.0; _params->clrHigh[2] = 1.0; _params->clrHigh[3] = 1.0;
					_params->params[0] = 0.25; _params->params[1] = 1.0; _params->params[2] = 1.0; _params->params[3] = 1.0;
					_params->pos[0] = 0.5; _params->pos[1] = 0.5;
					_params->scale[0] = 1.0; _params->scale[1] = 1.0;

					// generate visuals using shader over internal FBO
					shader.setUniform1f("time", _params->time);
					shader.setUniform2f("resolution", _params->resolution[0], _params->resolution[1]);
					shader.setUniform2f("mouse", _params->mouse[0], _params->mouse[1]);
					shader.setUniform2f("surfacesize", _params->surfaceSize[0], _params->surfaceSize[1] );
					shader.setUniform4f("clrBase", _params->clrBase[0], _params->clrBase[1], _params->clrBase[2], _params->clrBase[3] );
					shader.setUniform4f("clrLow", _params->clrLow[0], _params->clrLow[1], _params->clrLow[2], _params->clrLow[3] );
					shader.setUniform4f("clrHigh", _params->clrHigh[0], _params->clrHigh[1], _params->clrHigh[2], _params->clrHigh[3] );
					shader.setUniform4f("params", _params->params[0], _params->params[1], _params->params[2], _params->params[3] );
					shader.setUniform2f("pos", _params->pos[0], _params->pos[1]);
					shader.setUniform2f("scale", _params->scale[0], _params->scale[1]);

					shader.setUniformTexture("tex0", internalCanvas.getTextureReference(), 0 );
					shader.setUniform2f("size0", internalCanvas.getWidth(), internalCanvas.getHeight());

					//shader.setUniformTexture("tex1", internalCanvas.getTextureReference(), 1 );
					//shader.setUniform2f("size1", internalCanvas.getWidth(), internalCanvas.getHeight());

					ofSetColor(ofColor::white);

					glBegin(GL_QUADS);
						glTexCoord2f(0, 0);
						glVertex3f(0, 0, 0);
						glTexCoord2f(_params->canvas->getWidth(), 0);
						glVertex3f(getWidth(), 0, 0);
						glTexCoord2f(_params->canvas->getWidth(), _params->canvas->getHeight());
						glVertex3f(getWidth(), getHeight(), 0);
						glTexCoord2f(0, _params->canvas->getHeight());
						glVertex3f(0, getHeight(), 0);
					glEnd();

				shader.end();

			_params->canvas->end();
		}
	}
}

void phdFilter::draw(float x, float y, float w, float h, phdGenParams * _params) {
	if(allocated) {
		internalCanvas.draw(x,y,w,h);
	}
}

//--------------------------------------------------------------------------------------------------------------
phdFilterList::phdFilterList() {
	autoFreeItems = false; // dont free items
}

phdFilterList::~phdFilterList() {
	if(autoFreeItems) freeItems();
}

void phdFilterList::freeItems() {
	for (vector<phdFilter*>::iterator i = items.begin(); i != items.end(); i++) {
		delete(*i); (*i) = NULL;
	}
	items.clear();
}

void phdFilterList::clear() {
	items.clear();
}

int phdFilterList::indexOf(phdFilter * _item) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i] == _item) return i;
	}
	return -1;
}

phdFilter * phdFilterList::addItem(phdFilter * _item) {
	int _index = indexOf(_item);
	if(_index == -1) {
		items.push_back(_item);
		return _item;
	}
	return items[_index];
}

void phdFilterList::delItem(phdFilter * _item) {
	delItem(indexOf(_item));
}

void phdFilterList::delItem(int _index) {
	if(_index > -1 && _index < items.size()) {
		delete items[_index]; items[_index] = NULL;
		items.erase(items.begin() + _index);
	}
}

phdFilter * phdFilterList::pushItem(phdFilter * _item) {
	return addItem(_item);
}

void phdFilterList::popItem(phdFilter * _item) {
	popItem(indexOf(_item));
}

void phdFilterList::popItem(int _index) {
	if(_index > -1 && _index < items.size()) {
		items.erase(items.begin() + _index);
	}
}

//--------------------------------------------------------------------------------------------------------------
phdFilterManager::phdFilterManager() {
}

phdFilterManager::~phdFilterManager() {
	freeItems();
}

void phdFilterManager::freeItems() {
	for (vector<phdFilter*>::iterator i = items.begin(); i != items.end(); i++) {
		delete(*i); (*i) = NULL;
	}
	items.clear();
}

void phdFilterManager::clear() {
	items.clear();
}

phdFilter * phdFilterManager::registerFilter(phdFilter * _item) {
	int _index = indexOf(_item);
	if(_index == -1) {
		phdFilter * _filter = findByFileName(_item->getFileName());
		if(_filter != NULL) return _filter;
		items.push_back(_item);
		return _item;
	}
	return items[_index];
}

phdFilter * phdFilterManager::getFilter(string _className) {
	return findByClassName(_className);
}

phdFilter * phdFilterManager::getFilter(int _index) {
	if(_index > -1 && _index < items.size()) {
		return items[_index];
	}
	return NULL;
}

int phdFilterManager::indexOf(phdFilter * _item) {
	if(_item == NULL) return -1;
	for(int i = 0; i < items.size(); i++) {
		if(items[i] == _item) return i;
	}
	return -1;
}

phdFilter * phdFilterManager::findByClassName(string _className) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i]->getClassName() == _className) return items[i];
	}
	return NULL;
}


phdFilter * phdFilterManager::findByFileName(string _fileName) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i]->getFileName() == _fileName) return items[i];
	}
	return NULL;
}
