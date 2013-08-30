#pragma once

#include "ofMain.h"

#include "phdWarper.h"
#include "phdShapeQuadWarp.h"
#include "phdDesigner.h"

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
	//phdWarpLink(phdShapeQuadWarp * _source, phdShapeQuadWarp * _target);
	phdWarpLink(phdShapeQuadWarp * _source, phdDesigner * _dsgnIn, phdShapeQuadWarp * _target, phdDesigner * _dsgnOut);
	~phdWarpLink();
	void updateWarper(double _srcW, double _srcH, double _dstW, double _dstH, bool _scale);
};
