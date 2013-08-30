#pragma once

#include "ofMain.h"

#include "phdWarper.h"
#include "phdControlManager.h"
#include "phdPopupMenu.h"

//--------------------------------------------------------------------------------------------------------------
enum phdProjectorWarpType { pwtInside, pwtEdge, pwtVertex, pwtOutside };

class phdWarpProjectorInfo {
public:
	phdProjectorWarpType infoType;
	float mouseX, mouseY;
	float mouseDownX, mouseDownY;
	bool dragging;
	int vertex;
};

//--------------------------------------------------------------------------------------------------------------
enum phdProjectorDrawMode { pdmWarper, pdmContents, pdmBoth };
//--------------------------------------------------------------------------------------------------------------
class phdProjectorControl : public phdControlBase {
protected:
	phdWarpProjectorInfo hitTestInfo;
	phdProjectorDrawMode drawMode;

public:
	phdWarper warper;

	phdProjectorControl();
	~phdProjectorControl();

	string getClassName() { return "PHDPROJECTOR"; }

	void setDrawMode(phdProjectorDrawMode _drawMode) { drawMode = _drawMode; };
	phdProjectorDrawMode getDrawMode() { return drawMode; }

	void setSource(float _x, float _y, float _w, float _h);

	bool updateHitTestInfo(float _x, float _y);
	void update();
	void draw();

	void onMouseMoved(ofMouseEventArgs& args);
	void onMousePressed(ofMouseEventArgs& args);
	void onMouseDragged(ofMouseEventArgs& args);
	void onMouseReleased(ofMouseEventArgs& args);

	void onPopupClick(phdPopupMenuEventData &_eventData);

	string getSetupString();
	void setupFromString(string _setupString);
	void saveToStrings(vector<string> & _strings, string _title);
};