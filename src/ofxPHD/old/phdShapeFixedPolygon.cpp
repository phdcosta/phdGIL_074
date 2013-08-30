#include "phdShapeFixedPolygon.h"

//--------------------------------------------------------------------------------------------------------------
phdShapeFixedPolygon::phdShapeFixedPolygon() {
	phdShapeBase::phdShapeBase();
}

void phdShapeFixedPolygon::shapeMouseMoved(ofMouseEventArgs& args) {

}

void phdShapeFixedPolygon::shapeMousePressed(ofMouseEventArgs& args) {
	
	clickPos.set(args.x, args.y, 0.0);
	dragPos.set(args.x, args.y, 0.0);
	
	updateHitTestInfo(args.x, args.y);
}

void phdShapeFixedPolygon::shapeMouseDragged(ofMouseEventArgs& args) {

	if(args.button == 0) { // left button
		if(hitTestInfo.operation == goWidth) {
			setWidthScale(args.x, args.y);
		} else if(hitTestInfo.operation == goRotate) {
			setAngle(args.x, args.y);
		} else if(hitTestInfo.operation == goCenter) {
			setRotationPoint(args.x, args.y);
		} else if(hitTestInfo.operation == goHeight) {
			setHeightScale(args.x, args.y);
		} else if(hitTestInfo.operation == goVertex) {

		} else if(hitTestInfo.operation == goEdge) {
			if(!closed) {
				gimbal.cx += args.x - dragPos.x;
				gimbal.cy += args.y - dragPos.y;
			}
		} else if(hitTestInfo.operation == goInside) {
			gimbal.cx += args.x - dragPos.x;
			gimbal.cy += args.y - dragPos.y;
		}
	}

	dragPos.set(args.x, args.y, 0.0);
}

void phdShapeFixedPolygon::shapeMouseReleased(ofMouseEventArgs& args) {
	if(hitTestInfo.operation == goWidth) {

	} else if(hitTestInfo.operation == goRotate) {

	} else if(hitTestInfo.operation == goCenter) {

	} else if(hitTestInfo.operation == goHeight) {

	} else if(hitTestInfo.operation == goVertex) {

	} else if(hitTestInfo.operation == goEdge) {

	} else if(hitTestInfo.operation == goInside) {

	}
}

void phdShapeFixedPolygon::update() {

}

void phdShapeFixedPolygon::draw(ofMatrix4x4 & _mat) {

	ofPolyline _poly; vertices.getTransformed(_poly, gimbal.getOTSMatrix() * _mat);

	if(selected) {
		if(closed) drawFace(_poly, _poly);
		drawBorder(_poly);
		drawHitTestInfo(_poly, _mat, true, false, false);
	} else if(focused) {
		if(closed) drawFace(_poly, _poly);
		drawBorder(_poly);
		drawHitTestInfo(_poly, _mat, true, false, false);
	} else {
		if(closed) drawFace(_poly, _poly);
		drawBorder(_poly);
	}

	if(selected) drawStatus();
}

