#include "phdMapping.h"

//--------------------------------------------------------------------------------------------------------------

phdMapping::phdMapping() {
	masterOutput = NULL;
	showLinks = false;
}

phdMapping::~phdMapping() {
}

void phdMapping::freeItems() {

	curLinkSchema = NULL;

	// free link schemas
	for (vector<phdWarpLinkSchema*>::iterator i = linkSchemas.begin(); i != linkSchemas.end(); i++) {
		delete(*i); (*i) = NULL;
	}
	linkSchemas.clear();

	// free master output
	if(masterOutput != NULL) masterOutput->schema.shapes.freeShapes();

	masterOutput = NULL;

	// free designers
	for (vector<phdDesigner*>::iterator i = designers.begin(); i != designers.end(); i++) {
		//(*i)->schema.shapes.freeShapes();
		//delete(*i); (*i) = NULL;
	}
	designers.clear();

	// free clips
	for (vector<phdControlBase*>::iterator i = clips.begin(); i != clips.end(); i++) {
		//delete(*i); (*i) = NULL;
	}
	clips.clear();

	// free all projectors
	projectors.clear();
}

phdDesigner * phdMapping::findDesignerByQuadWarp(phdShapeQuadWarp * _warp) {
	for(int i = 0; i < designers.size(); i++) {
		for(int j = 0; j < designers[i]->schema.size(); j++) {
			if(designers[i]->schema[j] == _warp) return designers[i];
		}
	}
	return NULL;
}

int phdMapping::indexOfLinkSchema(phdWarpLinkSchema * _schema) {
	for(int i = 0; i < linkSchemas.size(); i++) {
		if(linkSchemas[i] == _schema) return i;
	}
	return - 1;
}

phdWarpLinkSchema * phdMapping::findLinkSchemaByName(string _name) {
	for(int i = 0; i < linkSchemas.size(); i++) {
		if(linkSchemas[i]->name == _name) return linkSchemas[i];
	}
	return NULL;
}

phdWarpLinkSchema * phdMapping::addLinkSchema(phdWarpLinkSchema * _schema) {
	if(_schema != NULL && indexOfLinkSchema(_schema) == -1) {
		linkSchemas.push_back(_schema);
	}
	return _schema;
}

void phdMapping::updateScreenLinkSchemaPos() {
	screenLinkSchemaPos.clear();
	if(curLinkSchema != NULL) {
		for(int i = 0; i < curLinkSchema->links.size(); i++) {
			phdShapeQuadWarp * _in  = curLinkSchema->links[i]->quadIn;
			phdShapeQuadWarp * _out = curLinkSchema->links[i]->quadOut;

			ofPoint _pIn; _pIn.set(0);
			ofPoint _pOut; _pOut.set(0);
			if(_in != NULL) {
				_pIn.x = _in->gimbal.cx;
				_pIn.y = _in->gimbal.cy;
			}
			if(_out != NULL) {
				_pOut.x = _out->gimbal.cx;
				_pOut.y = _out->gimbal.cy;
			}

			phdDesigner * _dgnIN  = findDesignerByQuadWarp(_in);
			if(_dgnIN != NULL) {

				phdDesigner * _dgnOUT = masterOutput; // output is masterOutput? if not find from quadOut

				if(_dgnOUT != NULL) {

					if(_dgnOUT->schema.indexOf(_out) == -1)	_dgnOUT = findDesignerByQuadWarp(_out);

					_pIn  = _dgnIN->viewport.getFromGlobalToViewport(_pIn.x, _pIn.y);
					_pOut = _dgnOUT->viewport.getFromGlobalToViewport(_pOut.x, _pOut.y);
					screenLinkSchemaPos.addVertex(_pIn);
					screenLinkSchemaPos.addVertex(_pOut);
					ofLine(_pIn, _pOut);
				}
			}
		}
	}
}

void phdMapping::setCurLinkSchema(phdWarpLinkSchema * _value) {
	if(curLinkSchema != _value) {
		curLinkSchema = _value;
		if(masterOutput != NULL) masterOutput->curLinkSchema = _value;
		updateScreenLinkSchemaPos();
		if(curLinkSchema != NULL) curLinkSchema->updateFlags(); // mark updates
	}
}

void phdMapping::setCurSelDesigner(phdDesigner * _dsgn) {

	int _index = indexOfDesigner(_dsgn);

	if(_index != -1) {
		for(int i = 0; i < designers.size(); i++) {
			if(designers[i] != NULL) designers[i]->setSelected(false);
		}
		designers[_index]->setSelected(true);
	}
}

int phdMapping::indexOfDesigner(phdDesigner * _designer) {
	for(int i = 0; i < designers.size(); i++) {
		if(designers[i] != NULL && designers[i] == _designer) return i;
	}
	return -1;
}

phdDesigner * phdMapping::findDesignerByName(string _name) {
	for(int i = 0; i < designers.size(); i++) {
		if(designers[i] != NULL && designers[i]->getControlName() == _name) return designers[i];
	}
	return NULL;
}

phdDesigner * phdMapping::findDesignerSelected() {
	for(int i = 0; i < designers.size(); i++) {
		if(designers[i] != NULL && designers[i]->getSelected()) return designers[i];
	}
	return NULL;
}

phdDesigner * phdMapping::addDesigner(phdDesigner * _designer) {
	if(_designer != NULL && indexOfDesigner(_designer) == -1) {
		designers.push_back(_designer);
	}
	return _designer;
}

int phdMapping::indexOfClipControl(phdControlBase * _clip) {
	for(int i = 0; i < clips.size(); i++) {
		if(clips[i] == _clip) return i;
	}
	return NULL;
}

void phdMapping::saveToFile(string _fileName) {

	fileName = _fileName;

	ofstream fout; // declare a file stream
	fout.open(_fileName.c_str() ); // open your text file

	string _line = "";
	vector<string> _strings;

	//--------------------------------------------------------------
	// save grabbers
	for(int i = 0; i < grabberManager.size(); i++) {
		string _s = "GRABBERCLASS|" + grabberManager[i]->getClassName() + "|\n";
		fout.write(ofToUpper(_s).c_str(), _s.size());
	}

	//--------------------------------------------------------------
	// save generators
	for(int i = 0; i < generatorManager.size(); i++) {
		string _s = "GENERATORCLASS|" + generatorManager[i]->getClassName() + "|\n";
		fout.write(ofToUpper(_s).c_str(), _s.size());
	}

	//--------------------------------------------------------------
	// save filters
	for(int i = 0; i < filterManager.size(); i++) {
		string _s = "FILTERCLASS|" + filterManager[i]->getClassName() + "|FILENAME|" + filterManager[i]->getFileName() + "|\n";
		fout.write(ofToUpper(_s).c_str(), _s.size());
	}

	//--------------------------------------------------------------
	for(int i = 0; i < clips.size(); i++) {
		_strings.clear();
		clips[i]->saveToStrings(_strings,"CLIPITEM");
		for(int s = 0; s < _strings.size(); s++) fout.write(ofToUpper(_strings[s]).c_str(), _strings[s].size());
	}

	if(masterOutput != NULL) {
		_strings.clear(); masterOutput->saveToStrings(_strings, "MASTEROUTPUT");
		for(int s = 0; s < _strings.size(); s++) fout.write(ofToUpper(_strings[s]).c_str(), _strings[s].size());
	}

	_line = "DESIGNERCOUNT|" + ofToString(designers.size()) + "|\n";
	fout.write(ofToUpper(_line).c_str(), _line.size());

	for(int i = 0; i < designers.size(); i++) {
		_strings.clear(); designers[i]->saveToStrings(_strings, "DESIGNER");
		for(int s = 0; s < _strings.size(); s++) fout.write(ofToUpper(_strings[s]).c_str(), _strings[s].size());
	}

	_line = "LINKSCHEMACOUNT|" + ofToString(linkSchemas.size()) + "|\n";
	fout.write(ofToUpper(_line).c_str(), _line.size());

	for(int i = 0; i < linkSchemas.size(); i++) {

		_line = "LINKSCHEMA|" + linkSchemas[i]->name + "|" + ofToString(linkSchemas[i]->links.size()) + "|\n";
		fout.write(ofToUpper(_line).c_str(), _line.size());
			
		for(int j = 0; j < linkSchemas[i]->links.size(); j++) {

			phdDesigner * _dgnIN = NULL;
			if(linkSchemas[i]->links[j]->quadIn != NULL) _dgnIN = findDesignerByQuadWarp(linkSchemas[i]->links[j]->quadIn);

			if(_dgnIN != NULL) {

				phdDesigner * _dgnOUT = masterOutput;
				if(_dgnOUT != NULL) {
					if(linkSchemas[i]->links[j]->quadOut != NULL && _dgnOUT->schema.indexOf(linkSchemas[i]->links[j]->quadOut) == -1) {
						_dgnOUT = findDesignerByQuadWarp(linkSchemas[i]->links[j]->quadOut);
					}
				} else {
					_dgnOUT = findDesignerByQuadWarp(linkSchemas[i]->links[j]->quadOut);
				}

				if(_dgnOUT != NULL) {
					_line = "LINK|";
					if(linkSchemas[i]->links[j]->quadIn != NULL) {
						_line += _dgnIN->getControlName() + "|";
						_line += linkSchemas[i]->links[j]->quadIn->name + "|";
					} else {
						_line += "||";
					}
					if(linkSchemas[i]->links[j]->quadOut != NULL) {
						_line += _dgnOUT->getControlName() + "|";
						_line += linkSchemas[i]->links[j]->quadOut->name + "|";
					} else {
						_line += "||";
					}
					_line += linkSchemas[i]->links[j]->horMirror ? "SIM|" : "NAO|";
					_line += linkSchemas[i]->links[j]->verMirror ? "SIM|" : "NAO|";

					_line += "\n";
					fout.write(ofToUpper(_line).c_str(), _line.size());
				}
			}
		}
	}

	for(int i = 0; i < projectors.size(); i++) {
		_strings.clear();
		projectors[i]->saveToStrings(_strings,"PROJECTOR");
		for(int s = 0; s < _strings.size(); s++) fout.write(ofToUpper(_strings[s]).c_str(), _strings[s].size());
	}

	fout.close(); // close file stream
}

//--------------------------------------------------------------------------------------------------------------
void phdMapping::loadFromFile(string _fileName) {

	// set mapping file name
	fileName = _fileName;

	if(masterOutput != NULL) {
		masterOutput->schema.shapes.setFocused(NULL);
		masterOutput->schema.shapes.setSelected(NULL);
	}

	freeItems();

	ifstream _fin; //declare a file stream  
	_fin.open(_fileName.c_str() ); //open your text file  
     
	vector<string> _data; //declare a vector of strings to store data
	_data.clear();
      
	while(_fin != NULL) { //as long as theres still text to be read
		string _str = ""; //declare a string for storage  
		getline(_fin, _str); //get a line from the file, put it in the string
		if(_str != "") {
			_data.push_back(ofToUpper(_str)); //push the string onto a vector of strings  
			//printf(_str.c_str()); printf("\n");
		}
	}

	_fin.close();

	if(_data.size() == 0) return; // empty file

	phdDesigner * _designer = NULL;
	phdWarpShapeSchema * _shapeSchema = NULL;
	phdWarpLinkSchema * _linkSchema = NULL;
	phdShapeQuadWarp * _shape = NULL;
	phdTrackControl * _track = NULL;
	//phdProjectorControl * _projector = NULL;

	for(int i = 0; i < _data.size(); i++) {

		vector<string> _items = ofSplitString(_data[i], "|", true, true);

		if(_items.size() > 0) {
			if(_items[0] == "DESIGNERCOUNT") {
					
				// resize designer array

			} else if(_items[0] == "CLIP" && _items.size() == 7) {

				if(_items[1] == "PHDTRACKCONTROL") {

					_track = new phdTrackControl();

					_track->getClipTrack()->filters.setAutoFreeItems(false);

					_track->setFilterManager(&filterManager); // links with filterManager

					clips.push_back(_track);

				} else if(_items[1] == "PHDCLIPCONTROL") {

				}

				_track->setName(_items[2]);
				_track->setPosition(ofToFloat(_items[3]), ofToFloat(_items[4]));
				_track->setDimension(ofToFloat(_items[5]), ofToFloat(_items[6]));

			} else if(_items[0] == "CLIPITEM" && _track != NULL && _items.size() > 2) { // minimum 3

				if(_items[1] == "GENERATOR") {

					phdGenerator * _generatorClass = generatorManager.findByClassName(_items[2]);

					string _clipGrabberClass = "NONE";

					if(_generatorClass != NULL) {

						phdGenerator * _generatorInstance = _generatorClass->createInstanceSameClass();

						string _setupString = "";
						for(int s = 3; s < _items.size(); s++) {
							_setupString += _items[s] + "|";

							if(_items[s] == "GRABBER" && s+1 < _items.size()) {
								_clipGrabberClass = _items[s+1];
							}

						}
						_generatorInstance->setSetupString(_setupString);

						phdClip * _newClip = new phdClip(_generatorInstance);

						if(_clipGrabberClass != "NONE") {
							_newClip->grabber = grabberManager.findByClassName(_clipGrabberClass);
						}

						_track->getClipTrack()->clips.addItem(_newClip);
					}
				} else if(_items[1] == "GRABBER") {

					phdGrabberBase * _genGrabber = grabberManager.findByClassName(_items[2]);

					if(_genGrabber != NULL) {
						_track->getClipTrack()->clips.addItem(new phdClip((phdGenerator*) _genGrabber));
					}
				}

			} else if(_items[0] == "MASTEROUTPUT" && _items.size() == 8) {
				
				//_designer = &masterOutput;
				_designer = NULL;
				_designer = (phdDesigner*) new phdDesigner();
				_designer->designerType = phdDesignerOutput;
				_designer->setControlName(ofToUpper(_items[1]));

				masterOutput = _designer;

				float _x = ofToFloat(_items[2]);
				float _y = ofToFloat(_items[3]);
				float _w = ofToFloat(_items[4]);
				float _h = ofToFloat(_items[5]);

				_designer->setup(_x,_y,_w,_h, NULL, ofToFloat(_items[6]), ofToFloat(_items[7]));

			} else if(_items[0] == "DESIGNER" && _items.size() == 8) {
				
				_designer = NULL;
				_designer = (phdDesigner*) addDesigner(new phdDesigner());
				_designer->designerType = phdDesignerInput;
				_designer->setControlName(ofToUpper(_items[1]));

				float _x = ofToFloat(_items[2]);
				float _y = ofToFloat(_items[3]);
				float _w = ofToFloat(_items[4]);
				float _h = ofToFloat(_items[5]);

				_designer->setup(_x,_y,_w,_h, NULL, ofToFloat(_items[6]), ofToFloat(_items[7]));

			} else if(_items[0] == "SHAPESCHEMA" && _items.size() == 3 && _designer != NULL) {
				
				_shapeSchema = &_designer->schema;
				_shapeSchema->name = _items[1];

			} else if(_items[0] == "SHAPE" && _items.size() == 3 && _designer != NULL) {//_shapeSchema != NULL) {

				_shape = _designer->schema.addShape(_items[1]);
				//_shape = _shapeSchema->addShape(_items[1]);
				_shape->faceType = (phdWarpFaceType) ofToInt(_items[2]);

			} else if(_items[0] == "GIMBAL" && _items.size() == 8 && _shape != NULL) {

				_shape->gimbal.angle = ofToFloat(_items[1]);
				_shape->gimbal.cx = ofToFloat(_items[2]);
				_shape->gimbal.cy = ofToFloat(_items[3]);
				_shape->gimbal.sx = ofToFloat(_items[4]);
				_shape->gimbal.sy = ofToFloat(_items[5]);
				_shape->gimbal.w = ofToFloat(_items[6]);
				_shape->gimbal.h = ofToFloat(_items[7]);

			} else if(_items[0] == "VERTEXCOUNT" && _items.size() > 3 && _shape != NULL) {

				_shape->vertices.vertices.reserve(ofToInt(_items[1]));
				
				for(int k = 2; k < _items.size()-1; k += 2) {
					_shape->addVertex(ofToFloat(_items[k]), ofToFloat(_items[k+1]));
				}

			} else if(_items[0] == "LINKSCHEMACOUNT") {

			} else if(_items[0] == "LINKSCHEMA" && _items.size() == 3) {

				_linkSchema = addLinkSchema(new phdWarpLinkSchema());
				_linkSchema->name = _items[1];

			} else if(_items[0] == "LINK" && _items.size() == 7 && _linkSchema != NULL) {

				phdDesigner * _dgnIN  = findDesignerByName(_items[1]);
				phdDesigner * _dgnOUT = findDesignerByName(_items[3]);
				if(_dgnOUT == NULL) _dgnOUT = masterOutput;

				if(_dgnIN != NULL && _dgnOUT != NULL) {

					phdShapeQuadWarp * _quadIN  = _dgnIN->schema.findByShapeName(_items[2]);
					phdShapeQuadWarp * _quadOUT = _dgnOUT->schema.findByShapeName(_items[4]);

					phdWarpLink * _link = _linkSchema->addLink(_quadIN, _dgnIN, _quadOUT, _dgnOUT);
					_link->horMirror = (_items[5] == "SIM");
					_link->verMirror = (_items[6] == "SIM");
					_link->updated = true;
				}

			} else if(_items[0] == "PROJECTOR") {

				if(_items[1] == "PHDPROJECTOR") {

					phdProjectorControl * _projector = new phdProjectorControl();

					// make warper setup string
					string _strWarper = "W|";
					for(int s = 3; s < _items.size(); s++) {
						_strWarper += _items[s] + "|";
					}

					_projector->warper.setAsString(_strWarper);

					projectors.push_back(_projector);

				}

			} else if(_items[0] == "GRABBERCLASS" && _items.size() > 1) {

				if(_items[1] == "PHDPS3EYEGRABBER") {

					phdPS3EyeGrabber * _grabber = (phdPS3EyeGrabber*) grabberManager.findByClassName("PHDPS3EYEGRABBER");

					if(_grabber == NULL) {
						_grabber = (phdPS3EyeGrabber*) grabberManager.registerGrabber(new phdPS3EyeGrabber());
						_grabber->configureGrabber();
					}

				} else if(_items[1] == "PHDKINECTGRABBER") {

					phdKinectGrabber * _grabber = (phdKinectGrabber*) grabberManager.findByClassName("PHDKINECTGRABBER");

					if(_grabber == NULL) {
						_grabber = (phdKinectGrabber*) grabberManager.registerGrabber(new phdKinectGrabber());
						_grabber->configureGrabber();
					}
				}

			} else if(_items[0] == "FILTERCLASS" && _items.size() > 1) {

				if(_items[1] == "PHDFILTER") { // filter class

					if(_items[2] == "FILENAME") { // filter params
						phdFilter * _filter = filterManager.registerFilter(new phdFilter());
						_filter->setupFromFile(_items[3]);
					}
				}

			} else if(_items[0] == "GENERATORCLASS" && _items.size() > 1) {

				/*phdGenerator * _generatorClass = NULL;

				if(_items[1] == "PHDGENIMAGE") {

					_generatorClass = new phdGenImage();

				} else if(_items[1] == "PHDGENVIDEOPLAYER") {

					_generatorClass = new phdGenVideoPlayer();

				} else if(_items[1] == "PHDGENFBOSHADER") {

					_generatorClass = new phdGenFboShader();

				} else if(_items[1] == "PHDGENELLIPSE") {

					_generatorClass = new phdGenEllipse();

				} else if(_items[1] == "PHDGENRECTANGLE") {

					_generatorClass = new phdGenRectangle();

				} else if(_items[1] == "PHDGENFBO") {

					_generatorClass = new phdGenFbo();

				} else if(_items[1] == "PHDGENAURA") {

					_generatorClass = new phdGenAura();

				} else if(_items[1] == "PHDGENGRABBER") {

					// grabbers are generators, so dont need to create anything
				}

				if(_generatorClass != NULL) {
					generatorManager.registerGenerator(_generatorClass);
				}*/
			}
		}
	}

	if(_linkSchema != NULL) {
		curLinkSchema = _linkSchema;
	} else {
		curLinkSchema = addLinkSchema(new phdWarpLinkSchema());
		curLinkSchema->name = "LINKS";
	}
}

//--------------------------------------------------------------------------------------------------------------
phdMappingControl::phdMappingControl() {
	setDimension(300,75);
	mapping.owner = this;
	mapping.showLinks = true;
}

phdMappingControl::~phdMappingControl() {

}

void phdMappingControl::setPosition(float _x, float _y) {
	setX(_x);
	setY(_y);
	updateHotspotsArea();
}

void phdMappingControl::setDimension(float _width, float _height) {
	setWidth(300); setHeight(75); // fixed size
	updateHotspotsArea();
}

void phdMappingControl::update() {
}

void phdMappingControl::draw() {

	ofPushStyle();

	float _titleW = mapping.fileName.length() * 8.0 + 8.0;
	if(hotTitleBar.w != _titleW) {
		updateHotspotsArea();
		hotTitleBar.w = _titleW;
	}

	ofFill();
	if(selected) {
		drawFilledBorderRectangle(hotTitleBar.x, hotTitleBar.y, hotTitleBar.w, hotTitleBar.h, ofColor(120,192,255,200), ofColor(255,255,255,255));
	} else {
		drawFilledBorderRectangle(hotTitleBar.x, hotTitleBar.y, hotTitleBar.w, hotTitleBar.h, ofColor(160,92,220,200), ofColor(255,255,255,255));
	}

	ofSetColor(255,255,128,255);
	ofDrawBitmapString(mapping.fileName, hotTitleBar.x + 6.0, hotTitleBar.y + 10.0);

	for(int i = 0; i < hotspots.size(); i++) {
		if(hitTestInfo.hotspotIndex == i) {
			drawFilledBorderRectangle(hotspots[i].x, hotspots[i].y, hotspots[i].w, hotspots[i].h, ofColor(160,92,220,200), ofColor(255,255,255,255));
			ofDrawBitmapString(hotLabels[i], hotspots[i].x + 10.0, hotspots[i].y + 18.0);
		} else {
			drawFilledBorderRectangle(hotspots[i].x, hotspots[i].y, hotspots[i].w, hotspots[i].h, ofColor(192,128,255,100), ofColor(162,162,96,255));
			ofDrawBitmapString(hotLabels[i], hotspots[i].x + 10.0, hotspots[i].y + 18.0);
		}
	}

	for(int i = 0; i < hotSchemas.size(); i++) {
		if(hitTestInfo.schemaIndex == i) {
			drawFilledBorderRectangle(hotSchemas[i].x, hotSchemas[i].y, hotSchemas[i].w, hotSchemas[i].h, ofColor(160,92,220,200), ofColor(255,255,255,255));
			ofDrawBitmapString(strSchemas[i], hotSchemas[i].x + 4.0, hotSchemas[i].y + 10.0);
		} else {
			drawFilledBorderRectangle(hotSchemas[i].x, hotSchemas[i].y, hotSchemas[i].w, hotSchemas[i].h, ofColor(192,128,255,100), ofColor(162,162,96,255));
			ofDrawBitmapString(strSchemas[i], hotSchemas[i].x + 4.0, hotSchemas[i].y + 10.0);
		}
		if(i == hitTestInfo.selectedSchemaIndex) {
			drawFilledBorderRectangle(hotSchemas[i].x, hotSchemas[i].y, hotSchemas[i].w, hotSchemas[i].h, ofColor(128,64,192,100), ofColor(255,162,96,255));
			ofDrawBitmapString(strSchemas[i], hotSchemas[i].x + 4.0, hotSchemas[i].y + 10.0);
		}
	}

	ofPopStyle();
}

void phdMappingControl::updateHotspotsArea() {

	hotTitleBar.setup(getX(), getY(), getWidth(), 15);

	float _x = getX();
	float _y = getY() + hotTitleBar.h;
	float _w = getWidth() / 2.0;
	float _h = (getHeight() - hotTitleBar.h) / 2.0;

	while(hotspots.size() < nLabels) hotspots.push_back(phdHotspotArea());

	for(int i = 0; i < hotspots.size(); i++) {
		hotspots[i].setup(_x, _y, _w, _h);
		_x += _w;
		if(_x + _w > getX() + getWidth()) {
			_y += _h; _x = getX();
		}
	}

	hotSchemas.clear();
	strSchemas.clear();

	_x = getX() + getWidth();
	_y = getY() + hotTitleBar.h;
	_w = 120;
	_h = 15;

	for(int i = 0; i < mapping.linkSchemas.size(); i++) {
		hotSchemas.push_back(phdHotspotArea(_x,_y,_w,_h));
		strSchemas.push_back(mapping.linkSchemas[i]->name);
		_y += _h;
		if(_y > getY() + getHeight()) { _y = getY(); _x += _w; }
	}
}

bool phdMappingControl::updateHitTestInfo(float _x, float _y) {

	hitTestInfo.infoType = mhtOutside;
	hitTestInfo.mouseX = _x;
	hitTestInfo.mouseY = _y;
	hitTestInfo.popupIndex = -1;
	hitTestInfo.hotspotIndex = -1;
	hitTestInfo.schemaIndex = -1;

	if(getPopupMenu() != NULL && getPopupMenu()->testInsideMeAndMySubMenus(_x,_y)) {
		hitTestInfo.infoType = mhtPopup;
	}

	if(hitTestInfo.infoType == mhtOutside && hotTitleBar.pointInside(_x,_y)) {
		hitTestInfo.infoType = mhtTitleBar;
	}

	if(hitTestInfo.infoType == mhtOutside) {
		for(int i = 0; i < hotspots.size(); i++) {
			if(hotspots[i].pointInside(_x, _y)) {
				hitTestInfo.infoType = mhtHotspot;
				hitTestInfo.hotspotIndex = i;
				break;
			}
		}
	}

	if(hitTestInfo.infoType == mhtOutside) {
		for(int i = 0; i < hotSchemas.size(); i++) {
			if(hotSchemas[i].pointInside(_x, _y)) {
				hitTestInfo.infoType = mhtSchema;
				hitTestInfo.schemaIndex = i;
				break;
			}
		}
	}

	if(hitTestInfo.infoType == mhtOutside && _x >= getX() && _x <= getX() + getWidth() && _y >= getY() && _y <= getY() + getHeight()) {
		hitTestInfo.infoType = mhtInside;
	}

	return (hitTestInfo.infoType != mhtOutside);
}

void phdMappingControl::onMouseMoved(ofMouseEventArgs& args) {
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMouseMoved(args);
		return;
	}

	updateHitTestInfo(args.x, args.y);

	if(hitTestInfo.infoType == mhtInside) {
	} else if(hitTestInfo.infoType == mhtPopup) {
	} else if(hitTestInfo.infoType == mhtTitleBar) {
	} else if(hitTestInfo.infoType == mhtHotspot) {
	}
}

void phdMappingControl::onMousePressed(ofMouseEventArgs& args) {
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMousePressed(args);
		return;
	}

	if(hitTestInfo.infoType == mhtInside) {
	} else if(hitTestInfo.infoType == mhtPopup) {
	} else if(hitTestInfo.infoType == mhtTitleBar) {
	} else if(hitTestInfo.infoType == mhtHotspot) {
	}
}

void phdMappingControl::onMouseDragged(ofMouseEventArgs& args) {
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMouseDragged(args);
		return;
	}

	if(hitTestInfo.infoType == mhtInside) {
	} else if(hitTestInfo.infoType == mhtPopup) {
	} else if(hitTestInfo.infoType == mhtTitleBar) {

		setX(getX() + args.x - hitTestInfo.mouseX);
		setY(getY() + args.y - hitTestInfo.mouseY);

		hitTestInfo.mouseX = args.x;
		hitTestInfo.mouseY = args.y;

		updateHotspotsArea();

	} else if(hitTestInfo.infoType == mhtHotspot) {

	}
}

void phdMappingControl::onMouseReleased(ofMouseEventArgs& args) {
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMouseReleased(args);
		return;
	}

	if(hitTestInfo.infoType == mhtInside) {
		if(args.button == 2 && getPopupMenu() != NULL) {
			getPopupMenu()->setOwner(this);
			getPopupMenu()->popup(args.x, args.y, (void*)this, "MAPPING");
			ofAddListener(getPopupMenu()->popEvent, this, &phdMappingControl::onPopupClick);
		}
	} else if(hitTestInfo.infoType == mhtPopup) {

	} else if(hitTestInfo.infoType == mhtTitleBar) {

	} else if(hitTestInfo.infoType == mhtHotspot) {
		if(hitTestInfo.hotspotIndex == 0) {			// load mapping

			ofFileDialogResult fileDialogResult = ofSystemLoadDialog("Escolha um arquivo", false, "");
			if(fileDialogResult.bSuccess && getManager() != NULL) {
				loadMappingFile(fileDialogResult.fileName);
			}

		} else if(hitTestInfo.hotspotIndex == 1) {  // save mapping

			ofFileDialogResult fileDialogResult = ofSystemSaveDialog(mapping.fileName, "Escolha um arquivo");
			if(fileDialogResult.bSuccess && getManager() != NULL){
				mapping.saveToFile(fileDialogResult.getPath());
			}

		} else if(hitTestInfo.hotspotIndex == 2) {	// new designer

			phdDesigner * _designer = (phdDesigner*) mapping.addDesigner(new phdDesigner(100,100,256,192,NULL,1024,768));
			_designer->designerType = phdDesignerInput;
			_designer->setControlName("IN_" + ofToString(mapping.designers.size()));

			_designer->setInputClip(NULL);

			_designer->setPopupMenu(popups.getPopupByName("PM_DESIGNER"));
			_designer->schema.shapes.setPopupMenu(popups.getPopupByName("PM_SHAPEIN"));

			getManager()->addControl(_designer);

			/*
			bool _hasOutputConnected = false;
			for(int i = 0; i < getManager()->size(); i++) {
				phdControlBase * _control = (*getManager())[i];
				if(_control->getClassName() == "PHDDESIGNER") {
					phdDesigner * _designer = (phdDesigner*) _control;
					if(_designer->designerType == phdDesignerOutput) {
					}
				}
			}

			if(!_hasOutputConnected) { // connect output
				mapping.masterOutput = mapping.addDesigner(new phdDesigner(500, 100, 512, 384, NULL, 1024, 768));
				mapping.masterOutput->setInputClip(NULL);
				mapping.masterOutput->setPopupMenu(popups.getPopupByName("PM_OUTPUT"));
				mapping.masterOutput->schema.shapes.setPopupMenu(popups.getPopupByName("PM_SHAPEOUT"));
				getManager()->addControl(mapping.masterOutput);
			}
			*/
		} else if(hitTestInfo.hotspotIndex == 3) {	// new track

			phdTrackControl * _track = new phdTrackControl();

			_track->getClipTrack()->filters.setAutoFreeItems(false);

			_track->setFilterManager(&mapping.filterManager); // links with filterManager

			mapping.clips.push_back(_track);

			_track->setName("trk" + ofToString(mapping.clips.size()));
			_track->setPosition(100,100);
			_track->setDimension(160,120);
				
			_track->getClipTrack()->blend = getManager()->getBlend(); // links with blend algo

			_track->setPopupMenu(popups.getPopupByName("PM_TRACK"));
			_track->setClipsPopupMenu(popups.getPopupByName("PM_CLIP"));
			_track->updateHotspotsArea();

			getManager()->addControl(_track);
		}

	} else if(hitTestInfo.infoType == mhtSchema) {

		if(hitTestInfo.schemaIndex > -1 && hitTestInfo.schemaIndex < mapping.linkSchemas.size()) {

			mapping.setCurLinkSchema(mapping.linkSchemas[hitTestInfo.schemaIndex]);

			for(int i = getManager()->size()-1; i > -1; i--) {
				if((*getManager())[i]->getClassName() == "PHDDESIGNER") {
					phdDesigner * _designer = (phdDesigner*) (*getManager())[i];
					_designer->curLinkSchema = mapping.linkSchemas[hitTestInfo.schemaIndex];
				}
			}

			hitTestInfo.selectedSchemaIndex = hitTestInfo.schemaIndex;
		}
	}
}

void phdMappingControl::onPopupClick(phdPopupMenuEventData &_eventData) {
	if(_eventData.menu != NULL) {
		ofRemoveListener(_eventData.menu->popEvent, this, &phdMappingControl::onPopupClick);
	}
}

void phdMappingControl::loadMappingFile(string _fileName) {

		if(getManager() != NULL){

			//msg += " " + fileDialogResult.getPath();
			//msg += " " + fileDialogResult.getName();

			mapping.masterOutput = NULL;

			mapping.filterManager.freeItems();

				getManager()->setFocusedControl(NULL, false);
				getManager()->setSelectedControl(NULL, false);

				// clear all designers controls
				for(int i = getManager()->size()-1; i > -1; i--) {
					if((*getManager())[i]->getClassName() == "PHDDESIGNER") {
						phdDesigner * _designer = (phdDesigner*) (*getManager())[i];
						_designer->schema.shapes.setFocused(NULL);
						_designer->schema.shapes.setSelected(NULL);
						getManager()->delControl(i);
					}
				}

				// clear all controls, except the MappingControl
				for(int i = getManager()->size()-1; i > -1; i--) {
					if((*getManager())[i]->getClassName() != "PHDMAPPINGCONTROL") {
						getManager()->delControl(i);
					}
				}

				// load mapping from file
				//----------------------------------------------------------------------------------------
				mapping.loadFromFile(ofToDataPath("maps/" + _fileName, true));

				getManager()->setFocusedControl(NULL);
				getManager()->setSelectedControl(NULL);

				// add designers to control manager
				for(int i = 0; i < mapping.designers.size(); i++) {
					mapping.designers[i]->setInputClip(NULL);

					mapping.designers[i]->setPopupMenu(popups.getPopupByName("PM_DESIGNER"));
					mapping.designers[i]->schema.shapes.setPopupMenu(popups.getPopupByName("PM_SHAPEIN"));

					getManager()->addControl(mapping.designers[i]);
				}

				// add master output to control manager
				//----------------------------------------------------------------------------------------
				mapping.masterOutput->setInputClip(NULL);

				mapping.masterOutput->setPopupMenu(popups.getPopupByName("PM_OUTPUT"));
				mapping.masterOutput->schema.shapes.setPopupMenu(popups.getPopupByName("PM_SHAPEOUT"));

				getManager()->addControl(mapping.masterOutput);

				// add cliptrack to control manager
				//----------------------------------------------------------------------------------------
				for(int i = 0; i < mapping.clips.size(); i++) {

					getManager()->addControl(mapping.clips[i]);

					if(mapping.clips[i]->getClassName() == "PHDTRACKCONTROL") {
						phdTrackControl * _track = (phdTrackControl*)mapping.clips[i];
						_track->getClipTrack()->blend = getManager()->getBlend();
						_track->setPopupMenu(popups.getPopupByName("PM_TRACK"));
						_track->setClipsPopupMenu(popups.getPopupByName("PM_CLIP"));
						_track->updateHotspotsArea();
					}
				}

				// add projectors to control manager
				//----------------------------------------------------------------------------------------
				for(int i = 0; i < mapping.projectors.size(); i++) {
					if(mapping.projectors[i]->getClassName() == "PHDPROJECTOR") {
						phdProjectorControl * _projector = (phdProjectorControl*) mapping.projectors[i];
						getManager()->addControl(mapping.projectors[i]);
					}
				}
			}
}