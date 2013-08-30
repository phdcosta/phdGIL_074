#include "phdWarpLinkSchema.h"

//-----------------------------------------------------------------------------------------------------------
phdWarpLinkSchema::phdWarpLinkSchema() { }

phdWarpLinkSchema::~phdWarpLinkSchema() {
	freeLinks();
}

void phdWarpLinkSchema::freeLinks() {
	for (vector<phdWarpLink*>::iterator i = links.begin(); i != links.end(); i++) {
		(*i)->quadIn = NULL;	// dont delete shape
		(*i)->quadOut = NULL;	// dont delete shape
		delete(*i); // delete link
	}
	links.clear();
}

int phdWarpLinkSchema::size() {
	return links.size();
}

void phdWarpLinkSchema::setup(string _name) {
	name = _name;
}

void phdWarpLinkSchema::updateFlags() {
	for(int i = 0; i < links.size(); i++) {
		if(links[i]->quadIn != NULL) links[i]->updated = (links[i]->updated || links[i]->quadIn->updated);
		if(links[i]->quadOut != NULL) links[i]->updated = (links[i]->updated || links[i]->quadOut->updated);
	}
}

int phdWarpLinkSchema::indexOf(phdWarpLink * _link) {
	for(int i = 0; i < links.size(); i++) {
		if(_link->quadOut == links[i]->quadOut && _link->quadIn == links[i]->quadIn) return i;
	}
	return -1;
}

int phdWarpLinkSchema::indexOf(phdShapeQuadWarp * _quadIn, phdShapeQuadWarp * _quadOut) {
	for(int i = 0; i < links.size(); i++) {
		if(_quadOut == links[i]->quadOut && _quadIn == links[i]->quadIn) return i;
	}
	return -1;
}

phdWarpLink * phdWarpLinkSchema::getLinkFromQuadIn(phdShapeQuadWarp * _quadIn) {
	for(int i = 0; i < links.size(); i++) {
		if(_quadIn == links[i]->quadOut) return links[i];
	}
	return NULL;
}

phdWarpLink * phdWarpLinkSchema::getLinkFromQuadOut(phdShapeQuadWarp * _quadOut) {
	for(int i = 0; i < size(); i++) {
		if(_quadOut == links[i]->quadOut) return links[i];
	}
	return NULL;
}

/*phdWarpLink * phdWarpLinkSchema::addLink(phdShapeQuadWarp * _quadIn, phdShapeQuadWarp * _quadOut) {
	int _index = indexOf(_quadIn, _quadOut);
	if(_index == -1) {
		links.push_back(new phdWarpLink(_quadIn, _quadOut));
		return links[links.size()-1];
	}
	return links[_index];
}*/

phdWarpLink * phdWarpLinkSchema::addLink(phdShapeQuadWarp * _source, phdDesigner * _dsgnIn, phdShapeQuadWarp * _target, phdDesigner * _dsgnOut) {

	phdWarpLink * _result = getLinkFromQuadOut(_target); // keeps only one link with that quadOut

	if(_result == NULL) {

		_result = new phdWarpLink(_source, _dsgnIn, _target, _dsgnOut);
		
		_result->horMirror = false;
		_result->verMirror = false;

		links.push_back(_result);
		
		return links[links.size()-1];
	}
	return _result;
}

void phdWarpLinkSchema::delLink(phdWarpLink * _link) {
	int _index = indexOf(_link);
	if(_index != -1) {
		delete links[_index];
		links.erase(links.begin() + _index);
	}
}

void phdWarpLinkSchema::delLinksFromQuadIn(phdShapeQuadWarp * _quadIn) {
	for(int i = links.size()-1; i > -1; i--) {
		if(links[i]->quadIn == _quadIn) {
			delete links[i];
			links.erase(links.begin() + i);
		}
	}
}

void phdWarpLinkSchema::delLinksFromQuadOut(phdShapeQuadWarp * _quadOut) {
	for(int i = links.size()-1; i > -1; i--) {
		if(links[i]->quadOut == _quadOut) {
			delete links[i];
			links.erase(links.begin() + i);
		}
	}
}

void phdWarpLinkSchema::draw(ofMatrix4x4 & _srcMat, ofMatrix4x4 & _dstMat) {
	ofSetColor(ofColor::blue);
	for(int i = 0; i < links.size(); i++) {
		phdWarpLink * _link = links[i];
		phdShapeQuadWarp * _src = _link->quadIn;
		phdShapeQuadWarp * _dst = _link->quadOut;
		if(_src != NULL && _dst != NULL) {
			ofMatrix4x4 _ms = _src->gimbal.getOTSMatrix() * _srcMat;
			ofVec3f _cs; _cs.set(_src->gimbal.cx, _src->gimbal.cy); _cs = _cs * _ms;
			ofMatrix4x4 _md = _dst->gimbal.getOTSMatrix() * _dstMat;
			ofVec3f _cd; _cd.set(_dst->gimbal.cx, _dst->gimbal.cy); _cd = _cd * _md;
			ofLine(_cs.x, _cs.y, _cd.x, _cd.y);
		} else if(_src != NULL) {
			ofMatrix4x4 _ms = _src->gimbal.getOTSMatrix() * _srcMat;
			ofVec3f _cs; _cs.set(_src->gimbal.cx, _src->gimbal.cy); _cs = _cs * _ms;
			ofMatrix4x4 _md = _dst->gimbal.getOTSMatrix() * _dstMat;
			ofVec3f _cd; _cd.set(0,0); _cd = _cd * _md;
			ofLine(_cs.x, _cs.y, _cd.x, _cd.y);
		} else if(_dst != NULL) {
			ofMatrix4x4 _ms = _src->gimbal.getOTSMatrix() * _srcMat;
			ofVec3f _cs; _cs.set(0,0); _cs = _cs * _ms;
			ofMatrix4x4 _md = _dst->gimbal.getOTSMatrix() * _dstMat;
			ofVec3f _cd; _cd.set(_dst->gimbal.cx, _dst->gimbal.cy); _cd = _cd * _md;
			ofLine(_cs.x, _cs.y, _cd.x, _cd.y);
		}
	}
}
