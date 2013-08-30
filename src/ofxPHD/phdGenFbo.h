#pragma once

#include "ofMain.h"

#include "phdGenerator.h"

//--------------------------------------------------------------------------------------------------------------
class phdGenFbo : public phdGenerator {
protected:
	float internalWidth, internalHeight;
	ofFbo internalCanvas;
	bool allocated;
	void allocateCanvas();

public:
	phdGenFbo() {
		phdGenerator::phdGenerator();
		setup();
	}

	void deallocate() {
		stop();
	}

	string getClassName() { return "PHDGENFBO"; }
	phdGenerator * createInstanceSameClass() { return new phdGenFbo(); }

	ofFbo * getCanvasPtr() { return &internalCanvas; }

	void setDimension(float _width, float _height) { 
		if(internalWidth != _width || internalHeight != _height) {
			internalWidth = _width;
			internalHeight = _height;
			allocateCanvas();
		}
	}

	void setWidth(float _width) { internalWidth = _width; allocateCanvas(); }
	float getWidth() { return internalCanvas.getWidth(); }

	void setHeight(float _height) { internalHeight = _height; allocateCanvas(); }
	float getHeight() { return internalCanvas.getHeight(); }

	void setup();
	void setupFromFile(string _fileName);
	void update(phdGenParams * _params = NULL);
	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL);

	string getSetupString();
};

//--------------------------------------------------------------------------------------------------------------
class phdGenFboShader : public phdGenFbo {
protected:
	ofShader shader;
	string fileName;

public:
	phdGenFboShader() {
		phdGenFbo::phdGenFbo();
		setup();
	}

	void deallocate() {
		shader.unload();
	}

	string getClassName() { return "PHDGENFBOSHADER"; }
	phdGenerator * createInstanceSameClass() { return new phdGenFboShader(); }

	bool isFileBase() { return true; }

	void setup();
	void setupFromFile(string _fileName);
	void update(phdGenParams * _params = NULL);
	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL);

	void setSetupString(string _setupString);
	string getSetupString();
};

