#include "phdWarpLink.h"

//-----------------------------------------------------------------------------------------------------------------
phdWarpLink::phdWarpLink() {
	updated = true;
	verMirror = horMirror = false;
	quadIn = NULL;
	quadOut = NULL;
	dsgnIn = dsgnOut = NULL;
	warper.setup(512,384,512,384);
}
	
/*phdWarpLink::phdWarpLink(phdShapeQuadWarp * _source, phdShapeQuadWarp * _target) {
	verMirror = horMirror = false;
	updated = true;
	quadIn = _source;
	quadOut = _target;
	dsgnIn = dsgnOut = NULL;
	updateWarper(1024,768,1024,768,false);
}*/

phdWarpLink::phdWarpLink(phdShapeQuadWarp * _source, phdDesigner * _dsgnIn, phdShapeQuadWarp * _target, phdDesigner * _dsgnOut) {
	verMirror = horMirror = false;
	updated = true;
	quadIn = _source;
	dsgnIn = _dsgnIn;
	quadOut = _target;
	dsgnOut = _dsgnOut;
	updateWarper(512,384,512,384,false);
}

phdWarpLink::~phdWarpLink() {
	dsgnIn = NULL;
	quadIn = NULL;
	dsgnOut = NULL;
	quadOut = NULL;
}
	
void phdWarpLink::updateWarper(double _srcW, double _srcH, double _dstW, double _dstH, bool _scale) {

	if(quadIn != NULL && updated) {
		ofPolyline _verts;
		quadIn->vertices.getTransformed(_verts, quadIn->gimbal.getOTSMatrix());
		for(int i = 0; i < _verts.size(); i++) {
			warper.setSource(i, _verts[i].x, _verts[i].y);
		}

		// apply paralelogram rule on source if it is triangles
		double _x0, _y0, _x1, _y1, _x2, _y2, _x3, _y3;
		warper.getSource(0, _x0, _y0);
		warper.getSource(1, _x1, _y1);
		warper.getSource(2, _x2, _y2);
		warper.getSource(3, _x3, _y3);

		if(quadIn->faceType == wftTriTR) {
			warper.setSource(3, _x2+(_x0-_x1), _y2+(_y0-_y1));
			quadIn->setVertexPos(3, _x2+(_x0-_x1), _y2+(_y0-_y1));
		}
		if(quadIn->faceType == wftTriBL) {
			warper.setSource(1, _x0+(_x2-_x3), _y0+(_y2-_y3));
			quadIn->setVertexPos(1, _x0+(_x2-_x3), _y0+(_y2-_y3));
		}
		if(quadIn->faceType == wftTriTL) {
			warper.setSource(2, _x3+(_x1-_x0), _y3+(_y1-_y0));
			quadIn->setVertexPos(2, _x3+(_x1-_x0), _y3+(_y1-_y0));
		}
		if(quadIn->faceType == wftTriBR) {
			warper.setSource(0, _x1+(_x3-_x2), _y1+(_y3-_y2));
			quadIn->setVertexPos(0, _x1+(_x3-_x2), _y1+(_y3-_y2));
		}

		quadIn->updated = false;
	} else if(quadIn == NULL) {
		warper.setSrcQuad(0,0,512,0,512,384,0,384);
	}

	if(quadOut != NULL && updated) {
		ofPolyline _verts;
		quadOut->vertices.getTransformed(_verts, quadOut->gimbal.getOTSMatrix());

		for(int i = 0; i < _verts.size(); i++) {
			warper.setTarget(i, _verts[i].x, _verts[i].y);
		}

		if(verMirror && !horMirror) {
			warper.setTarget(0, _verts[3].x, _verts[3].y);
			warper.setTarget(3, _verts[0].x, _verts[0].y);
			warper.setTarget(1, _verts[2].x, _verts[2].y);
			warper.setTarget(2, _verts[1].x, _verts[1].y);
		}
		if(horMirror && !verMirror) {
			warper.setTarget(0, _verts[1].x, _verts[1].y);
			warper.setTarget(1, _verts[0].x, _verts[0].y);
			warper.setTarget(3, _verts[2].x, _verts[2].y);
			warper.setTarget(2, _verts[3].x, _verts[3].y);
		}
		if(horMirror && verMirror) {
			warper.setTarget(0, _verts[2].x, _verts[2].y);
			warper.setTarget(2, _verts[0].x, _verts[0].y);
			warper.setTarget(3, _verts[1].x, _verts[1].y);
			warper.setTarget(1, _verts[3].x, _verts[3].y);
		}

		quadOut->updated = false;
	} else if(quadOut == NULL) {
		warper.setDstQuad(0,0,512,0,512,384,0,384);
	}

	// actualized
	updated = false;
}
