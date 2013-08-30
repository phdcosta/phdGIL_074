#pragma once

#include "ofMain.h"

#include "phdClip.h"
#include "phdBlend.h"
#include "phdFilter.h"
#include "phdControlManager.h"
#include "phdDesigner.h"

//--------------------------------------------------------------------------------------------------------------
class phdClipTrack : public phdClip {
protected:
	phdClip * curClip;
	phdClip * oldClip;

public:
	phdClipList clips;
	phdBlend * blend;

	phdClipTrack();

	int getTrackCount() { return 1; }

	void fadeIn();
	void fadeOut();

	void play();
	void pause();
	void stop();

	void setup();
	void update();
	void draw(float _x, float _y, float _w, float _h);

	// ----------- class specific methods ---------------
	void setCurClip(int _index);
	int getCurClipIndex() {
		for(int i = 0; i < clips.size(); i++) {
			if(clips[i] == curClip) return i;
		}
		return -1;
	}

	void setCurClip(phdClip * _clip);
	phdClip * getCurClip() { return curClip; }
	void resetClips() { oldClip = NULL; curClip = NULL; }
};

//--------------------------------------------------------------------------------------------------------------
enum phdTrackHitTestInfoType { httOutput, httClip, httFilter, httCommand, httSlider, httTitleBar, httInputLink, httOutputLink, httLoopMode, httPopup, httOutside };

class phdTrackHitTestInfo {
public:
	phdTrackHitTestInfoType infoType;
	float mouseX, mouseY;
	float mouseDownX, mouseDownY;
	int clipIndex, filterIndex, linkIndex, popupIndex, commandIndex, loopIndex;
	phdClip * focusedClip;
	bool dragging;
	float paramX, paramY;
};

//--------------------------------------------------------------------------------------------------------------
class phdFilterHotspotArea {
public:
	phdHotspotArea hotspot;
	bool isActive;
	int order;
};

//--------------------------------------------------------------------------------------------------------------
class phdTrackControl : public phdControlBase {
protected:
	phdClipTrack clipTrack;
	phdTrackHitTestInfo hitTestInfo;
	phdHotspotArea hotSlider, hotTitleBar;
	vector<phdHotspotArea> commands;
	vector<phdHotspotArea> hotspots;
	vector<phdFilterHotspotArea> filterHotspots;

	vector<phdHotspotArea> loopSpots;

	phdPopupMenu * clipsPopupMenu;
	phdFilterManager * filterManager;

public:
	phdTrackControl();
	~phdTrackControl();

	string getClassName() { return "PHDTRACKCONTROL"; }

	void setPosition(float _x, float _y);
	void setDimension(float _width, float _height);

	void setFilterManager(phdFilterManager * _filterManager) { filterManager = _filterManager; }

	//void setClipTrack(phdClipTrack * _track);
	phdClipTrack * getClipTrack() { return &clipTrack; }

	virtual void setClipsPopupMenu(phdPopupMenu * _menu) { clipsPopupMenu = _menu; }
	virtual phdPopupMenu * getClipsPopupMenu() { return clipsPopupMenu; }

	void updateHotspotsArea();
	bool updateHitTestInfo(float _x, float _y);
	void update();
	void draw();

	void onMouseMoved(ofMouseEventArgs& args);
	void onMousePressed(ofMouseEventArgs& args);
	void onMouseDragged(ofMouseEventArgs& args);
	void onMouseReleased(ofMouseEventArgs& args);

	void onPopupClick(phdPopupMenuEventData &_eventData);
	void onClipsPopupClick(phdPopupMenuEventData &_eventData);

	string getSetupString();
	void saveToStrings(vector<string> & _strings, string _title);
};