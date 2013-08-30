#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	//===========================================================================================
	// NEEDS TO CALL THIS HERE TO SAVE APP DIR INSIDE OFUTILS.CPP
	//===========================================================================================
	ofSetDataPathRoot("data/");
	//===========================================================================================

	managerPopup.addItem("Add New Designer...", PM_MANAGER+0, NULL, false);
	managerPopup.addItem("Add New Track...",	PM_MANAGER+1, NULL, false);
	ofAddListener(managerPopup.popEvent, this, &testApp::managerPopupClick);

	designerPopup.addItem("Show/Hide Params...",	PM_DESIGNER+0, NULL, false);
	designerPopup.addItem("Show/Hide Slice Map",	PM_DESIGNER+1, NULL, false);
	designerPopup.addItem("Link Select to Output",	PM_DESIGNER+2, NULL, false);
	designerPopup.addItem("Del Select Slice",		PM_DESIGNER+3, NULL, false);
	designerPopup.addItem("Adjust Near Vertices",	PM_DESIGNER+4, NULL, false);
	designerPopup.addItem("Clear Links for Sel",	PM_DESIGNER+5, NULL, false);
	designerPopup.addItem("Delete this Designer",	PM_DESIGNER+6, NULL, false);
	ofAddListener(designerPopup.popEvent, this, &testApp::designerPopupClick);

	outputPopup.addItem("Show/Hide Params",		PM_OUTPUT+0, NULL, false);
	outputPopup.addItem("Show/Hide Warp Map",	PM_OUTPUT+1, NULL, false);
	outputPopup.addItem("Del Select Warp",		PM_OUTPUT+2, NULL, false);
	outputPopup.addItem("Adjust Near Vertices",	PM_OUTPUT+3, NULL, false);
	outputPopup.addItem("Clear Links for Sel",	PM_OUTPUT+4, NULL, false);
	ofAddListener(outputPopup.popEvent, this, &testApp::outputPopupClick);

	shapeModePopup.addItem("Quad TR", PM_SHAPEMODE+0, NULL, false);
	shapeModePopup.addItem("Quad BR", PM_SHAPEMODE+1, NULL, false);
	shapeModePopup.addItem("Tri TL",  PM_SHAPEMODE+2, NULL, false);
	shapeModePopup.addItem("Tri TR",  PM_SHAPEMODE+3, NULL, false);
	shapeModePopup.addItem("Tri BR",  PM_SHAPEMODE+4, NULL, false);
	shapeModePopup.addItem("Tri BL",  PM_SHAPEMODE+5, NULL, false);
	ofAddListener(shapeModePopup.popEvent, this, &testApp::shapeModePopupClick);

	mirrorPopup.addItem("No Mirror",	PM_MIRROR+0, NULL, false);
	mirrorPopup.addItem("Vertical",		PM_MIRROR+1, NULL, false);
	mirrorPopup.addItem("Horizontal",	PM_MIRROR+2, NULL, false);
	mirrorPopup.addItem("Invert",		PM_MIRROR+3, NULL, false);
	ofAddListener(mirrorPopup.popEvent, this, &testApp::mirrorPopupClick);

	shapeInPopup.addItem("Del Slice In",	PM_SHAPEIN+0, NULL, false);
	shapeInPopup.addItem("Shape Mode...",	PM_SHAPEIN+1, &shapeModePopup, false);
	ofAddListener(shapeInPopup.popEvent, this, &testApp::shapeInPopupClick);

	shapeOutPopup.addItem("Del Warp Out",	PM_SHAPEOUT+0, NULL, false);
	shapeOutPopup.addItem("Shape Mirror...",PM_SHAPEOUT+1, &mirrorPopup, false);
	ofAddListener(shapeOutPopup.popEvent, this, &testApp::shapeOutPopupClick);

	clipPopup.addItem("Load from file...",	PM_CLIP+0, NULL, false);
	clipPopup.addItem("Select Generator...",PM_CLIP+1, &generatorsPopup, false);
	clipPopup.addItem("Select Grabber...",	PM_CLIP+2, &grabbersPopup, false);
	clipPopup.addItem("Delete Clip",		PM_CLIP+3, NULL, false);
	ofAddListener(clipPopup.popEvent, this, &testApp::clipPopupClick);

	trackPopup.addItem("Add New Clip...",	PM_TRACK+0, NULL, false);
	trackPopup.addItem("Add New Filter",	PM_TRACK+1, NULL, false);
	trackPopup.addItem("Set Transiction",	PM_TRACK+2, NULL, false);
	trackPopup.addItem("Delete this Track",	PM_TRACK+3, NULL, false);
	ofAddListener(trackPopup.popEvent, this, &testApp::trackPopupClick);

	mappingPopup.addItem("Open mapping...",		PM_MAPPING+0, NULL, false);
	mappingPopup.addItem("Save mapping...",		PM_MAPPING+1, NULL, false);
	mappingPopup.addItem("Add New Schema...",	PM_MAPPING+2, NULL, false);
	mappingPopup.addItem("Add New Designer...",	PM_MAPPING+3, NULL, false);
	ofAddListener(mappingPopup.popEvent, this, &testApp::mappingPopupClick);

	ofEnableAlphaBlending();

	//-------------------------------------------------------
	_mapping = new phdMappingControl();
	controlManager.addControl(_mapping);

	_mapping->setDimension(200, 200);
	_mapping->setPosition(50, 20);
	_mapping->setPopupMenu(&mappingPopup);

	_mapping->popups.addItem("PM_DESIGNER",	&designerPopup);
	_mapping->popups.addItem("PM_OUTPUT",	&outputPopup);
	_mapping->popups.addItem("PM_SHAPEIN",	&shapeInPopup);
	_mapping->popups.addItem("PM_SHAPEOUT",	&shapeOutPopup);
	_mapping->popups.addItem("PM_CLIP",		&clipPopup);
	_mapping->popups.addItem("PM_TRACK",	&trackPopup);

	// register all grabbers
	//-------------------------------------------------------
	phdKinectGrabber * _grabber = new phdKinectGrabber();
	_grabber = (phdKinectGrabber*) _mapping->mapping.grabberManager.registerGrabber(_grabber);
	_grabber->configureGrabber();

	phdPS3EyeGrabber * _ps3grabber = new phdPS3EyeGrabber();
	_ps3grabber = (phdPS3EyeGrabber*) _mapping->mapping.grabberManager.registerGrabber(_ps3grabber);
	_ps3grabber->configureGrabber();

	phdVideoGrabber * _videoGrabber = new phdVideoGrabber();
	_videoGrabber = (phdVideoGrabber*) _mapping->mapping.grabberManager.registerGrabber(_videoGrabber);
	_videoGrabber->configureGrabber();

	// register all generatos
	//-------------------------------------------------------
	_mapping->mapping.generatorManager.registerGenerator(new phdGenImage());
	_mapping->mapping.generatorManager.registerGenerator(new phdGenVideoPlayer());
	_mapping->mapping.generatorManager.registerGenerator(new phdGenFboShader());

	_mapping->mapping.generatorManager.registerGenerator(new phdGenSoundPlayer());

	_mapping->mapping.generatorManager.registerGenerator(new phdGenEllipse());
	_mapping->mapping.generatorManager.registerGenerator(new phdGenRectangle());
	_mapping->mapping.generatorManager.registerGenerator(new phdGenAura());

	//-------------------------------------------------------
	controlManager.setPopupMenu(&managerPopup);

	bEditingGrabberWarpers = false;
	bShowProjectors = true;
	bShowControls = true;

	outputWidth = ofGetWidth();
	outputHeight = ofGetHeight();

	_mapping->loadMappingFile("mappingBase.txt");
}

//--------------------------------------------------------------
void testApp::update(){

	ofSetWindowTitle(ofToString(ofGetFrameRate(), 2));
	//for(int i = 0; i < _track->clips.size(); i++) {
		//track.clips[i]->setDrawAngle((ofGetElapsedTimef() *  4.0 * (float)(i+1.0)));
	//}

	controlManager.update();

	ofSoundUpdate();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	ofSetColor(255,255,255,255);

	if(bEditingGrabberWarpers && _mapping != NULL) {
		if(_mapping->mapping.grabberManager.getCurGrabber() != NULL) _mapping->mapping.grabberManager.getCurGrabber()->update();
		_mapping->mapping.grabberManager.draw();
	}

	if(_mapping != NULL && _mapping->mapping.masterOutput != NULL && bShowProjectors && !bEditingGrabberWarpers) {
		_mapping->mapping.masterOutput->bind();
			controlManager.drawWarpers();
		_mapping->mapping.masterOutput->unbind();
	}

	if(bShowControls && !bEditingGrabberWarpers) controlManager.draw();

	managerPopup.draw();
	designerPopup.draw();
	outputPopup.draw();
	shapeInPopup.draw();
	shapeOutPopup.draw();
	clipPopup.draw();
	trackPopup.draw();
	mappingPopup.draw();

	if(_mapping != NULL && _mapping->mapping.masterOutput != NULL && _mapping->mapping.masterOutput->showSchema && bShowControls &&!bEditingGrabberWarpers) {
		ofSetColor(192,64,255,255);
		_mapping->mapping.updateScreenLinkSchemaPos();
		for(int i = 0; i < _mapping->mapping.screenLinkSchemaPos.size(); i += 2) {
			ofLine(_mapping->mapping.screenLinkSchemaPos[i], _mapping->mapping.screenLinkSchemaPos[i+1]);
		}
	}

	ofSetColor(255,255,255,255);
	//ofDrawBitmapString(msg,20,20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	string _strClips = "1234567890";
	int _auxClip = _strClips.find_first_of(key);
	//if(_auxClip > -1 && _auxClip < _track->clips.size()) {
		//_track->setCurClip(_auxClip);
	//}

	if(key == OF_KEY_F1) {

	} else if(key == OF_KEY_F2) { // show Controls

		bShowControls = !bShowControls;

	} else if(key == OF_KEY_F3) { // show Projectors

		bShowProjectors = !bShowProjectors;

	} else if(key == OF_KEY_F4) { // edit grabber warpers input

		bEditingGrabberWarpers = !bEditingGrabberWarpers;

		if(bEditingGrabberWarpers && _mapping != NULL) {
			controlManager.disableEvents();
			controlManager.setDrawMode(cmdmBoth);
			_mapping->mapping.grabberManager.enableEvents();
		} else {
			controlManager.enableEvents();
			_mapping->mapping.grabberManager.disableEvents();
		}

	} else if(key == OF_KEY_F5) { // toggle projectors warpers borders

		if(_mapping != NULL && bShowProjectors) {
			for(int i = 0; i < _mapping->mapping.projectors.size(); i++) {
				phdProjectorControl * _projector = (phdProjectorControl*) _mapping->mapping.projectors[i];

				if(_projector != NULL) {

					phdProjectorDrawMode _drawMode = _projector->getDrawMode();

					if(_drawMode == pdmContents) { // drawing contents?
						_projector->setDrawMode(pdmBoth);
						msg = "pdmBoth";
					} else {
						_projector->setDrawMode(pdmWarper);
						msg = "pdmWarper";
					}
				}
			}
		}

	} else if(key == OF_KEY_F6) { // toggle projectors draw contents

		if(_mapping != NULL && bShowProjectors) {
			for(int i = 0; i < _mapping->mapping.projectors.size(); i++) {
				phdProjectorControl * _projector = (phdProjectorControl*) _mapping->mapping.projectors[i];

				if(_projector != NULL) {

					phdProjectorDrawMode _drawMode = _projector->getDrawMode();

					if(_drawMode == pdmWarper) { // drawing border?
						_projector->setDrawMode(pdmBoth);
						msg = "pdmBoth";
					} else {
						_projector->setDrawMode(pdmContents);
						msg = "pdmContents";
					}
				}
			}
		}

	} else if(key == OF_KEY_F7) {

	} else if(key == OF_KEY_F8) {

	} else if(key == OF_KEY_F9) {

	} else if(key == OF_KEY_F10) {

	} else if(key == OF_KEY_F11) {

	} else if(key == OF_KEY_F12) {

	} else if(key == OF_KEY_LEFT) {

		if(bEditingGrabberWarpers && _mapping != NULL) {
			_mapping->mapping.grabberManager.setCurGrabberIndex(_mapping->mapping.grabberManager.getCurGrabberIndex()-1);
		}

	} else if(key == OF_KEY_RIGHT) {

		if(bEditingGrabberWarpers && _mapping != NULL) {
			_mapping->mapping.grabberManager.setCurGrabberIndex(_mapping->mapping.grabberManager.getCurGrabberIndex()+1);
		}

	}

	if(key == ']') {

		updateOutputWindowSize(outputWidth + 128, outputHeight);

	} else if(key == '[') {

		updateOutputWindowSize(outputWidth - 128, outputHeight);

	} else if(key == '}') {

		updateOutputWindowSize(outputWidth, outputHeight + 128);

	} else if(key == '{') {

		updateOutputWindowSize(outputWidth, outputHeight - 128);

	}

	if(key == 'u') {
	} else if(key == 'i') {
		for(int i = controlManager.size()-1; i > -1; i--) {
			if(controlManager[i]->getClassName() == "PHDDESIGNER") {
				phdDesigner * _designer = (phdDesigner*) controlManager[i];
				if(_designer != NULL && _designer->designerType == phdDesignerOutput) {
					for(int j = 0; j < _designer->schema.shapes.size(); j++) {
						_designer->schema.shapes[j]->translate(-10.0, 0.0);
					}
				}
			}
		}
	} else if(key == 'o') {
		for(int i = controlManager.size()-1; i > -1; i--) {
			if(controlManager[i]->getClassName() == "PHDDESIGNER") {
				phdDesigner * _designer = (phdDesigner*) controlManager[i];
				if(_designer != NULL && _designer->designerType == phdDesignerOutput) {
					for(int j = 0; j < _designer->schema.shapes.size(); j++) {
						_designer->schema.shapes[j]->translate(10.0, 0.0);
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::managerPopupClick(phdPopupMenuEventData &_eventData) {

	msg = "";
	if(_eventData.menu != NULL) {
		if(_eventData.item != NULL) {
			msg += _eventData.item->name;
		} else {
			msg += "click out popup";
		}
		msg += " " + _eventData.menu->callerTag;
	}
}

//--------------------------------------------------------------
void testApp::designerPopupClick(phdPopupMenuEventData &_eventData) {

	msg = "";
	if(_eventData.menu != NULL) {
		if(_eventData.item != NULL) {
			msg += _eventData.item->name;

			if(_eventData.item->id == PM_DESIGNER+0 && _eventData.menu->caller != NULL) {

			} else if(_eventData.item->id == PM_DESIGNER+1 && _eventData.menu->caller != NULL) { // show hide warp map

				phdDesigner * _designer = (phdDesigner*) _eventData.menu->caller;
				_designer->showSchema = !_designer->showSchema;

			} else if(_eventData.item->id == PM_DESIGNER+2 && _eventData.menu->caller != NULL) { // link slice to warp

				phdDesigner * _dsgnIn = (phdDesigner*) _eventData.menu->caller;

				phdShapeQuadWarp * _shapeIn = (phdShapeQuadWarp*) _dsgnIn->schema.shapes.getSelected();
				phdShapeQuadWarp * _shapeOut = NULL;

				for(int i = controlManager.size()-1; i > -1; i--) {
					if(controlManager[i]->getClassName() == "PHDDESIGNER") {

						phdDesigner * _dsgnOut = (phdDesigner*) controlManager[i];

						if(_dsgnOut->designerType == phdDesignerOutput) {

							_shapeOut = (phdShapeQuadWarp*)_dsgnOut->schema.shapes.getSelected();

							if(_shapeIn != NULL && _shapeOut != NULL && _dsgnOut->curLinkSchema != NULL) {

								_dsgnOut->curLinkSchema->addLink(_shapeIn, _dsgnIn, _shapeOut, _dsgnOut);

								for(int j = 0; j < _mapping->mapping.linkSchemas.size(); j++) {
									_mapping->mapping.linkSchemas[j]->updateFlags();
								}
							}
						}
					}
				}
			} else if(_eventData.item->id == PM_DESIGNER+3 && _eventData.menu->caller != NULL) { // delete warp quad

				phdDesigner * _designer = (phdDesigner*) _eventData.menu->caller;

				phdShapeQuadWarp * _selected = (phdShapeQuadWarp*) _designer->schema.shapes.getSelected();

				if(_selected != NULL) {
					_designer->schema.shapes.setSelected(NULL);
					_designer->schema.shapes.setFocused(NULL);

					for(int j = 0; j < _mapping->mapping.linkSchemas.size(); j++) {
						_mapping->mapping.linkSchemas[j]->delLinksFromQuadIn(_selected);
						_mapping->mapping.linkSchemas[j]->delLinksFromQuadOut(_selected);
						_mapping->mapping.linkSchemas[j]->updateFlags();
					}

					_designer->schema.shapes.delShape(_selected);
				}
			} else if(_eventData.item->id == PM_DESIGNER+4 && _eventData.menu->caller != NULL) { // adjust near vertices
				
				phdDesigner * _designer = (phdDesigner*) _eventData.menu->caller;

				_designer->schema.shapes.adjustNearVertices(5.0);

			} else if(_eventData.item->id == PM_DESIGNER+5 && _eventData.menu->caller != NULL) { // delete links from this warp quad

				phdDesigner * _designer = (phdDesigner*) _eventData.menu->caller;

				phdShapeQuadWarp * _selected = (phdShapeQuadWarp*) _designer->schema.shapes.getSelected();

				for(int j = 0; j < _mapping->mapping.linkSchemas.size(); j++) {
					_mapping->mapping.linkSchemas[j]->delLinksFromQuadIn(_selected);
					_mapping->mapping.linkSchemas[j]->delLinksFromQuadOut(_selected);
					_mapping->mapping.linkSchemas[j]->updateFlags();
				}

			} else if(_eventData.item->id == PM_DESIGNER+6 && _eventData.menu->caller != NULL) { // delete this designer

				if(_mapping != NULL) {
					int _index = _mapping->mapping.indexOfDesigner((phdDesigner*)_eventData.menu->caller);
					if(_index != -1) {

						phdDesigner * _designer = _mapping->mapping.designers[_index];

						// delete designer from list
						_mapping->mapping.designers.erase(_mapping->mapping.designers.begin() + _index);

						for(int i = 0; i < _designer->schema.shapes.size(); i++) {
							phdShapeQuadWarp * _shape = (phdShapeQuadWarp*) _designer->schema.shapes[i];
							for(int j = 0; j < _mapping->mapping.linkSchemas.size(); j++) {
								_mapping->mapping.linkSchemas[j]->delLinksFromQuadIn(_shape);
								_mapping->mapping.linkSchemas[j]->delLinksFromQuadOut(_shape);
								_mapping->mapping.linkSchemas[j]->updateFlags();
							}
						}

						_designer->setFocused(NULL);
						_designer->setSelected(NULL);
						_designer->curLinkSchema = NULL;
						_designer->schema.shapes.freeShapes();

						_eventData.menu->parent = NULL;
						_eventData.menu->setOwner(NULL);

						controlManager.setFocusedControl(NULL, false);
						controlManager.setSelectedControl(NULL, false);
						controlManager.delControl(_designer);
					}
				}
			}
		} else {
			msg += "click out popup";
		}
		msg += " " + _eventData.menu->callerTag;
	}
}

//--------------------------------------------------------------
void testApp::outputPopupClick(phdPopupMenuEventData &_eventData) {

	msg = "";
	if(_eventData.menu != NULL) {
		if(_eventData.item != NULL) {
			msg += _eventData.item->name;

			if(_eventData.item->id == PM_OUTPUT+1 && _eventData.menu->caller != NULL) { // show hide warp map

				phdDesigner * _designer = (phdDesigner*) _eventData.menu->caller;
				_designer->showSchema = !_designer->showSchema;

			} else if(_eventData.item->id == PM_OUTPUT+2 && _eventData.menu->caller != NULL) { // delete warp quad

				phdDesigner * _designer = (phdDesigner*) _eventData.menu->caller;

				phdShapeQuadWarp * _selected = (phdShapeQuadWarp*) _designer->schema.shapes.getSelected();

				if(_selected != NULL) { // delete shape and all links

					_designer->schema.shapes.setSelected(NULL);
					_designer->schema.shapes.setFocused(NULL);

					for(int j = 0; j < _mapping->mapping.linkSchemas.size(); j++) {
						_mapping->mapping.linkSchemas[j]->delLinksFromQuadIn(_selected);
						_mapping->mapping.linkSchemas[j]->delLinksFromQuadOut(_selected);
						_mapping->mapping.linkSchemas[j]->updateFlags();
					}

					_designer->schema.shapes.delShape(_selected);
				}

			} else if(_eventData.item->id == PM_OUTPUT+3 && _eventData.menu->caller != NULL) { // adjust vertices
				
				phdDesigner * _designer = (phdDesigner*) _eventData.menu->caller;

				_designer->schema.shapes.adjustNearVertices(5.0);

			} else if(_eventData.item->id == PM_OUTPUT+4 && _eventData.menu->caller != NULL) { // clear links for selected quad warp

				phdDesigner * _designer = (phdDesigner*) _eventData.menu->caller;

				phdShapeQuadWarp * _selected = (phdShapeQuadWarp*) _designer->schema.shapes.getSelected();

				if(_selected != NULL && _designer->curLinkSchema) {
				for(int j = 0; j < _mapping->mapping.linkSchemas.size(); j++) {
					_mapping->mapping.linkSchemas[j]->delLinksFromQuadIn(_selected);
					_mapping->mapping.linkSchemas[j]->delLinksFromQuadOut(_selected);
					_mapping->mapping.linkSchemas[j]->updateFlags();
				}
				}
			}
		} else {
			msg += "click out popup";
		}
		msg += " " + _eventData.menu->callerTag;
	}
}

//--------------------------------------------------------------
void testApp::shapeInPopupClick(phdPopupMenuEventData &_eventData) {

	msg = "";
	if(_eventData.menu != NULL) {
		if(_eventData.item != NULL) {
			msg += _eventData.item->name;

			if(_eventData.item->id == PM_SHAPEIN+0 && _eventData.menu->caller != NULL) {

			}
		} else {
			msg += "click out popup";
		}
		msg += " " + _eventData.menu->callerTag;
	}
}

//--------------------------------------------------------------
void testApp::shapeOutPopupClick(phdPopupMenuEventData &_eventData) {

	msg = "";
	if(_eventData.menu != NULL) {
		if(_eventData.item != NULL) {
			msg += _eventData.item->name;
			if(_eventData.item->id == PM_SHAPEOUT+0 && _eventData.menu->caller != NULL) {

			}
		} else {
			msg += "click out popup";
		}
		msg += " " + _eventData.menu->callerTag;
	}
}

//--------------------------------------------------------------
void testApp::mirrorPopupClick(phdPopupMenuEventData &_eventData) {

	msg = "";
	if(_eventData.menu != NULL) {
		if(_eventData.item != NULL) {
			msg += _eventData.item->name;
			if(_eventData.menu != NULL && _eventData.menu->caller != NULL && _mapping->mapping.curLinkSchema) {
				phdShapeQuadWarp * _shape = (phdShapeQuadWarp*) _eventData.menu->caller;
				phdWarpLink * _link =_mapping->mapping.curLinkSchema->getLinkFromQuadOut(_shape);
				if(_link != NULL) {
					int _mirrorIndex = _eventData.item->id - PM_MIRROR;
					_link->horMirror = (_mirrorIndex == 1 || _mirrorIndex == 3);
					_link->verMirror = (_mirrorIndex == 2 || _mirrorIndex == 3);
					_link->updated = true;
				}
			}

		} else {
			msg += "click out popup";
		}
		msg += " " + _eventData.menu->callerTag;
	}
}

//--------------------------------------------------------------
void testApp::shapeModePopupClick(phdPopupMenuEventData &_eventData) {

	msg = "";
	if(_eventData.menu != NULL) {
		if(_eventData.item != NULL) {
			msg += _eventData.item->name;
			if(_eventData.menu != NULL && _eventData.menu->caller != NULL) {
				phdShapeQuadWarp * _shape = (phdShapeQuadWarp*) _eventData.menu->caller;
				int _ftIndex = _eventData.item->id - PM_SHAPEMODE;
				if(_shape != NULL) _shape->faceType = (phdWarpFaceType)_ftIndex;
			}
		} else {
			msg += "click out popup";
		}
		msg += " " + _eventData.menu->callerTag;
	}
}

//--------------------------------------------------------------
void testApp::clipPopupClick(phdPopupMenuEventData &_eventData) {

	msg = "";
	if(_eventData.menu != NULL) {
		if(_eventData.item != NULL) {
			msg += _eventData.item->name;
			if(_eventData.item->id == PM_CLIP + 0) {	// file base generator

				ofFileDialogResult fileDialogResult = ofSystemLoadDialog("Escolha um arquivo", false, "");
				if(fileDialogResult.bSuccess){

					phdClip * _clip = (phdClip*) _eventData.menu->caller;
					phdGenerator * _generator = NULL;

					string _ext = ofToUpper(ofFilePath().getFileExt(fileDialogResult.getName()));
					string _name = fileDialogResult.fileName;

					if(_ext == "JPG" || _ext == "JPEG" || _ext == "PNG" || _ext == "BMP") {
						_generator = new phdGenImage();
					} else if(_ext == "MOV" || _ext == "MP4" || _ext == "MPG" || _ext == "MPEG" || _ext == "AVI") {
						_generator = new phdGenVideoPlayer();
					} else if(_ext == "WAV" || _ext == "MP3" || _ext == "WMA") {
						_generator = new phdGenSoundPlayer();
					} else if(_ext == "FRAG" || _ext == "FS") {
						_generator = new phdGenFboShader();
						_name = ofFilePath().removeExt(_name);
					}

					if(_generator != NULL) {
						_generator->setupFromFile(_name);
						_clip->setGenerator(_generator);
						msg += " " + fileDialogResult.getPath() + " " + fileDialogResult.getName();
					} else {
						msg += "tipo de arquivo desconhecido";
					}
				}
			} else if(_eventData.item->id == PM_CLIP + 1) { // internal generator - not file based

				if(_mapping != NULL) {
					ofRemoveListener(generatorsPopup.popEvent, this, &testApp::generatorsPopupClick);
					generatorsPopup.freeItems();
					for(int i = 0; i < _mapping->mapping.generatorManager.size(); i++) {
						if(_mapping->mapping.generatorManager[i] != NULL && !_mapping->mapping.generatorManager[i]->isFileBase()) {
							generatorsPopup.addItem(_mapping->mapping.generatorManager[i]->getClassName(), PM_GENERATORS + i, NULL, false);
						}
					}
					ofAddListener(generatorsPopup.popEvent, this, &testApp::generatorsPopupClick);
				}

			} else if(_eventData.item->id == PM_CLIP + 2) { // grabber

				if(_mapping != NULL) {
					ofRemoveListener(grabbersPopup.popEvent, this, &testApp::grabbersPopupClick);
					grabbersPopup.freeItems();
					for(int i = 0; i < _mapping->mapping.grabberManager.size(); i++) {
						if(_mapping->mapping.grabberManager[i] != NULL) {
							grabbersPopup.addItem(_mapping->mapping.grabberManager[i]->getClassName(), PM_GRABBERS + i, NULL, false);
						}
					}
					ofAddListener(grabbersPopup.popEvent, this, &testApp::grabbersPopupClick);
				}

			} else if(_eventData.item->id == PM_CLIP + 3) { // delete clip

				phdClip * _clip = (phdClip*) _eventData.menu->caller;
				phdTrackControl * _track = (phdTrackControl*) _eventData.menu->owner;

				_clip->setGenerator(NULL);

				if(_track->getClipTrack()->getCurClip() == _clip) _track->getClipTrack()->resetClips();

				_track->getClipTrack()->clips.delItem(_clip); _clip = NULL;
				_track->updateHotspotsArea();
			}
		} else {
			msg += "click out popup";
		}
		msg += " " + _eventData.menu->callerTag;
	}
}

//--------------------------------------------------------------
void testApp::generatorsPopupClick(phdPopupMenuEventData &_eventData) {

	msg = "";
	if(_eventData.menu != NULL) {
		if(_eventData.item != NULL) {
			if(_mapping != NULL) {
				phdGenerator * _generator = _mapping->mapping.generatorManager.findByClassName(_eventData.item->name);
				if(_generator != NULL) {
					phdClip * _clip = (phdClip*) _eventData.menu->caller;
					_clip->setGenerator(_generator->createInstanceSameClass());
				}
			}
			msg += _eventData.item->name;
		} else {
			msg += "click out popup";
		}
		msg += " " + _eventData.menu->callerTag;
	}
}
	
//--------------------------------------------------------------
void testApp::grabbersPopupClick(phdPopupMenuEventData &_eventData) {

	msg = "";
	if(_eventData.menu != NULL) {
		if(_eventData.item != NULL) {
			if(_mapping != NULL) {
				phdGrabberBase * _grabber = _mapping->mapping.grabberManager.findByClassName(_eventData.item->name);
				if(_grabber != NULL) {
					phdClip * _clip = (phdClip*) _eventData.menu->caller;
					_clip->setGenerator((phdGenerator*) _grabber);
				}
			}
			msg += _eventData.item->name;
		} else {
			msg += "click out popup";
		}
		msg += " " + _eventData.menu->callerTag;
	}
}
	
//--------------------------------------------------------------
void testApp::trackPopupClick(phdPopupMenuEventData &_eventData) {

	msg = "";
	if(_eventData.menu != NULL) {
		if(_eventData.item != NULL) {
			if(_eventData.item->id == PM_TRACK + 0) {	// insert new CLIP on this track

				phdTrackControl * _track = (phdTrackControl*) _eventData.menu->caller;

				if(_track != NULL) {
					_track->getClipTrack()->clips.addItem(new phdClip());
					_track->updateHotspotsArea();
				}
			} else if(_eventData.item->id == PM_TRACK + 1) {	// insert new FILTER on this track

			} else if(_eventData.item->id == PM_TRACK + 2) {	// insert new TRANSICTION on this track

			} else if(_eventData.item->id == PM_TRACK + 3) {	// delete this TRACK

				phdTrackControl * _track = (phdTrackControl*) _eventData.menu->caller;

				if(_track != NULL) {

					int _index = _mapping->mapping.indexOfClipControl(_track);

					if(_index != -1) {
						_mapping->mapping.clips.erase(_mapping->mapping.clips.begin() + _index);

						_track->setPopupMenu(NULL);
						_track->setClipsPopupMenu(NULL);
						_track->getClipTrack()->resetClips();

						_eventData.menu->caller = NULL;
						_eventData.menu->parent = NULL;
						_eventData.menu->setOwner(NULL);

						controlManager.setFocusedControl(NULL, false);
						controlManager.setSelectedControl(NULL, false);
						controlManager.delControl(_track);
					}
				}

			}
			msg += _eventData.item->name;
		} else {
			msg += "click out popup";
		}
		msg += " " + _eventData.menu->callerTag;
	}
}

//--------------------------------------------------------------
void testApp::mappingPopupClick(phdPopupMenuEventData &_eventData) {

	msg = "";
	if(_eventData.menu != NULL) {
		if(_eventData.item != NULL) {
			msg += _eventData.item->name;
		} else {
			msg += "click out popup";
		}
		msg += " " + _eventData.menu->callerTag;
	}
}

//--------------------------------------------------------------
void testApp::updateOutputWindowSize(int _width, int _height) {
	if((_width >= 512 && _height >= 384) && (outputWidth != _width || outputHeight != _height)) {
		outputWidth = _width;
		outputHeight = _height;
		ofSetWindowShape(_width, _height);
		ofSetWindowPosition(0,0);
	}
}
