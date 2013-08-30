#pragma once

#include "ofMain.h"

#include "phdControlManager.h"
#include "phdViewport.h"
#include "phdPopupMenu.h"
#include "phdWarpShapeSchema.h"
#include "phdClip.h"

#include "phdWarper.h"

//--------------------------------------------------------------------------------------------------------------
class phdDesigner;
class phdWarpLink;
class phdWarpLinkSchema;

//--------------------------------------------------------------------------------------------------------------
class phdDesignerHitTestInfo {
public:
	phdHitTestInfoType infoType;
	int command;
	string commands;
	vector<string> labels;
	float mouseX, mouseY;
	float verScrollX, verScrollY;
	float horScrollX, horScrollY;
	int linkIndex;

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
enum phdDesignerType { phdDesignerInput, phdDesignerOutput };

//--------------------------------------------------------------------------------------------------------------
class phdDesigner : public phdControlBase {
protected:
	phdDesignerHitTestInfo hitTestInfo;

	phdClip * inputClip;

	ofFbo internalCanvas;
	void allocateInternalCanvas(float _w, float _h);

public:
	phdViewport viewport;

	bool showSchema;

	phdDesignerType designerType;
	phdWarpLinkSchema * curLinkSchema;

	phdWarpShapeSchema schema;

	phdDesigner(ofFbo * _canvas = NULL);
	phdDesigner(float _x, float _y, float _viewWidth, float _viewHeight, ofFbo * _canvas, float _worldWidth, float _worldHeight);
	~phdDesigner();

	string getClassName() { return "PHDDESIGNER"; }

	void setup(float _x, float _y, float _viewWidth, float _viewHeight, ofFbo * _canvas, float _worldWidth, float _worldHeight);

	void setInputClip(phdClip * _inputClip) { inputClip = _inputClip; }
	phdClip * getInputClip() { return inputClip; }

	phdShapeQuadWarp * addShape(string _name);

	void begin();
	void end();

	void bind();
	void unbind();

	bool updateHitTestInfo(float _x, float _y);
	void update();
	void draw();
	void DrawDesignerBorder();
	void DrawWarpSchema(phdWarpLinkSchema * _schema, phdDesigner * _default = NULL);

	void saveToStrings(vector<string> & _strings, string _title);

	void onMouseMoved(ofMouseEventArgs& args);
	void onMousePressed(ofMouseEventArgs& args);
	void onMouseDragged(ofMouseEventArgs& args);
	void onMouseReleased(ofMouseEventArgs& args);

	void onPopupClick(phdPopupMenuEventData &_eventData);
};

//-----------------------------------------------------------------------------------------------------------
// links Texture to Model
//-----------------------------------------------------------------------------------------------------------
class phdWarpLink {
public:
	phdWarper warper;
	phdShapeQuadWarp * quadIn;
	phdDesigner * dsgnIn;
	phdShapeQuadWarp * quadOut;
	phdDesigner * dsgnOut;
	bool verMirror;
	bool horMirror;
	bool updated;

	phdWarpLink();
	phdWarpLink(phdShapeQuadWarp * _source, phdDesigner * _dsgnIn, phdShapeQuadWarp * _target, phdDesigner * _dsgnOut);
	~phdWarpLink();
	void updateWarper(double _srcW, double _srcH, double _dstW, double _dstH, bool _scale);
};

//-----------------------------------------------------------------------------------------------------------
// schema links of QuadIn and QuadOut
//-----------------------------------------------------------------------------------------------------------
class phdWarpLinkSchema {
public:
	vector<phdWarpLink*> links;
	string name;

	phdWarpLinkSchema();
	~phdWarpLinkSchema();
	void freeLinks();
	phdWarpLink * phdWarpLinkSchema::operator[] (int _index) { return links[_index]; }
	int size();
	void setup(string _name);
	void updateFlags();
	int indexOf(phdWarpLink * _link);
	int indexOf(phdShapeQuadWarp * _quadIn, phdShapeQuadWarp * _quadOut);
	phdWarpLink * getLinkFromQuadOut(phdShapeQuadWarp * _quadOut);
	phdWarpLink * getLinkFromQuadIn(phdShapeQuadWarp * _quadIn);
	phdWarpLink * addLink(phdShapeQuadWarp * _quadIn, phdShapeQuadWarp * _quadOut);
	phdWarpLink * addLink(phdShapeQuadWarp * _source, phdDesigner * _dsgnIn, phdShapeQuadWarp * _target, phdDesigner * _dsgnOut);
	void delLink(phdWarpLink * _link);
	void delLinksFromQuadIn(phdShapeQuadWarp * _quadIn);
	void delLinksFromQuadOut(phdShapeQuadWarp * _quadOut);
	void draw(ofMatrix4x4 & _srcMat, ofMatrix4x4 & _dstMat); // draw links
};
