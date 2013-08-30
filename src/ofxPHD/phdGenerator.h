#pragma once

#include "ofMain.h"

#include "phdDefinitions.h"
#include "phdGenParams.h"
#include "phdWarper.h"

//--------------------------------------------------------------------------------------------------------------
class phdGeneratorManager;

//--------------------------------------------------------------------------------------------------------------
class phdGenerator {
protected:
	float width;
	float height;
	bool isPlaying;
	bool needsUpdate;

	float internalWidth, internalHeight;
	phdGeneratorManager * manager;

public:
	phdGenerator();

	virtual void deallocate();

	virtual string getClassName() { return "PHDGENERATOR"; }

	virtual phdGenerator * createInstanceSameClass() { return new phdGenerator(); }

	virtual bool isExclusive() { return true; } // exclusive generators belongs to one clip (1 to 1)
	virtual bool isFileBase() { return false; }
	virtual phdWarper * getWarper() { return NULL; }

	virtual ofFbo * getCanvasPtr() { return NULL; }

	virtual void setDimension(float _width, float _height) { width = internalWidth = _width; height = internalHeight = _height; }
	virtual void setWidth(float _width) { width = internalWidth = _width; }
	virtual float getWidth() { return internalWidth; }

	virtual void setHeight(float _height) { height = internalHeight = _height; }
	virtual float getHeight() { return internalHeight; }

	virtual void markToUpdate() { needsUpdate  = true;}

	virtual void play();
	virtual void pause();
	virtual void stop();

	phdGeneratorManager * getManager() { return manager; }
	void setManager(phdGeneratorManager * _manager) { manager = _manager; }

	virtual void setup();
	virtual void update(phdGenParams * _params = NULL);
	virtual void draw(phdGenParams * _params = NULL);
	virtual void draw(float x, float y, float w, float h, phdGenParams * _params = NULL);

	virtual void setupFromFile(string _fileName);
	virtual void saveToStrings(vector<string> & _strings);

	virtual void setSetupString(string _setupString);
	virtual string getSetupString();

	virtual string getProperty(string _name) { return ""; }
};

//--------------------------------------------------------------------------------------------------------------
class phdGeneratorManager {
protected:
	vector<phdGenerator*> items;

public:
	phdGeneratorManager();
	~phdGeneratorManager();

	phdGenerator * phdGeneratorManager::operator[](unsigned i) { return items[i]; }
	int phdGeneratorManager::size() { return items.size(); }

	void freeItems();
	void clear();

	phdGenerator * registerGenerator(phdGenerator * _item);
	phdGenerator * getGenerator(string _className);
	phdGenerator * getGenerator(int _index);

	int indexOf(phdGenerator * _item);
	phdGenerator * findByClassName(string _className);
};

//--------------------------------------------------------------------------------------------------------------
class phdGeneratorList {
protected:
	vector<phdGenerator*> items;

public:
	phdGeneratorList();
	~phdGeneratorList();

	phdGenerator * phdGeneratorList::operator[](unsigned i) { return items[i]; }
	int phdGeneratorList::size() { return items.size(); }

	void freeItems();
	void clear();
	int indexOf(phdGenerator * _item);
	phdGenerator * addItem(phdGenerator * _item);
};

//--------------------------------------------------------------------------------------------------------------
class phdGenRectangle : public phdGenerator {
protected:

public:
	phdGenRectangle() {
		phdGenerator::phdGenerator();
		setup();
	}

	string getClassName() { return "PHDGENRECTANGLE"; }
	phdGenerator * createInstanceSameClass() { return new phdGenRectangle(); }

	void setup() {
	}

	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL) {
		ofPushStyle();
		ofFill();
		if(_params != NULL) {
			ofSetColor((int)_params->mouse[0]/_params->resolution[0]*255.0, (int)_params->mouse[1]/_params->resolution[1]*255.0, ofRandom(192,210));
		} else {
			ofSetColor(ofRandom(128,160), ofRandom(120,160), ofRandom(192,210));
		}
		ofRect(x,y,w,h);
		ofNoFill();
		ofPopStyle();
	}

	void setSetupString(string _setupString) {
	}

	string getSetupString() {
		return "GENERATOR|" + getClassName() + "|";
	}
};

//--------------------------------------------------------------------------------------------------------------
class phdGenEllipse : public phdGenerator {
protected:

public:
	phdGenEllipse() {
		phdGenerator::phdGenerator();
		setup();
	}

	string getClassName() { return "PHDGENELLIPSE"; }
	phdGenerator * createInstanceSameClass() { return new phdGenEllipse(); }

	void setup() {
	}

	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL) {
		if(_params != NULL) {
			ofEllipse(_params->mouse[0], _params->mouse[1], w/4.0, h/4.0);
		} else {
			ofEllipse(w/2.0, h/2.0, w/4.0, h/4.0);
		}
	}

	void setSetupString(string _setupString) {
	}

	string getSetupString() {
		return "GENERATOR|" + getClassName() + "|";
	}
};

//--------------------------------------------------------------------------------------------------------------
class phdGenImage : public phdGenerator {
protected:
	ofImage image;
	string fileName;

public:
	phdGenImage() {
		phdGenerator::phdGenerator();
		setup();
	}

	void deallocate() {
		image.clear();
	}

	string getClassName() { return "PHDGENIMAGE"; }
	phdGenerator * createInstanceSameClass() { return new phdGenImage(); }

	bool isFileBase() { return true; }

	float getWidth() { return image.getWidth(); }
	float getHeight() { return image.getHeight(); }

	void setup() {
	}

	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL) {
		image.draw(x,y,w,h);
	}

	void setupFromFile(string _fileName) {
		image.loadImage(ofToDataPath("images/" + _fileName, true));
		fileName = _fileName;
	}

	void setSetupString(string _setupString) {

		vector<string> _items = ofSplitString(ofToUpper(_setupString), "|", true, true);

		int _size = _items.size() - 1;
		for(int i = 0; i < _size; i+=2) { // works in "NAME|VALUE|"
			if(_items[i] == "FILENAME" && i < _size) {
				if(fileName != _items[i+1]) {
					fileName = _items[i+1];
					image.loadImage(ofToDataPath("images/" + fileName, true));
				}
			}
		}
	}

	string getSetupString() {
		
		string _result = "GENERATOR|" + getClassName() + "|";

		_result += "FILENAME|" + fileName + "|";

		return _result;
	}
};

//--------------------------------------------------------------------------------------------------------------
class phdGenVideoPlayer : public phdGenerator {
protected:
	ofVideoPlayer videoPlayer;
	string fileName;
	bool playFromBegin;

public:
	phdGenVideoPlayer() {
		phdGenerator::phdGenerator();
		setup();
	}

	void deallocate() {
		videoPlayer.closeMovie();
	}

	string getClassName() { return "PHDGENVIDEOPLAYER"; }
	phdGenerator * createInstanceSameClass() { return new phdGenVideoPlayer(); }

	bool isFileBase() { return true; }

	void play() {
		if(playFromBegin) videoPlayer.setPosition(0.0);
		videoPlayer.setPaused(false);
		videoPlayer.play();
		isPlaying = true;
	}
	void pause() {
		videoPlayer.setPaused(true);
		isPlaying = false;
	}
	void stop() {
		videoPlayer.stop();
		isPlaying = false;
	}

	float getWidth() { return videoPlayer.getWidth(); }
	float getHeight() { return videoPlayer.getHeight(); }

	void setup() {
		fileName = "";
		playFromBegin = true;
		videoPlayer.setLoopState(OF_LOOP_NORMAL);
	}

	void update(phdGenParams * _params = NULL) {
		if(_params != NULL) videoPlayer.setVolume(_params->faderPos);
		videoPlayer.update();
	}

	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL) {
		videoPlayer.draw(x,y,w,h);
	}

	void setupFromFile(string _fileName) {
		videoPlayer.loadMovie(ofToDataPath("videos/" + _fileName, true));
		fileName = _fileName;
		videoPlayer.play();
		videoPlayer.setPaused(true);
	}

	void setSetupString(string _setupString) {

		vector<string> _items = ofSplitString(ofToUpper(_setupString), "|", true, true);

		int _size = _items.size() - 1;
		for(int i = 0; i < _size; i+=2) { // works in "NAME|VALUE|"
			if(_items[i] == "FILENAME" && i < _size) {
				if(fileName != _items[i+1]) {
					setupFromFile(_items[i+1]);
				}
			} else if(_items[i] == "LOOPSTATE" && i < _size) {
				if(_items[i+1] == "NORMAL") {
					videoPlayer.setLoopState(OF_LOOP_NORMAL);
				} else if(_items[i+1] == "PALINDROME") {
					videoPlayer.setLoopState(OF_LOOP_PALINDROME);
				} else {
					videoPlayer.setLoopState(OF_LOOP_NONE);
				}
			} else if(_items[i] == "PLAYMODE" && i < _size) {
				if(_items[i+1] == "CURRENT") {
					playFromBegin = false;
				} else {
					playFromBegin = true;
				}
			}
		}
	}

	string getSetupString() {
		
		string _result = "GENERATOR|" + getClassName() + "|";

		_result += "FILENAME|" + fileName + "|";

		if(videoPlayer.getLoopState() == OF_LOOP_NORMAL) {
			_result += "LOOPSTATE|NORMAL|";
		} else if(videoPlayer.getLoopState() == OF_LOOP_PALINDROME) {
			_result += "LOOPSTATE|PALINDROME|";
		} else if(videoPlayer.getLoopState() == OF_LOOP_NONE) {
			_result += "LOOPSTATE|NONE|";
		}

		if(playFromBegin) _result += "PLAYMODE|BEGIN|"; else "PLAYMODE|CURRENT|";

		return _result;
	}

	virtual string getProperty(string _name) {
		if(_name == "LOOPSTATE") {
			if(videoPlayer.getLoopState() == OF_LOOP_NORMAL) {
				return "NORMAL";
			} else if(videoPlayer.getLoopState() == OF_LOOP_PALINDROME) {
				return "PALINDROME";
			} else if(videoPlayer.getLoopState() == OF_LOOP_NONE) {
				return "NONE";
			}
		}
		return "";
	}
};

//--------------------------------------------------------------------------------------------------------------
class phdGenSoundPlayer : public phdGenerator {
protected:
	ofSoundPlayer soundPlayer;
	string fileName;
	bool looping, playFromBegin;

public:
	phdGenSoundPlayer() {
		phdGenerator::phdGenerator();
		setup();
	}

	void deallocate() {
		soundPlayer.stop();
		soundPlayer.unloadSound();
	}

	string getClassName() { return "PHDGENSOUNDPLAYER"; }
	phdGenerator * createInstanceSameClass() { return new phdGenSoundPlayer(); }

	bool isFileBase() { return true; }

	void play() {
		if(playFromBegin) soundPlayer.setPosition(0.0);
		soundPlayer.setPaused(false);
		soundPlayer.play();
		isPlaying = true;
	}
	void pause() {
		soundPlayer.setPaused(true);
		isPlaying = false;
	}
	void stop() {
		soundPlayer.stop();
		isPlaying = false;
	}

	float getWidth() { return 160; }
	float getHeight() { return 120; }

	void setup() {
		fileName = "";
		looping = false;
		playFromBegin = true;
		soundPlayer.setLoop(looping);
	}

	void update(phdGenParams * _params = NULL) {
		if(_params != NULL) soundPlayer.setVolume(_params->faderPos);
	}

	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL) {
		ofLine(ofRandom(0,160), ofRandom(0,140),ofRandom(0,160), ofRandom(0,140));
	}

	void setupFromFile(string _fileName) {
		fileName = _fileName;
		soundPlayer.loadSound(ofToDataPath("sounds/" + _fileName, true));
		soundPlayer.play();
		soundPlayer.setPaused(true);
	}

	void setSetupString(string _setupString) {

		vector<string> _items = ofSplitString(ofToUpper(_setupString), "|", true, true);

		int _size = _items.size() - 1;
		for(int i = 0; i < _size; i+=2) { // works in "NAME|VALUE|"
			if(_items[i] == "FILENAME" && i < _size) {
				if(fileName != _items[i+1]) {
					setupFromFile(_items[i+1]);
				}
			} else if(_items[i] == "LOOP" && i < _size) {
				if(_items[i+1] == "TRUE") {
					looping = true;
				} else {
					looping = false;
				}
				soundPlayer.setLoop(looping);
			} else if(_items[i] == "PLAYMODE" && i < _size) {
				if(_items[i+1] == "CURRENT") {
					playFromBegin = false;
				} else {
					playFromBegin = true;
				}
			}
		}
	}

	string getSetupString() {
		
		string _result = "GENERATOR|" + getClassName() + "|";

		_result += "FILENAME|" + fileName + "|";

		if(looping) _result += "LOOP|TRUE|"; else "LOOP|FALSE|";

		if(playFromBegin) _result += "PLAYMODE|BEGIN|"; else _result += "PLAYMODE|CURRENT|";

		return _result;
	}
};
