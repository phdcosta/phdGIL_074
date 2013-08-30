#include "phdDesigner.h"

//--------------------------------------------------------------------------------------------------------------
phdDesigner::phdDesigner(ofFbo * _canvas) {
	curLinkSchema = NULL;

	setPosition(100,100);
	setDimension(320,240);
	//setCanvasPtr(_canvas);
	schema.name = "DGN";
	schema.shapes.freeShapes();
	viewport.checkBounds = true;
	setPopupMenu(NULL);
	showSchema = true;
	selected = false;

	inputLinks.addLink();
}

phdDesigner::phdDesigner(float _x, float _y, float _viewWidth, float _viewHeight, ofFbo * _canvas, float _worldWidth, float _worldHeight) {
	curLinkSchema = NULL;

	setPosition(100,100);
	setDimension(320,240);
	//setCanvasPtr(_canvas);
	schema.name = "DGN";
	schema.shapes.freeShapes();
	viewport.checkBounds = true;
	setPopupMenu(NULL);
	showSchema = true;
	selected = false;

	inputLinks.addLink();

	setup(_x, _y, _viewWidth, _viewHeight, _canvas, _worldWidth, _worldHeight);
}

phdDesigner::~phdDesigner() {
	schema.shapes.setFocused(NULL);
	schema.shapes.setSelected(NULL);
	schema.shapes.freeShapes();
}

void phdDesigner::allocateInternalCanvas(float _w, float _h) {
	if(_w > 0 && _h > 0) {
		if(_w != internalCanvas.getWidth() || _h != internalCanvas.getHeight() || !internalCanvas.isAllocated()) {
			internalCanvas.allocate(_w, _h, GL_RGBA);
		}
	}
}

void phdDesigner::setup(float _x, float _y, float _viewWidth, float _viewHeight, ofFbo * _canvas, float _worldWidth, float _worldHeight) {

	allocateInternalCanvas(_worldWidth, _worldHeight);

	if(_viewWidth == 0 || _viewHeight == 0) return;

	viewport.setup(_x, _y, _viewWidth, _viewHeight, &internalCanvas, _worldWidth, _worldHeight);

	setPosition(_x, _y);
	setDimension(_viewWidth, _viewHeight);
}

void phdDesigner::begin() {
	ofPushMatrix();
	glLoadMatrixf((GLfloat*) viewport.stc.getPtr());
	if(internalCanvas.isAllocated()) internalCanvas.begin();
}

void phdDesigner::end() {
	if(internalCanvas.isAllocated()) internalCanvas.end();
	ofPopMatrix();
}

void phdDesigner::bind() {
	if(internalCanvas.isAllocated()) internalCanvas.getTextureReference().bind();
}

void phdDesigner::unbind() {
	if(internalCanvas.isAllocated()) internalCanvas.getTextureReference().unbind();
}

void phdDesigner::saveToStrings(vector<string> & _strings, string _title) {
	
	string _line = "";

	_line = _title + "|" + name + "|" +
		ofToString(viewport.screenTopLeft.x) + "|" + 
		ofToString(viewport.screenTopLeft.y) + "|" + 
		ofToString(viewport.screenBottomRight.x-viewport.screenTopLeft.x) + "|" + 
		ofToString(viewport.screenBottomRight.y-viewport.screenTopLeft.y) + "|";

	if(viewport.worldSize.x == 0) _line += "512|"; else _line += ofToString(viewport.worldSize.x) + "|";
	if(viewport.worldSize.y == 0) _line += "384|"; else _line += ofToString(viewport.worldSize.y) + "|";

		_line += "\n";
	_strings.push_back(_line);

	phdWarpShapeSchema * _schema = &schema;

	_line = "SHAPESCHEMA|" + _schema->name + "|" + ofToString(_schema->size()) + "|\n";
	_strings.push_back(_line);

	for(int j = 0; j < _schema->size(); j++) {
		phdShapeQuadWarp * _shape = (phdShapeQuadWarp*) _schema->shapes[j];
		_line = "SHAPE|" + _shape->name + "|" + ofToString((int)_shape->faceType) + "|\n";
		_strings.push_back(_line);

		phdGimbal2d * _gimbal = &_schema->shapes[j]->gimbal;

		_line = "GIMBAL|" + ofToString(_gimbal->angle) + "|" +
			ofToString(_gimbal->cx) + "|" + ofToString(_gimbal->cy) + "|" +
			ofToString(_gimbal->sx) + "|" + ofToString(_gimbal->sy) + "|" +
			ofToString(_gimbal->w)  + "|" + ofToString(_gimbal->h)  + "|\n";
		_strings.push_back(_line);

		_line = "VERTEXCOUNT|" + ofToString(_schema->shapes[j]->vertices.size()) + "|";

		for(int k = 0; k < _schema->shapes[j]->vertices.size(); k++) {
			_line += ofToString(_schema->shapes[j]->vertices[k].x) + "|";
			_line += ofToString(_schema->shapes[j]->vertices[k].y) + "|";
		}

		_line += "\n";
		_strings.push_back(_line);
	}
	
}

void phdDesigner::update() {

	schema.shapes.update();
	if(curLinkSchema != NULL) curLinkSchema->updateFlags(); // mark upates

	if(designerType == phdDesignerInput) {
		// viewport canvas is same as visual canvas so can change to visual->open()
		//if(canvas != NULL) canvas->begin();
		if(internalCanvas.isAllocated()) internalCanvas.begin();
			ofPushStyle();
			ofClear(255);
			//ofFill();
			//ofSetColor(0,0,0,255);
			//ofRect(0, 0, internalCanvas.getWidth(), internalCanvas.getHeight());
			//ofNoFill();
			//ofClear(0);
			//ofBackground(0);
			ofSetColor(255,255,255,255);
			if(getInputClip() != NULL) getInputClip()->draw(0, 0, internalCanvas.getWidth(), internalCanvas.getHeight());

			ofSetColor(255,255,255,255);
			ofMatrix4x4 _ma; //_ma = viewport.cts;
			if(showSchema ) schema.draw(_ma);
			ofPopStyle();
		if(internalCanvas.isAllocated()) internalCanvas.end();
		//if(canvas != NULL) canvas->end();
	} else if(designerType == phdDesignerOutput) {
		if(internalCanvas.isAllocated()) internalCanvas.begin();
			ofPushStyle();
			ofClear(255);
			//ofFill();
			//ofSetColor(0,0,0,255);
			//ofRect(0, 0, internalCanvas.getWidth(), internalCanvas.getHeight());
			//ofNoFill();
			//ofClear(0);
			//ofBackground(0);
			ofSetColor(255,255,255,255);
			if(curLinkSchema != NULL) {
				curLinkSchema->updateFlags(); // mark upates
				DrawWarpSchema(curLinkSchema, NULL);
			}
			ofSetColor(255,255,255,255);
			ofMatrix4x4 _ma; //_ma = viewport.cts;
			if(showSchema ) schema.draw(_ma);
			ofPopStyle();
		if(internalCanvas.isAllocated()) internalCanvas.end();
	}
}

void phdDesigner::DrawWarpSchema(phdWarpLinkSchema * _schema, phdDesigner * _default) {
	//ofSetColor(255,255,255,255);
	for(int i = 0; i < _schema->links.size(); i++) {
		double _x, _y;
		_schema->links[i]->updateWarper(512,384,512,384,false); // adjust if triangles
		_schema->links[i]->warper.begin();
		if(_schema->links[i]->quadIn != NULL) {
			if(_schema->links[i]->dsgnIn != NULL) {
				phdDesigner * _dsgnIn = _schema->links[i]->dsgnIn;
				if(_dsgnIn != NULL) _dsgnIn->bind();
				//if(_schema->links[i]->dsgnIn != NULL) _schema->links[i]->dsgnIn->bind();
			}
			glBegin(GL_TRIANGLES);
				if(_schema->links[i]->quadIn->faceType == wftQuadDLD || _schema->links[i]->quadIn->faceType == wftTriTR) {
					_schema->links[i]->warper.getSource(0, _x, _y); glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0);// glColor4f(1.0,1.0,1.0,1.0);
					_schema->links[i]->warper.getSource(2, _x, _y); glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0);// glColor4f(1.0,1.0,1.0,1.0);
					_schema->links[i]->warper.getSource(1, _x, _y); glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0);// glColor4f(1.0,1.0,1.0,1.0);
				}
				if(_schema->links[i]->quadIn->faceType == wftQuadDLD || _schema->links[i]->quadIn->faceType == wftTriBL) {
					_schema->links[i]->warper.getSource(0, _x, _y); glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0);// glColor4f(1.0,1.0,1.0,1.0);
					_schema->links[i]->warper.getSource(3, _x, _y); glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0);// glColor4f(1.0,1.0,1.0,1.0);
					_schema->links[i]->warper.getSource(2, _x, _y); glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0);// glColor4f(1.0,1.0,1.0,1.0);
				}
				if(_schema->links[i]->quadIn->faceType == wftQuadDLU || _schema->links[i]->quadIn->faceType == wftTriTL) {
					_schema->links[i]->warper.getSource(0, _x, _y); glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0);// glColor4f(1.0,1.0,1.0,1.0);
					_schema->links[i]->warper.getSource(3, _x, _y); glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0);// glColor4f(1.0,1.0,1.0,1.0);
					_schema->links[i]->warper.getSource(1, _x, _y); glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0);// glColor4f(1.0,1.0,1.0,1.0);
				}
				if(_schema->links[i]->quadIn->faceType == wftQuadDLU || _schema->links[i]->quadIn->faceType == wftTriBR) {
					_schema->links[i]->warper.getSource(3, _x, _y); glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0);// glColor4f(1.0,1.0,1.0,1.0);
					_schema->links[i]->warper.getSource(2, _x, _y); glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0);// glColor4f(1.0,1.0,1.0,1.0);
					_schema->links[i]->warper.getSource(1, _x, _y); glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0);// glColor4f(1.0,1.0,1.0,1.0);
				}
			glEnd();
			if(_schema->links[i]->dsgnIn != NULL && _schema->links[i]->dsgnIn != NULL) {
				phdDesigner * _dsgnIn = _schema->links[i]->dsgnIn;
				if(_dsgnIn != NULL) _dsgnIn->unbind();
				//if(_schema->links[i]->dsgnIn != NULL) _schema->links[i]->dsgnIn->unbind();
			}
		} else if(_default != NULL) { // if there inst quadIn use full src frame
			double _x, _y;
			float _srcW = 512, _srcH = 384; // default size
			if(_default != NULL) { _srcW = _default->width; _srcH = _default->height; }
			glBegin(GL_TRIANGLES);
				_x = 0;		_y = 0;		glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0); glColor4f(1.0,1.0,1.0,1.0);
				_x = _srcW; _y = _srcH;	glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0); glColor4f(1.0,1.0,1.0,1.0);
				_x = _srcW; _y = 0;		glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0); glColor4f(1.0,1.0,1.0,1.0);

				_x = 0;		_y = 0;		glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0); glColor4f(1.0,1.0,1.0,1.0);
				_x = 0;		_y = _srcH;	glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0); glColor4f(1.0,1.0,1.0,1.0);
				_x = _srcW; _y = _srcH;	glTexCoord2f(_x, _y); glVertex3f(_x, _y, 0); glColor4f(1.0,1.0,1.0,1.0);
			glEnd();
		}
		_schema->links[i]->warper.end();
	}
}

void phdDesigner::DrawDesignerBorder() {
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
	ofDrawBitmapString("phdGIL", viewport.screenTopLeft.x + 5, viewport.screenTopLeft.y - 5);
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

	inputLinks.draw(getX()+linkSize, getY()-(float)(linkSize*0.75)-15.0);
	outputLinks.draw(getX()+linkSize, getY()+getHeight()+(float)(linkSize*0.75));
}

void phdDesigner::draw() {

	ofPushStyle();
	ofPushMatrix();

	viewport.draw(); // send to screen throught viewport

	DrawDesignerBorder(); // Draw Border

	ofPopMatrix();
	ofPopStyle();
}

bool phdDesigner::updateHitTestInfo(float _x, float _y) {

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
	hitTestInfo.infoType = ghtOutside;

	hitTestInfo.linkIndex = -1;

	// popup menu
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		if(getPopupMenu()->updateHitTestInfo(_x, _y)) {
			hitTestInfo.infoType = ghtPopupMenu;
			return true;
		}
	}

	if(showSchema && schema.shapes.getPopupMenu() != NULL && schema.shapes.getPopupMenu()->getVisible()) {
		if(schema.shapes.getPopupMenu()->updateHitTestInfo(_x, _y)) {
			hitTestInfo.infoType = ghtPopupMenu;
			return true;
		}
	}

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
	} else if(inputLinks.updateHitTestInfo(_x,_y)) {
		hitTestInfo.infoType = ghtInputLink;
		hitTestInfo.linkIndex = inputLinks.focusedIndex;
	} else if(outputLinks.updateHitTestInfo(_x,_y)) {
		hitTestInfo.infoType = ghtOutputLink;
		hitTestInfo.linkIndex = outputLinks.focusedIndex;
	} else if(_x >= _x1 && _x <= _x2 && _y >= _y1 && _y <= _y2) {
		hitTestInfo.infoType = ghtInside;
	} else {
		hitTestInfo.infoType = ghtOutside;
	}

	return (hitTestInfo.infoType != ghtOutside);
}

void phdDesigner::onMouseMoved(ofMouseEventArgs& args) {

	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {// && getPopupMenu()->updateHitTestInfo(args.x,args.y)) {
		getPopupMenu()->onMouseMoved(args);
		return;
	}

	if(showSchema && schema.shapes.getPopupMenu() != NULL && schema.shapes.getPopupMenu()->getVisible()) {
		schema.shapes.getPopupMenu()->onMouseMoved(args);
		return;
	}

	updateHitTestInfo(args.x, args.y);

	if(hitTestInfo.infoType == ghtPopupMenu) {
	} else if(hitTestInfo.infoType == ghtMenu) {
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

	ofPoint _mousePos = viewport.mousePos;
	if(viewport.checkBounds && _mousePos.x == -1 && _mousePos.y == -1) return;

	ofMouseEventArgs _args = args; _args.x = _mousePos.x; _args.y = _mousePos.y;
	schema.shapes.onMouseMoved(_args);
}

void phdDesigner::onMousePressed(ofMouseEventArgs& args) {

	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMousePressed(args);
		return;
	}
	/*
	if(showSchema && schema.shapes.getPopupMenu() != NULL && schema.shapes.getPopupMenu()->getVisible()) {
		schema.shapes.getPopupMenu()->onMousePressed(args);
		return;
	}
	*/
	if(hitTestInfo.infoType != ghtOutside) {
		// hides shape popup if visible
		if(schema.shapes.getPopupMenu() != NULL &&schema.shapes.getPopupMenu()->getVisible())
			schema.shapes.getPopupMenu()->onMouseReleased(args);
	}

	if(hitTestInfo.infoType == ghtPopupMenu) {
	} else if(hitTestInfo.infoType == ghtMenu) {
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

	ofPoint _mousePos = viewport.mousePos;
	if(viewport.checkBounds && _mousePos.x == -1 && _mousePos.y == -1) return;

	ofMouseEventArgs _args = args; _args.x = _mousePos.x; _args.y = _mousePos.y;
	schema.shapes.onMousePressed(_args);
}

void phdDesigner::onMouseDragged(ofMouseEventArgs& args) {

	if(getPopupMenu() != NULL && getPopupMenu()->getVisible() && getPopupMenu()->updateHitTestInfo(args.x,args.y)) {
		getPopupMenu()->onMouseDragged(args);
		return;
	}

	if(showSchema && schema.shapes.getPopupMenu() != NULL && schema.shapes.getPopupMenu()->getVisible()) {
		schema.shapes.getPopupMenu()->onMouseDragged(args);
		return;
	}

	if(hitTestInfo.infoType == ghtPopupMenu) {

	} else if(hitTestInfo.infoType == ghtMenu) {

	} else if(hitTestInfo.infoType == ghtDragControl) {
		float _x1 = viewport.screenTopLeft.x;
		float _y1 = viewport.screenTopLeft.y;
		float _x2 = viewport.screenBottomRight.x;
		float _y2 = viewport.screenBottomRight.y;
		float _dx = args.x-hitTestInfo.mouseX;
		float _dy = args.y-hitTestInfo.mouseY;
		viewport.setScreenArea(ofPoint(_x1 + _dx, _y1 + _dy), ofPoint(_x2 + _dx, _y2 + _dy));

		setPosition(_x1, _y1);
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
		setDimension(viewport.screenBottomRight.x-viewport.screenTopLeft.x, viewport.screenBottomRight.y-viewport.screenTopLeft.y);

	} else if(hitTestInfo.infoType == ghtCommand) {
	} else if(hitTestInfo.infoType == ghtInside) {
	} else if(hitTestInfo.infoType == ghtOutside) {
	}
	// screen coords
	hitTestInfo.mouseX = args.x;
	hitTestInfo.mouseY = args.y;

	viewport.mouseDragged(args);

	// viewport coords
	ofPoint _mousePos = viewport.mousePos;
	if(viewport.checkBounds && _mousePos.x == -1 && _mousePos.y == -1) return;

	ofMouseEventArgs _args = args; _args.x = _mousePos.x; _args.y = _mousePos.y;
	schema.shapes.onMouseDragged(_args);
}

void phdDesigner::onMouseReleased(ofMouseEventArgs& args) {

	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMouseReleased(args);
		//ofRemoveListener(getPopupMenu()->popEvent, this, &phdDesigner::onPopupClick);
		return;
	}

	if(showSchema && schema.shapes.getPopupMenu() != NULL && schema.shapes.getPopupMenu()->getVisible()) {
		schema.shapes.getPopupMenu()->onMouseReleased(args);
		//ofRemoveListener(schema.shapes.getPopupMenu()->popEvent, this, &phdShapeGroup::onPopupClick);
		return;
	}

	if(hitTestInfo.infoType == ghtPopupMenu) {

	} else if(hitTestInfo.infoType == ghtMenu) {
		if(getPopupMenu() != NULL) {
			getPopupMenu()->setOwner(this);
			getPopupMenu()->popup(viewport.screenTopLeft.x + 8, viewport.screenTopLeft.y + 4, (void*)this, "DESIGNER" + name);
			//ofAddListener(getPopupMenu()->popEvent, this, &phdDesigner::onPopupClick);
		}
	} else if(hitTestInfo.infoType == ghtDragControl) {

		if(getPopupMenu() != NULL) {
			phdDesignerEventData _ded;
			_ded.designer = (phdDesigner*) this;
			_ded.eventType = pdeOnClickTitle;
			if(!selected) {
				_ded.eventType = pdeOnSelect;
			}
			//ofNotifyEvent(popupMenu->dgnEvent, _ded, &(*this).popupMenu);  /******* PROBLEMA --- DGNEVENT

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

	// viewport coords
	ofPoint _mousePos = viewport.mousePos;
	if(viewport.checkBounds && _mousePos.x == -1 && _mousePos.y == -1) return;

	ofMouseEventArgs _args = args; _args.x = _mousePos.x; _args.y = _mousePos.y;
	schema.shapes.onMouseReleased(_args);
}

void phdDesigner::onPopupClick(phdPopupMenuEventData &_eventData) {
	if(_eventData.menu != NULL) {
		ofRemoveListener(_eventData.menu->popEvent, this, &phdDesigner::onPopupClick);
	}
}

//-----------------------------------------------------------------------------------------------------------------
phdWarpLink::phdWarpLink() {
	updated = true;
	verMirror = horMirror = false;
	quadIn = NULL;
	quadOut = NULL;
	dsgnIn = dsgnOut = NULL;
	warper.setup(512,384,512,384);
}

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
