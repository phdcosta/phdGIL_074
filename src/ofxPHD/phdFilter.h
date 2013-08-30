#pragma once

#include "ofMain.h"

#include "phdDefinitions.h"
#include "phdGenParams.h"

//--------------------------------------------------------------------------------------------------------------
class phdFilter {
protected:
	float internalWidth, internalHeight;
	ofFbo internalCanvas;
	bool allocated;
	void allocateCanvas();

	ofShader shader;
	string fileName;

public:
	phdFilter();

	string getClassName() { return "PHDFILTER"; }
	string getFileName() { return fileName; }

	ofFbo * getCanvasPtr() { return &internalCanvas; }

	void setWidth(float _width) { internalWidth = _width; allocateCanvas(); }
	float getWidth() { return internalWidth; }

	void setHeight(float _height) { internalHeight = _height; allocateCanvas(); }
	float getHeight() { return internalHeight; }

	void setup();
	void setupFromFile(string _fileName);
	void update(phdGenParams * _params = NULL);
	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL);
};

//--------------------------------------------------------------------------------------------------------------
class phdFilterList {
protected:
	vector<phdFilter*> items;
	bool autoFreeItems;

public:
	phdFilterList();
	~phdFilterList();

	phdFilter * phdFilterList::operator[](unsigned i) { return items[i]; }
	int phdFilterList::size() { return items.size(); }

	void setAutoFreeItems(bool _auto) { autoFreeItems = _auto; }
	bool getAutoFreeItems() { return autoFreeItems; }

	void freeItems();
	void clear();
	int indexOf(phdFilter * _item);
	phdFilter * addItem(phdFilter * _item);
	void delItem(phdFilter * _item);
	void delItem(int _index);

	phdFilter * pushItem(phdFilter * _item);
	void popItem(phdFilter * _item);
	void popItem(int _index);
};

//--------------------------------------------------------------------------------------------------------------
class phdFilterManager {
protected:
	vector<phdFilter*> items;

public:
	phdFilterManager();
	~phdFilterManager();

	phdFilter * phdFilterManager::operator[](unsigned i) { return items[i]; }
	int phdFilterManager::size() { return items.size(); }

	void freeItems();
	void clear();

	phdFilter * registerFilter(phdFilter * _item);
	phdFilter * getFilter(string _className);
	phdFilter * getFilter(int _index);

	int indexOf(phdFilter * _item);
	phdFilter * findByClassName(string _className);
	phdFilter * findByFileName(string _fileName);
};
