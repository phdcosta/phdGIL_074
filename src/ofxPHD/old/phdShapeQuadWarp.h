#pragma once

#include "ofMain.h"
#include "phdShapeBase.h"

//-----------------------------------------------------------------------------------------------------------
enum phdWarpFaceType { wftQuadDLD, wftQuadDLU, wftTriTL, wftTriTR, wftTriBR, wftTriBL };

//--------------------------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------------------------
class phdShapeQuadWarp : public phdShapeBase {
public:
	phdWarpFaceType faceType;
	phdShapeQuadWarp();
	virtual void setVertex(int _index, double _x, double _y);
	virtual void update();
	virtual void drawEdgeInfo(ofPolyline & _coords);
	virtual void draw(ofMatrix4x4 & _mat);
};
