#include "phdShapeHitTestInfo.h"

//--------------------------------------------------------------------------------------------------------------
phdShapeHitTestInfo::phdShapeHitTestInfo() {
	info = htiNone; index = -1; operation = goNone;
}

phdShapeHitTestInfo::phdShapeHitTestInfo(phdShapeHitTestInfoType _info, int _index, phdGeometricOperationType _oper) {
	info = _info;
	index = _index;
	operation = _oper;
}

void phdShapeHitTestInfo::set(phdShapeHitTestInfoType _info, int _index, phdGeometricOperationType _oper) {
	info = _info;
	index = _index;
	operation = _oper;
}
