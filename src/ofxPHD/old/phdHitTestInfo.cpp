#include "phdHitTestInfo.h"

//--------------------------------------------------------------------------------------------------------------
phdHitTestInfo::phdHitTestInfo() {
	info = htiNone; index = -1; operation = goNone;
}

phdHitTestInfo::phdHitTestInfo(phdHitTestInfoType _info, int _index, phdGeometricOperationType _oper) {
	info = _info;
	index = _index;
	operation = _oper;
}

void phdHitTestInfo::set(phdHitTestInfoType _info, int _index, phdGeometricOperationType _oper) {
	info = _info;
	index = _index;
	operation = _oper;
}
