#pragma once

#include "ofMain.h"

#include "phdShapeBase.h"
//--------------------------------------------------------------------------------------------------------------
class phdShapeFixedPolygon : public phdShapeBase {
public:
	phdShapeFixedPolygon();

	virtual void onMouseMoved(ofMouseEventArgs& args);
	virtual void onMousePressed(ofMouseEventArgs& args);
	virtual void onMouseDragged(ofMouseEventArgs& args);
	virtual void onMouseReleased(ofMouseEventArgs& args);

	virtual void update();
	virtual void draw(ofMatrix4x4 & _mat);
};
