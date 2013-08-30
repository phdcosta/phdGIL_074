#pragma once

#include "ofMain.h"

#include "phdDesigner.h"
#include "phdDesignerFbo.h"
#include "phdWarpLinkSchema.h"

//--------------------------------------------------------------------------------------------------------------
class phdVisualFile {
public:
	vector<phdWarpLinkSchema*> linkSchemas; // links inputs and outputs
	vector<phdDesigner*> designers;			// controls edition
	phdDesigner masterOutput;				// output
	phdWarpLinkSchema * curLinkSchema;		// curLinkSchema;

	ofPolyline screenLinkSchemaPos;

	phdVisualFile();
	~phdVisualFile();
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
	void saveToFile(string _fileName);
	void loadFromFile(string _fileName);
};
