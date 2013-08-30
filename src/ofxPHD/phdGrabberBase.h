#pragma once

#include "ofMain.h"

#include "phdGenerator.h"
#include "phdWarper.h"
#include "phdPs3Eye.h"

//--------------------------------------------------------------------------------------------------
class phdGrabberManager;

//--------------------------------------------------------------------------------------------------
class phdGrabberBase : public phdGenerator {
protected:
	float internalWidth, internalHeight;
	phdGrabberManager * manager;
	int lastUpdatedFrame;
	phdWarper inputWarper;	// warp input coordinates to normalized space

public:
	phdGrabberBase() { 	inputWarper.setup(320,240,320.0,240.0); }
	~phdGrabberBase() { }

	virtual string getClassName() { return "PHDGRABBERBASE"; }

	virtual bool isExclusive() { return false; } // grabber are used by many clips so its can't be deleted in clip.setGenerator
	virtual phdWarper * getWarper() { return &inputWarper; }

	virtual void configureGrabber() { };
	virtual bool getIsInitialized() { return false; }

	virtual void setWidth(float _width) { internalWidth = _width; configureGrabber(); }
	virtual float getWidth() { return internalWidth; }

	virtual void setHeight(float _height) { internalHeight = _height; configureGrabber(); }
	virtual float getHeight() { return internalHeight; }

	inline virtual int genImagesCount() { return 0; }
	inline virtual int genMasksCount() { return 0; }
	inline virtual bool genData() { return false; }

	phdGrabberManager * getManager() { return manager; }
	void setManager(phdGrabberManager * _manager) { manager = _manager; }

	virtual void setup() { }
	virtual void update(phdGenParams * _params = NULL) { }
	virtual void draw(phdGenParams * _params = NULL) { }
	virtual void draw(float x, float y, float w, float h, phdGenParams * _params = NULL) { }

	string getSetupString();

	virtual void getNumericData(vector<float> & _results, vector<string> & _names) { }
	virtual void getStringData(vector<string> & _results, vector<string> & _names) { }
};

//--------------------------------------------------------------------------------------------------------------
enum phdGrabberManagerHitTestType { gmhtWarpInputVertex, gmhtOutside };

//--------------------------------------------------------------------------------------------------------------
class phdGrabberHitTestInfo {
public:
	phdGrabberManagerHitTestType infoType;
	int vertexIndex;
	bool dragging;
	float clickX, clickY;

	phdGrabberHitTestInfo() {
		infoType = gmhtOutside;
		vertexIndex = -1;
		dragging = false;
		clickX = clickY = -1;
	}
};

//--------------------------------------------------------------------------------------------------------------
class phdGrabberManager {
protected:
	vector<phdGrabberBase*> items;
	int curGrabberIndex;
	phdGrabberBase * curGrabber;
	phdGrabberHitTestInfo hitTestInfo;
	bool usingEvents;

public:
	phdGrabberManager();
	~phdGrabberManager();

	phdGrabberBase * phdGrabberManager::operator[](unsigned i) { return items[i]; }
	int phdGrabberManager::size() { return items.size(); }

	void freeItems();
	void clear();

	phdGrabberBase * registerGrabber(phdGrabberBase * _item);
	phdGrabberBase * getGrabber(string _className);
	phdGrabberBase * getGrabber(int _index);

	int indexOf(phdGrabberBase * _item);
	phdGrabberBase * findByClassName(string _className);

	void setCurGrabber(phdGrabberBase * _curGrabber);
	phdGrabberBase * getCurGrabber();
	void setCurGrabberIndex(int _index);
	int getCurGrabberIndex();
	void draw();
	bool updateHitTestInfo(float _x, float _y);
	void mouseMoved(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	void enableEvents();
	void disableEvents();
};

//--------------------------------------------------------------------------------------------------
class phdMouseGrabber : public phdGrabberBase {
protected:
	float x, y;
	int button;
	bool usingEvents;

public:
	phdMouseGrabber();
	~phdMouseGrabber();

	string getClassName() { return "PHDMOUSEGRABBER"; }

	bool getIsInitialized() { return true; }
	inline bool genData() { return true; }

	void setup();
	void update(phdGenParams * _params = NULL);
	void draw(phdGenParams * _params = NULL);
	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL);

	void getNumericData(vector<float> & _results, vector<string> & _names);
	void getStringData(vector<string> & _results, vector<string> & _names);

	void mouseMoved(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	void enableEvents();
	void disableEvents();
};

//--------------------------------------------------------------------------------------------------
class phdKeyGrabber : public phdGrabberBase {
protected:
	bool usingEvents;

public:
	phdKeyGrabber();
	~phdKeyGrabber();

	string getClassName() { return "PHDKEYGRABBER"; }

	bool getIsInitialized() { return true; }
	inline bool genData() { return true; }

	void setup();
	void update(phdGenParams * _params = NULL);
	void draw(phdGenParams * _params = NULL);
	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL);

	void getNumericData(vector<float> & _results, vector<string> & _names);
	void getStringData(vector<string> & _results, vector<string> & _names);

	void keyPressed(ofKeyEventArgs& eventArgs);
	void keyReleased(ofKeyEventArgs& eventArgs);
	void enableEvents();
	void disableEvents();
};

//--------------------------------------------------------------------------------------------------------------
class phdPS3EyeGrabber : public phdGrabberBase {
protected:
	phdPs3Eye cam;
	bool hasCam;
	bool bIsInitialized;

public:
	phdPS3EyeGrabber() {
		bIsInitialized = false;
		hasCam = false;
		internalWidth = 320;
		internalHeight = 240;
	}

	~phdPS3EyeGrabber() { };

	string getClassName() { return "PHDPS3EYEGRABBER"; }

	void configureGrabber() {

		internalWidth = 320;
		internalHeight = 240;

		if(!bIsInitialized) {
			hasCam = cam.setup(internalWidth, internalHeight, 60, true);
		}

		bIsInitialized = true;
	};

	bool getIsInitialized() { return hasCam && bIsInitialized; }

	void setWidth(float _width) { internalWidth = _width; configureGrabber(); }
	float getWidth() { return internalWidth; }

	void setHeight(float _height) { internalHeight = _height; configureGrabber(); }
	float getHeight() { return internalHeight; }

	inline int genImagesCount() { return 1; }
	inline int genMasksCount() { return 1; }
	inline bool genData() { return false; }

	void setup() {
		setWidth(320);
		setHeight(240);
	}

	void update(phdGenParams * _params = NULL) {
		int _frame = ofGetFrameNum();
		if(bIsInitialized && hasCam && _frame != lastUpdatedFrame) {
			cam.update();
			lastUpdatedFrame = _frame;
		}
	}

	void draw(phdGenParams * _params = NULL) {
		draw( 0, 0, getWidth(), getHeight(), _params);
	}

	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL) {
		if(hasCam && bIsInitialized) {
			cam.draw(x,y,w,h);
		}
	}

	void setSetupString(string _setupString) {
	}

	string getSetupString() {
		return "GRABBER|" + getClassName() + "|";
	}

	void getNumericData(vector<float> & _results, vector<string> & _names) { }
	void getStringData(vector<string> & _results, vector<string> & _names) { }
};

//--------------------------------------------------------------------------------------------------------------
class phdVideoGrabber : public phdGrabberBase {
protected:
	ofVideoGrabber videoGrabber;
	bool hasCam;
	bool bIsInitialized;

public:
	phdVideoGrabber() {
		bIsInitialized = false;
		hasCam = false;
		internalWidth = 320;
		internalHeight = 240;
	}

	~phdVideoGrabber() { };

	string getClassName() { return "PHDVIDEOGRABBER"; }

	void configureGrabber() {

		internalWidth = 320;
		internalHeight = 240;

		if(!bIsInitialized) {
			videoGrabber.initGrabber(internalWidth, internalHeight, true);
			hasCam = videoGrabber.isInitialized();
		}

		bIsInitialized = true;
	};

	bool getIsInitialized() { return hasCam && bIsInitialized; }

	void setWidth(float _width) { internalWidth = _width; configureGrabber(); }
	float getWidth() { return internalWidth; }

	void setHeight(float _height) { internalHeight = _height; configureGrabber(); }
	float getHeight() { return internalHeight; }

	inline int genImagesCount() { return 1; }
	inline int genMasksCount() { return 1; }
	inline bool genData() { return false; }

	void setup() {
		setWidth(320);
		setHeight(240);
	}

	void update(phdGenParams * _params = NULL) {
		int _frame = ofGetFrameNum();
		if(bIsInitialized && hasCam && _frame != lastUpdatedFrame) {
			videoGrabber.update();
			if(videoGrabber.isFrameNew()) lastUpdatedFrame = _frame;
		}
	}

	void draw(phdGenParams * _params = NULL) {
		draw( 0, 0, getWidth(), getHeight(), _params);
	}

	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL) {
		if(hasCam && bIsInitialized) {
			videoGrabber.draw(x,y,w,h);
		}
	}

	void setSetupString(string _setupString) {
	}

	string getSetupString() {
		return "GRABBER|" + getClassName() + "|";
	}

	void getNumericData(vector<float> & _results, vector<string> & _names) { }
	void getStringData(vector<string> & _results, vector<string> & _names) { }
};
