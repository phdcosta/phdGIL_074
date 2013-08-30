#pragma once

#include "ofMain.h"

#include "phdDesigner.h"
#include "phdGenBase.h"
#include "phdPopupMenu.h"
#include "phdViewport.h"
#include "phdWarpShapeSchema.h"

//--------------------------------------------------------------------------------------------------------------
class phdDesigner;

//--------------------------------------------------------------------------------------------------------------
enum phdDesignerHitTestInfoType { ghtOutside, ghtInside, ghtMenu, ghtDragControl, ghtVerticalScroll, ghtHorizontalScroll, ghtResize, ghtCommand };

//--------------------------------------------------------------------------------------------------------------
class phdDesignerHitTestInfo {
public:
	phdDesignerHitTestInfoType infoType;
	int command;
	string commands;
	vector<string> labels;
	float mouseX, mouseY;
	float verScrollX, verScrollY;
	float horScrollX, horScrollY;

	phdDesignerHitTestInfo() {
		infoType = ghtOutside;
		mouseX = mouseY = verScrollX = verScrollY = horScrollX = horScrollY = -1;
		command = -1;
		commands = "+-#*=";
		labels.push_back("Aproxima (zoom in)");
		labels.push_back("Afasta (zoom out)");
		labels.push_back("Visualiza tudo (fit all)");
		labels.push_back("Inclui nova fatia (add new slice)");
		labels.push_back("Muda tipo de fatia (changes slice type)");
		//labels.push_back("Exclui fatia selecionada (del selected slice)");
	}

	void setup(string & _commands, vector<string> & _labels) {
		commands = _commands;
		labels = _labels;
	}
};

//--------------------------------------------------------------------------------------------------------------
enum phdDesignerEventType { pdeOnClickTitle, pdeOnSelect };

class phdDesignerEventData {
public:
	phdDesignerEventType eventType;
	phdDesigner * designer;
};

//--------------------------------------------------------------------------------------------------------------
class phdDesigner {
public:
	phdDesignerHitTestInfo hitTestInfo;
	phdViewport viewport;
	phdWarpShapeSchema schema;
	phdPopupMenu * popupMenu;

	bool usingEvents;
	bool showSchema;
	bool selected;

	bool autoplay;
	bool useFade;

	string name;

	phdGenBase * visual;

	phdGenBase * lastVisual;

	phdDesigner();
	phdDesigner(float _x, float _y, float _viewWidth, float _viewHeight, ofFbo * _canvas, float _worldWidth, float _worldHeight);
	~phdDesigner();

	virtual void setup(float _x, float _y, float _viewWidth, float _viewHeight, ofFbo * _canvas, float _worldWidth, float _worldHeight);

	virtual void setCanvasPtr(ofFbo * _canvas);
	virtual ofFbo * getCanvasPtr();

	void setVisual(phdGenBase * _visual, bool _useFade);

	phdShapeQuadWarp * addShape(string _name);
	void begin();
	void end();
	void draw();
	void saveToStrings(vector<string> & _strings, string _title);
	void updateHitTestInfo(float _x, float _y);
	void mouseMoved(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	void enableEvents();
	void disableEvents();
};
