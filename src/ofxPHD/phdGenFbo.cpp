#include "phdGenFbo.h"

//--------------------------------------------------------------------------------------------------

void phdGenFbo::allocateCanvas() {
	if(internalWidth > 0 && internalHeight > 0 && (internalWidth != getWidth() || internalHeight != getHeight())) {
		internalCanvas.allocate(internalWidth, internalHeight, GL_RGBA);
		allocated = true;
	}
}

//--------------------------------------------------------------------------------------------------
void phdGenFbo::setup() {
	allocated = false;
	setWidth(320);
	setHeight(240);
}

void phdGenFbo::setupFromFile(string _fileName) { }

void phdGenFbo::update(phdGenParams * _params) {
	if(allocated) {
		internalCanvas.begin();
			ofSetColor(0,0,0,0);
			ofRect(0, 0, getWidth(), getHeight());
			ofSetColor(255, 160, 90, 255);
			ofRect(getWidth()*0.1, getHeight()*0.25, getWidth()*0.75, getHeight()*0.90);
		internalCanvas.end();
	}
}

void phdGenFbo::draw(float x, float y, float w, float h, phdGenParams * _params) {
	if(allocated) {
		internalCanvas.draw(x,y,w,h);
	}
}

string phdGenFbo::getSetupString() {
		
	string _result = "GENERATOR|" + getClassName() + "|";

	return _result;
}
//--------------------------------------------------------------------------------------------------
void phdGenFboShader::setup() {
	phdGenFbo::setup();
	allocated = true;
}

void phdGenFboShader::setupFromFile(string _fileName) {
	shader.load("shaders/" + _fileName);
	fileName = _fileName;
}

void phdGenFboShader::update(phdGenParams * _params) {
	if(allocated && internalCanvas.isAllocated()) {
		internalCanvas.begin();
			shader.begin();

				if(_params != NULL) {
					_params->time = ofGetElapsedTimef();
					_params->resolution[0] = getWidth(); _params->resolution[1] = getHeight();
					//_params->mouse[0] = ofGetMouseX(); _params->mouse[1] = ofGetMouseY();
					_params->surfaceSize[0] = 1024; _params->surfaceSize[1] = 768;
					_params->clrBase[0] = 0.2; _params->clrBase[1] = 0.2; _params->clrBase[2] = 0.2; _params->clrBase[3] = 0.2;
					_params->clrLow[0] = 0.0; _params->clrLow[1] = 0.0; _params->clrLow[2] = 0.0; _params->clrLow[3] = 0.0;
					_params->clrHigh[0] = 1.0; _params->clrHigh[1] = 1.0; _params->clrHigh[2] = 1.0; _params->clrHigh[3] = 1.0;
					_params->params[0] = 1.0; _params->params[1] = 1.0; _params->params[2] = 1.0; _params->params[3] = 1.0;
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
					/*
					shader.setUniformTexture("tex0", (*tex0), 0 );
					shader.setUniform2f("size0", (*tex0).getWidth(), (*tex0).getHeight());
					shader.setUniformTexture("tex1", (*tex1), 1 );
					shader.setUniform2f("size1", (*tex1).getWidth(), (*tex1).getHeight());
					*/
				} else {
					shader.setUniform1f("time", ofGetElapsedTimef() );
					shader.setUniform2f("resolution", 320.0, 240.0 );
					shader.setUniform2f("mouse", ofGetMouseX(), ofGetMouseY());
					shader.setUniform2f("surfacesize", 320.0, 240.0 );
					shader.setUniform4f("clrBase", 0, 0, 0, 0 );
					shader.setUniform4f("clrLow", 0, 0, 0, 0 );
					shader.setUniform4f("clrHigh", 255, 255, 255, 255 );
					shader.setUniform4f("params", 1.0, 1.0, 1.0, 1.0 );
					shader.setUniform2f("pos", 0.5, 0.5 );
					shader.setUniform2f("scale", 1.0, 1.0 );
				}
			
				ofSetColor(0,0,0,0);
				ofRect(0, 0, getWidth(), getHeight());
			shader.end();
		internalCanvas.end();
	}
}

void phdGenFboShader::draw(float x, float y, float w, float h, phdGenParams * _params) {
	if(allocated) {
		internalCanvas.draw(x,y,w,h);
	}
}

void phdGenFboShader::setSetupString(string _setupString) {

	vector<string> _items = ofSplitString(ofToUpper(_setupString), "|", true, true);

	int _size = _items.size() - 1;
	for(int i = 0; i < _size; i+=2) { // works in "NAME|VALUE|"
		if(_items[i] == "FILENAME" && i < _size) {
			if(fileName != _items[i+1]) {
				setupFromFile(_items[i+1]);
			}
		}
	}
}

string phdGenFboShader::getSetupString() {
		
	string _result = "GENERATOR|" + getClassName() + "|";

	_result += "FILENAME|" + fileName + "|";

	return _result;
}
