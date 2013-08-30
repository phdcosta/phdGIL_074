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
	void setVertexPos(int _index, double _x, double _y);
	void update();
	void drawEdgeInfo(ofPolyline & _coords);
	void draw(ofMatrix4x4 & _mat);
};
