#pragma once

#include "ofMain.h"

#include "phdWarpLink.h"
#include "phdShapeQuadWarp.h"
#include "phdDesigner.h"

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
