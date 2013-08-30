#include "phdDesignerFbo.h"

//--------------------------------------------------------------------------------------------------------------
phdDesignerFbo::phdDesignerFbo() {
	schema.name = "DGNFBO";
	schema.shapes.freeShapes();
	viewport.checkBounds = true;
	popupMenu = NULL;
	visual = NULL;
	lastVisual= NULL;
	usingEvents = false;
	showSchema = true;
	selected = false;
	autoplay = true;
	useFade = true;

	setup(100,100,400,300,NULL,512,384);

	enableEvents();
}

phdDesignerFbo::phdDesignerFbo(float _x, float _y, float _viewWidth, float _viewHeight, ofFbo * _canvas, float _worldWidth, float _worldHeight) {
	schema.name = "DGNFBO";
	schema.shapes.freeShapes();
	viewport.checkBounds = true;
	popupMenu = NULL;
	visual = NULL;
	lastVisual= NULL;
	usingEvents = false;
	showSchema = true;
	selected = false;
	autoplay = true;
	useFade = true;

	setup(_x, _y, _viewWidth, _viewHeight, _canvas, _worldWidth, _worldHeight);

	enableEvents();
}

phdDesignerFbo::~phdDesignerFbo() {
	phdDesigner::~phdDesigner();
}

void phdDesignerFbo::setup(float _x, float _y, float _viewWidth, float _viewHeight, ofFbo * _canvas, float _worldWidth, float _worldHeight) {

	if(_viewWidth == 0 || _viewHeight == 0 || _worldWidth < 1 || _worldHeight < 1) return;

	if(_worldWidth != canvas.getWidth() || _worldHeight != canvas.getWidth()) {
		canvas.allocate(_worldWidth, _worldHeight);
	}

	viewport.setup(_x, _y, _viewWidth, _viewHeight, &canvas, _worldWidth, _worldHeight);
}

ofFbo * phdDesignerFbo::getCanvasPtr() {
	return &canvas;
}

