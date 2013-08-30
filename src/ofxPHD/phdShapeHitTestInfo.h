#pragma once

#include "ofMain.h"
#include "phdGeom.h"

enum phdShapeHitTestInfoType { htiNone, htiGimbal, htiVertex, htiEdge, htiInside };
enum phdGeometricOperationType { goNone, goWidth, goRotate, goCenter, goHeight, goVertex, goEdge, goInside };
enum phdShapeEditingModeType { semNone, semGimbal, semVertex };

//--------------------------------------------------------------------------------------------------------------
class phdShapeHitTestInfo {
public:
	phdGeometricOperationType operation;
	phdShapeHitTestInfoType info;
	int index;

	phdShapeHitTestInfo();
	phdShapeHitTestInfo(phdShapeHitTestInfoType _info, int _index, phdGeometricOperationType _oper);
	void set(phdShapeHitTestInfoType _info, int _index, phdGeometricOperationType _oper);
};
