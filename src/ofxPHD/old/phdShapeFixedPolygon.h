#pragma once

#include "ofMain.h"

#include "phdShapeBase.h"
//--------------------------------------------------------------------------------------------------------------
class phdShapeFixedPolygon : public phdShapeBase {
public:
	phdShapeFixedPolygon();

	virtual void shapeMouseMoved(ofMouseEventArgs& args);
	virtual void shapeMousePressed(ofMouseEventArgs& args);
	virtual void shapeMouseDragged(ofMouseEventArgs& args);
	virtual void shapeMouseReleased(ofMouseEventArgs& args);

	virtual void update();
	virtual void draw(ofMatrix4x4 & _mat);
};
