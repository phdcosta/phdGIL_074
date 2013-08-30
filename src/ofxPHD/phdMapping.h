#pragma once

#include "ofMain.h"

#include "phdPopupMenu.h"
#include "phdDesigner.h"
#include "phdControlManager.h"

#include "phdGenerator.h"
#include "phdGenFbo.h"
#include "phdGenAura.h"

#include "phdClipTrack.h"
#include "phdProjector.h"

#include "phdGrabberBase.h"
#include "phdKinectGrabber.h"

#include "phdFilter.h"

class phdMappingControl;

//--------------------------------------------------------------------------------------------------------------
class phdMapping {
public:
	vector<phdWarpLinkSchema*> linkSchemas; // links inputs and outputs
	vector<phdDesigner*> designers;			// designer controls
	vector<phdControlBase*> clips;			// clip controls
	vector<phdControlBase*> projectors;		// output to projectors
	phdDesigner * masterOutput;				// designer control for output
	phdWarpLinkSchema * curLinkSchema;		// curLinkSchema;

	ofPolyline screenLinkSchemaPos;
	bool showLinks;

	string fileName;
	phdMappingControl * owner;

	phdGrabberManager grabberManager;		// manage all grabbers
	phdGeneratorManager generatorManager;	// manage all generators
	phdFilterManager filterManager;			// manage all filters

	phdMapping();
	~phdMapping();
	void freeItems();
	int indexOfLinkSchema(phdWarpLinkSchema * _schema);
	phdWarpLinkSchema * findLinkSchemaByName(string _name);
	phdWarpLinkSchema * addLinkSchema(phdWarpLinkSchema * _schema);
	void updateScreenLinkSchemaPos();
	void setCurLinkSchema(phdWarpLinkSchema * _value);
	void setCurSelDesigner(phdDesigner * _dsgn);
	int indexOfDesigner(phdDesigner * _designer);
	phdDesigner * findDesignerByQuadWarp(phdShapeQuadWarp * _warp);
	phdDesigner * findDesignerByName(string _name);
	phdDesigner * findDesignerSelected();
	phdDesigner * addDesigner(phdDesigner * _designer);
	int indexOfClipControl(phdControlBase * _clip);
	void saveToFile(string _fileName);
	void loadFromFile(string _fileName);
};

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
enum phdMappingHitTestInfoType { mhtInside, mhtTitleBar, mhtHotspot, mhtSchema, mhtPopup, mhtOutside };

class phdMappingHitTestInfo {
public:
	phdMappingHitTestInfoType infoType;
	float mouseX, mouseY;
	int hotspotIndex, schemaIndex, popupIndex, selectedSchemaIndex;
};

static const int nLabels = 4;
static char * hotLabels[nLabels] = { "Load Mapping...", "Save Mapping...", "New Designer...", "New Track..." };

//--------------------------------------------------------------------------------------------------------------
class phdMappingControl : public phdControlBase {
protected:
	phdMappingHitTestInfo hitTestInfo;
	phdHotspotArea hotTitleBar;
	vector<phdHotspotArea> hotspots;
	vector<phdHotspotArea> hotSchemas;
	vector<string> strSchemas;

	void updateHotspotsArea();

public:
	phdMapping mapping;
	phdPopupMenuList popups;

	phdMappingControl();
	~phdMappingControl();

	string getClassName() { return "PHDMAPPINGCONTROL"; }

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
	void loadMappingFile(string _fileName);
};
