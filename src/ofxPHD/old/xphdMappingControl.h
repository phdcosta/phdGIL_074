#pragma once

#include "ofMain.h"
#include "phdControlManager.h"
#include "phdMapping.h"

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
enum phdMappingHitTestInfoType { mhtInside, mhtTitleBar, mhtHotspot, mhtPopup, mhtOutside };

class phdMappingHitTestInfo {
public:
	phdMappingHitTestInfoType infoType;
	float mouseX, mouseY;
	int hotspotIndex, popupIndex;
};

static const int nLabels = 4;
static char * hotLabels[nLabels] = { "Load Mapping...", "Save Mapping...", "New Designer...", "New Schema..." };

//--------------------------------------------------------------------------------------------------------------
class phdMappingControl : public phdControlBase {
protected:
	phdMappingHitTestInfo hitTestInfo;
	phdHotspotArea hotTitleBar;
	vector<phdHotspotArea> hotspots;
	void updateHotspotsArea();

public:
	string mappingFileName;
	phdMapping mapping;

	phdMappingControl();
	~phdMappingControl();

	void setPosition(float _x, float _y);
	void setDimension(float _width, float _height);

	bool updateHitTestInfo(float _x, float _y);
	void update();
	void draw();

	void onMouseMoved(ofMouseEventArgs& args);
	void onMousePressed(ofMouseEventArgs& args);
	void onMouseDragged(ofMouseEventArgs& args);
	void onMouseReleased(ofMouseEventArgs& args);

	void onPopupClick(phdPopupMenuEventData &_eventData);
};