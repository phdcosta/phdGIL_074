#pragma once

#include "ofMain.h"
#include "phdGeom.h"

enum phdHitTestInfoType { htiNone, htiGimbal, htiVertex, htiEdge, htiInside };
enum phdGeometricOperationType { goNone, goWidth, goRotate, goCenter, goHeight, goVertex, goEdge, goInside };
enum phdShapeEditingModeType { semNone, semGimbal, semVertex };

//--------------------------------------------------------------------------------------------------------------
class phdHitTestInfo {
public:
	phdGeometricOperationType operation;
	phdHitTestInfoType info;
	int index;

	phdHitTestInfo();
	phdHitTestInfo(phdHitTestInfoType _info, int _index, phdGeometricOperationType _oper);
	void set(phdHitTestInfoType _info, int _index, phdGeometricOperationType _oper);
};
