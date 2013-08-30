#include "phdDesigner.h"

//--------------------------------------------------------------------------------------------------------------
phdDesigner::phdDesigner() {
	schema.name = "DGN";
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

	enableEvents();
}

phdDesigner::phdDesigner(float _x, float _y, float _viewWidth, float _viewHeight, ofFbo * _canvas, float _worldWidth, float _worldHeight) {
	schema.name = "DGN";
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

phdDesigner::~phdDesigner() {
}

void phdDesigner::setup(float _x, float _y, float _viewWidth, float _viewHeight, ofFbo * _canvas, float _worldWidth, float _worldHeight) {

	if(_viewWidth == 0 || _viewHeight == 0) return;

	viewport.setup(_x, _y, _viewWidth, _viewHeight, _canvas, _worldWidth, _worldHeight);
}

void phdDesigner::setCanvasPtr(ofFbo * _canvas) {
	viewport.setCanvas(_canvas, viewport.worldSize.x, viewport.worldSize.y);
}

ofFbo * phdDesigner::getCanvasPtr() {
	return viewport.canvas;
}

void phdDesigner::setVisual(phdGenBase * _visual, bool _useFade) {

	lastVisual = visual; // keeps last visual

	if(visual != NULL) {
//		visual->refCount -= 1;
//		if(visual->refCount <= 0) visual->stop(_useFade);
	}

	visual = _visual;

	if(visual != NULL) {
//		if(autoplay) visual->play(_useFade);
	}
}

void phdDesigner::begin() {
	ofPushMatrix();
	glLoadMatrixf((GLfloat*) viewport.stc.getPtr());
	if(viewport.canvas != NULL) viewport.canvas->begin();
}

void phdDesigner::end() {
	if(viewport.canvas != NULL) viewport.canvas->end();
	ofPopMatrix();
}

void phdDesigner::saveToStrings(vector<string> & _strings, string _title) {

	string _line = "";

	_line = _title + ":" + name + ":" +
		ofToString(viewport.screenTopLeft.x) + ":" + 
		ofToString(viewport.screenTopLeft.y) + ":" + 
		ofToString(viewport.screenBottomRight.x-viewport.screenTopLeft.x) + ":" + 
		ofToString(viewport.screenBottomRight.y-viewport.screenTopLeft.y) + ":";

		if(viewport.worldSize.x == 0) _line += "512:"; else _line += ofToString(viewport.worldSize.x) + ":";
		if(viewport.worldSize.y == 0) _line += "384:"; else _line += ofToString(viewport.worldSize.y) + ":";

		_line += "\n";
	_strings.push_back(_line);

	phdWarpShapeSchema * _schema = &schema;

	_line = "SHAPESCHEMA:" + _schema->name + ":" + ofToString(_schema->size()) + ":\n";
	_strings.push_back(_line);

	for(int j = 0; j < _schema->size(); j++) {
		phdShapeQuadWarp * _shape = (phdShapeQuadWarp*) _schema->shapes[j];
		_line = "SHAPE:" + _shape->name + ":" + ofToString((int)_shape->faceType) + ":\n";
		_strings.push_back(_line);

		phdGimbal2d * _gimbal = &_schema->shapes[j]->gimbal;

		_line = "GIMBAL:" + ofToString(_gimbal->angle) + ":" +
			ofToString(_gimbal->cx) + ":" + ofToString(_gimbal->cy) + ":" +
			ofToString(_gimbal->sx) + ":" + ofToString(_gimbal->sy) + ":" +
			ofToString(_gimbal->w)  + ":" + ofToString(_gimbal->h)  + ":\n";
		_strings.push_back(_line);

		_line = "VERTEXCOUNT:" + ofToString(_schema->shapes[j]->vertices.size()) + ":";

		for(int k = 0; k < _schema->shapes[j]->vertices.size(); k++) {
			_line += ofToString(_schema->shapes[j]->vertices[k].x) + ":";
			_line += ofToString(_schema->shapes[j]->vertices[k].y) + ":";
		}

		_line += "\n";
		_strings.push_back(_line);
	}
}

void phdDesigner::draw() {

	// viewport canvas is same as visual canvas so can change to visual->open()
	if(viewport.canvas != NULL) viewport.canvas->begin();
		ofSetColor(255,255,255,255);
		ofMatrix4x4 _ma;
		if(showSchema ) schema.draw(_ma);
	if(viewport.canvas != NULL) viewport.canvas->end();

	viewport.draw(); // send to screen throught viewport

	// draw menu and messages
	float _toolSize = (hitTestInfo.commands.size() * 15.0);
	float _menuSize = 58;
	float _w = viewport.screenBottomRight.x-viewport.screenTopLeft.x;

	// menu
	drawFilledBorderRectangle(viewport.screenTopLeft.x, viewport.screenTopLeft.y - 15, _menuSize, 15, ofColor(92,32,92,255), ofColor(255,255,255,255));
	// title
	if(selected) {
		drawFilledBorderRectangle(viewport.screenTopLeft.x + _menuSize, viewport.screenTopLeft.y - 15, _w-_toolSize-_menuSize, 15, ofColor(120,192,255,200), ofColor(255,255,255,255));
	} else {
		drawFilledBorderRectangle(viewport.screenTopLeft.x + _menuSize, viewport.screenTopLeft.y - 15, _w-_toolSize-_menuSize, 15, ofColor(160,92,220,200), ofColor(255,255,255,255));
	}

	drawFilledBorderRectangle(viewport.screenBottomRight.x-4, viewport.screenBottomRight.y-4, 8, 8, ofColor(192,128,255,200), ofColor(255,255,255,255));

	ofSetColor(255,255,255,255);
	
	// main menu of designer - top left
	ofDrawBitmapString("phd.TJ", viewport.screenTopLeft.x + 5, viewport.screenTopLeft.y - 5);
	ofDrawBitmapString(this->name + ":" + ofToString(ofGetFrameRate(),2), viewport.screenTopLeft.x + _menuSize + 8, viewport.screenTopLeft.y - 5);

	if(hitTestInfo.infoType == ghtMenu) {
		string _msg = "Menu de comandos (options menu)";
		drawFilledBorderRectangle(viewport.screenTopLeft.x + 4, viewport.screenTopLeft.y+0, _msg.size()*8+6, 15, ofColor(192,128,255,200), ofColor(255,255,255,255));
		ofDrawBitmapString(_msg, viewport.screenTopLeft.x+6, viewport.screenTopLeft.y+11);
	}

	// draw tool bar - top right
	for(int i = 0; i < hitTestInfo.commands.size(); i++) {
		drawFilledBorderRectangle(viewport.screenBottomRight.x - _toolSize + (i*15), viewport.screenTopLeft.y - 15, 15, 15, ofColor(192,128,255,200), ofColor(255,255,255,255));
		ofDrawBitmapString(hitTestInfo.commands.substr(i,1), viewport.screenBottomRight.x - _toolSize + 4 + (i*15), viewport.screenTopLeft.y - 4);
	}

	if(hitTestInfo.command > -1) {
		drawFilledBorderRectangle(viewport.screenBottomRight.x-_toolSize, viewport.screenTopLeft.y+0, hitTestInfo.labels[hitTestInfo.command].size()*8, 15, ofColor(192,128,255,200), ofColor(255,255,255,255));
		ofDrawBitmapString(hitTestInfo.labels[hitTestInfo.command], viewport.screenBottomRight.x - _toolSize, viewport.screenTopLeft.y+11);
	}

	// drag point bottom right
	if(hitTestInfo.infoType == ghtResize) {
		string _msg = "Arraste para redimensionar (drag to resize)";
		drawFilledBorderRectangle(viewport.screenBottomRight.x, viewport.screenBottomRight.y, _msg.size()*8, 15, ofColor(192,128,255,200), ofColor(255,255,255,255));
		ofDrawBitmapString(_msg, viewport.screenBottomRight.x, viewport.screenBottomRight.y+11);
	}
	ofSetColor(255,255,255,255);
	ofFill();
}

void phdDesigner::updateHitTestInfo(float _x, float _y) {

	float _x1 = viewport.screenTopLeft.x;
	float _y1 = viewport.screenTopLeft.y;
	float _x2 = viewport.screenBottomRight.x;
	float _y2 = viewport.screenBottomRight.y;

	float verScrollX = viewport.verScrollPos.x;
	float verScrollY = viewport.verScrollPos.y;
	float horScrollX = viewport.horScrollPos.x;
	float horScrollY = viewport.horScrollPos.y;

	hitTestInfo.mouseX = _x;
	hitTestInfo.mouseY = _y;
	hitTestInfo.command = -1;

	float _toolSize = (hitTestInfo.commands.size() * 15.0);
	float _menuSize = 58;

	if(_x > _x1 && _x < _x1+_menuSize && _y > _y1-15 && _y < _y1) {
		hitTestInfo.infoType = ghtMenu;
	} else if(_x > _x1+_menuSize && _x < _x2-_toolSize && _y > _y1-15 && _y < _y1) {
		hitTestInfo.infoType = ghtDragControl;
	} else if(_x > horScrollX-6 && _x < horScrollX+6 && _y > horScrollY-6 && _y < horScrollY+6) {
		hitTestInfo.infoType = ghtHorizontalScroll;
	} else if(_x > verScrollX-6 && _x < verScrollX+6 && _y > verScrollY-6 && _y < verScrollY+6) {
		hitTestInfo.infoType = ghtVerticalScroll;
	} else if(_x > _x2-_toolSize && _x < _x2 && _y > _y1-15 && _y < _y1) {
		hitTestInfo.infoType = ghtCommand;
		if(hitTestInfo.commands.size() > 0) {
			hitTestInfo.command = (int)(hitTestInfo.commands.size() - ((_x2-_x)/_toolSize) * hitTestInfo.commands.size());
		}
	} else if(_x >= _x2-5 && _x <= _x2+5 && _y >= _y2-5 && _y <= _y2+5) {
		hitTestInfo.infoType = ghtResize;
	} else if(_x >= _x1 && _x <= _x2 && _y >= _y1 && _y <= _y2) {
		hitTestInfo.infoType = ghtInside;
	} else {
		hitTestInfo.infoType = ghtOutside;
	}
}

void phdDesigner::mouseMoved(ofMouseEventArgs& args) {

	updateHitTestInfo(args.x, args.y);

	if(hitTestInfo.infoType == ghtMenu) {
	} else if(hitTestInfo.infoType == ghtDragControl) {
	} else if(hitTestInfo.infoType == ghtHorizontalScroll) {
	} else if(hitTestInfo.infoType == ghtVerticalScroll) {
	} else if(hitTestInfo.infoType == ghtResize) {
	} else if(hitTestInfo.infoType == ghtCommand) {
	} else if(hitTestInfo.infoType == ghtInside) {
	} else if(hitTestInfo.infoType == ghtOutside) {
	}
	// screen coords
	hitTestInfo.mouseX = args.x;
	hitTestInfo.mouseY = args.y;

	viewport.mouseMoved(args);

	//if(schema == NULL) return;

	ofPoint _mousePos = viewport.mousePos;
	if(viewport.checkBounds && _mousePos.x == -1 && _mousePos.y == -1) return;

	_mousePos = _mousePos;// * viewport.ctw;
	ofMouseEventArgs _args = args; _args.x = _mousePos.x; _args.y = _mousePos.y;

	if(schema.shapes.getFocused() != NULL) {
		if(schema.shapes.getFocused()->updateHitTestInfo(_mousePos.x, _mousePos.y)) return;
	}

	schema.shapes.setFocused(NULL);
	for(int i = 0; i < schema.shapes.size(); i++) {
		if(schema.shapes[i] != NULL && schema.shapes[i]->updateHitTestInfo(_mousePos.x, _mousePos.y)) {
			schema.shapes.setFocused(schema.shapes[i]);
		}
	}

	if(schema.shapes.getFocused() != NULL) schema.shapes.getFocused()->shapeMouseMoved(_args);
}

void phdDesigner::mousePressed(ofMouseEventArgs& args) {

	if(hitTestInfo.infoType == ghtMenu) {
	} else if(hitTestInfo.infoType == ghtDragControl) {
	} else if(hitTestInfo.infoType == ghtHorizontalScroll) {
	} else if(hitTestInfo.infoType == ghtVerticalScroll) {
	} else if(hitTestInfo.infoType == ghtResize) {
	} else if(hitTestInfo.infoType == ghtCommand) {
	} else if(hitTestInfo.infoType == ghtInside) {
	} else if(hitTestInfo.infoType == ghtOutside) {
	}
	// screen coords
	hitTestInfo.mouseX = args.x;
	hitTestInfo.mouseY = args.y;

	viewport.mousePressed(args);

	//if(schema == NULL) return;

	ofPoint _mousePos = viewport.mousePos;
	if(viewport.checkBounds && _mousePos.x == -1 && _mousePos.y == -1) return;

	_mousePos = _mousePos;// * viewport.ctw;
	ofMouseEventArgs _args = args; _args.x = _mousePos.x; _args.y = _mousePos.y;
	if(args.button == 0 && schema.shapes.getSelected() != schema.shapes.getFocused()) schema.shapes.setSelected(schema.shapes.getFocused());
	if(schema.shapes.getSelected() != NULL) schema.shapes.getSelected()->shapeMousePressed(_args);
}

void phdDesigner::mouseDragged(ofMouseEventArgs& args) {

	if(hitTestInfo.infoType == ghtMenu) {
	} else if(hitTestInfo.infoType == ghtDragControl) {
		float _x1 = viewport.screenTopLeft.x;
		float _y1 = viewport.screenTopLeft.y;
		float _x2 = viewport.screenBottomRight.x;
		float _y2 = viewport.screenBottomRight.y;
		float _dx = args.x-hitTestInfo.mouseX;
		float _dy = args.y-hitTestInfo.mouseY;
		viewport.setScreenArea(ofPoint(_x1 + _dx, _y1 + _dy), ofPoint(_x2 + _dx, _y2 + _dy));
	} else if(hitTestInfo.infoType == ghtHorizontalScroll) {

		viewport.setHorScrollBarPos(args.x);

	} else if(hitTestInfo.infoType == ghtVerticalScroll) {

		viewport.setVerScrollBarPos(args.y);

	} else if(hitTestInfo.infoType == ghtResize) {
		float _x2 = viewport.screenBottomRight.x;
		float _y2 = viewport.screenBottomRight.y;
		float _dx = args.x-hitTestInfo.mouseX;
		float _dy = args.y-hitTestInfo.mouseY;
		viewport.setScreenArea(viewport.screenTopLeft, ofPoint(_x2 + _dx, _y2 + _dy));
	} else if(hitTestInfo.infoType == ghtCommand) {
	} else if(hitTestInfo.infoType == ghtInside) {
	} else if(hitTestInfo.infoType == ghtOutside) {
	}
	// screen coords
	hitTestInfo.mouseX = args.x;
	hitTestInfo.mouseY = args.y;

	viewport.mouseDragged(args);

	//if(schema == NULL) return;

	ofPoint _mousePos = viewport.mousePos;
	if(viewport.checkBounds && _mousePos.x == -1 && _mousePos.y == -1) return;

	_mousePos = _mousePos;// * viewport.ctw;
	ofMouseEventArgs _args = args; _args.x = _mousePos.x; _args.y = _mousePos.y;
	if(schema.shapes.getSelected() != NULL) {
		schema.shapes.getSelected()->shapeMouseDragged(_args);
		return;
	}
}

void phdDesigner::mouseReleased(ofMouseEventArgs& args) {

	if(hitTestInfo.infoType == ghtMenu) {
		if(popupMenu != NULL) {
			popupMenu->caller = name;
			popupMenu->setPosition(viewport.screenTopLeft.x + 8, viewport.screenTopLeft.y + 4);
			popupMenu->setVisible(true);
		}
	} else if(hitTestInfo.infoType == ghtDragControl) {

		if(popupMenu != NULL) {
			phdDesignerEventData _ded;
			_ded.designer = (phdDesigner*) this;
			_ded.eventType = pdeOnClickTitle;
			if(!selected) {
				_ded.eventType = pdeOnSelect;
			}
			ofNotifyEvent(popupMenu->dgnEvent, _ded, &(*this).popupMenu);

			selected = true;
		}

	} else if(hitTestInfo.infoType == ghtHorizontalScroll) {
	} else if(hitTestInfo.infoType == ghtVerticalScroll) {
	} else if(hitTestInfo.infoType == ghtResize) {
	} else if(hitTestInfo.infoType == ghtCommand) {
		if(hitTestInfo.command != -1) {
			char _c = hitTestInfo.commands[hitTestInfo.command];
			if(hitTestInfo.command == 0) viewport.zoomIn(); // (+)
			if(hitTestInfo.command == 1) viewport.zoomOut(); // (-)
			if(hitTestInfo.command == 2) viewport.fitAll(); // (#)
			if(hitTestInfo.command == 3) { // (*)
				ofPolyline _vrts; _vrts.addVertex(0,0); _vrts.addVertex(100,0); _vrts.addVertex(100,100); _vrts.addVertex(0,100);
				schema.addShape(name+"_"+ofToString(schema.size()), _vrts);
			}
			if(hitTestInfo.command == 4 && schema.shapes.getSelected() != NULL) { // (=)
				phdShapeQuadWarp * _shp = (phdShapeQuadWarp*) schema.shapes.getSelected();
				int _ft = _shp->faceType; _ft += 1; if(_ft > 5) _ft = 0;
				_shp->faceType = (phdWarpFaceType) _ft; //wftQuadDLD, wftQuadDLU, wftTriTL, wftTriTR, wftTriBR, wftTriBL
			}
			/*if(hitTestInfo.command == 5 && schema.shapes.getSelected() != NULL) { // (%)
				schema.shapes.delShape(schema.shapes.indexOf(schema.shapes.getSelected()));
			}*/
		}
	} else if(hitTestInfo.infoType == ghtInside) {
	} else if(hitTestInfo.infoType == ghtOutside) {
	}
	// screen coords
	hitTestInfo.mouseX = args.x;
	hitTestInfo.mouseY = args.y;

	viewport.mouseReleased(args);

	ofPoint _mousePos = viewport.mousePos;
	if(viewport.checkBounds && _mousePos.x == -1 && _mousePos.y == -1) return;

	_mousePos = _mousePos;// * viewport.ctw;
	ofMouseEventArgs _args = args; _args.x = _mousePos.x; _args.y = _mousePos.y;
	if(schema.shapes.getSelected() != NULL) schema.shapes.getSelected()->shapeMouseReleased(_args);
}

void phdDesigner::enableEvents() {
	if(!usingEvents) {
		ofAddListener(ofEvents().mouseMoved, this, &phdDesigner::mouseMoved);
		ofAddListener(ofEvents().mousePressed, this, &phdDesigner::mousePressed);
		ofAddListener(ofEvents().mouseDragged, this, &phdDesigner::mouseDragged);
		ofAddListener(ofEvents().mouseReleased, this, &phdDesigner::mouseReleased);
		usingEvents = true;
	}
}

void phdDesigner::disableEvents(){
	if(usingEvents) {
		ofRemoveListener(ofEvents().mouseMoved, this, &phdDesigner::mouseMoved);
		ofRemoveListener(ofEvents().mousePressed, this, &phdDesigner::mousePressed);
		ofRemoveListener(ofEvents().mouseDragged, this, &phdDesigner::mouseDragged);
		ofRemoveListener(ofEvents().mouseReleased, this, &phdDesigner::mouseReleased);
		usingEvents = false;
	}
}
