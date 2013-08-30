#include "phdVisualFile.h"

//--------------------------------------------------------------------------------------------------------------

phdVisualFile::phdVisualFile() { }

phdVisualFile::~phdVisualFile() {
}

void phdVisualFile::freeItems() {

	curLinkSchema = NULL;

	// free link schemas
	for (vector<phdWarpLinkSchema*>::iterator i = linkSchemas.begin(); i != linkSchemas.end(); i++) {
		delete(*i); (*i) = NULL;
	}
	linkSchemas.clear();

	// free shape schemas
	for (vector<phdWarpLinkSchema*>::iterator i = linkSchemas.begin(); i != linkSchemas.end(); i++) {
		delete(*i); (*i) = NULL;
	}
	linkSchemas.clear();

	// free designers
	designers.clear();
	masterOutput.schema.shapes.freeShapes();
}

phdDesigner * phdVisualFile::findDesignerByQuadWarp(phdShapeQuadWarp * _warp) {
	for(int i = 0; i < designers.size(); i++) {
		for(int j = 0; j < designers[i]->schema.size(); j++) {
			if(designers[i]->schema[j] == _warp) return designers[i];
		}
	}
	return NULL;
}

int phdVisualFile::indexOfLinkSchema(phdWarpLinkSchema * _schema) {
	for(int i = 0; i < linkSchemas.size(); i++) {
		if(linkSchemas[i] == _schema) return i;
	}
	return - 1;
}

phdWarpLinkSchema * phdVisualFile::findLinkSchemaByName(string _name) {
	for(int i = 0; i < linkSchemas.size(); i++) {
		if(linkSchemas[i]->name == _name) return linkSchemas[i];
	}
	return NULL;
}

phdWarpLinkSchema * phdVisualFile::addLinkSchema(phdWarpLinkSchema * _schema) {
	if(_schema != NULL && indexOfLinkSchema(_schema) == -1) {
		linkSchemas.push_back(_schema);
	}
	return _schema;
}

void phdVisualFile::updateScreenLinkSchemaPos() {
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

				phdDesigner * _dgnOUT = &masterOutput; // output is masterOutput? if not find from quadOut
				if(_dgnOUT->schema.indexOf(_out) == -1)	_dgnOUT = findDesignerByQuadWarp(_out);

				if(_dgnOUT != NULL) {
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

void phdVisualFile::setCurLinkSchema(phdWarpLinkSchema * _value) {
	if(curLinkSchema != _value) {
		curLinkSchema = _value;
		updateScreenLinkSchemaPos();
	}
}

void phdVisualFile::setCurSelDesigner(phdDesigner * _dsgn) {

	int _index = indexOfDesigner(_dsgn);

	if(_index != -1) {
		for(int i = 0; i < designers.size(); i++) {
			if(designers[i] != NULL) designers[i]->selected = false;
		}
		designers[_index]->selected = true;
	}
}

int phdVisualFile::indexOfDesigner(phdDesigner * _designer) {
	for(int i = 0; i < designers.size(); i++) {
		if(designers[i] != NULL && designers[i] == _designer) return i;
	}
	return - 1;
}

phdDesigner * phdVisualFile::findDesignerByName(string _name) {
	for(int i = 0; i < designers.size(); i++) {
		if(designers[i] != NULL && designers[i]->name == _name) return designers[i];
	}
	return NULL;
}

phdDesigner * phdVisualFile::findDesignerSelected() {
	for(int i = 0; i < designers.size(); i++) {
		if(designers[i] != NULL && designers[i]->selected) return designers[i];
	}
	return NULL;
}

phdDesigner * phdVisualFile::addDesigner(phdDesigner * _designer) {
	if(_designer != NULL && indexOfDesigner(_designer) == -1) {
		designers.push_back(_designer);
	}
	return _designer;
}

void phdVisualFile::saveToFile(string _fileName) {

	ofstream fout; // declare a file stream
	fout.open( ofToDataPath(_fileName).c_str() ); // open your text file

	string _line = "";
	vector<string> _strings;

	_strings.clear(); masterOutput.saveToStrings(_strings, "MASTEROUTPUT");
	for(int s = 0; s < _strings.size(); s++) fout.write(ofToUpper(_strings[s]).c_str(), _strings[s].size());

	_line = "DESIGNERCOUNT:" + ofToString(designers.size()) + ":\n";
	fout.write(ofToUpper(_line).c_str(), _line.size());

	for(int i = 0; i < designers.size(); i++) {
		_strings.clear(); designers[i]->saveToStrings(_strings, "DESIGNER");
		for(int s = 0; s < _strings.size(); s++) fout.write(ofToUpper(_strings[s]).c_str(), _strings[s].size());
	}

	_line = "LINKSCHEMACOUNT:" + ofToString(linkSchemas.size()) + ":\n";
	fout.write(ofToUpper(_line).c_str(), _line.size());

	for(int i = 0; i < linkSchemas.size(); i++) {

		_line = "LINKSCHEMA:" + linkSchemas[i]->name + ":" + ofToString(linkSchemas[i]->links.size()) + ":\n";
		fout.write(ofToUpper(_line).c_str(), _line.size());
			
		for(int j = 0; j < linkSchemas[i]->links.size(); j++) {

			phdDesigner * _dgnIN = NULL;
			if(linkSchemas[i]->links[j]->quadIn != NULL) _dgnIN = findDesignerByQuadWarp(linkSchemas[i]->links[j]->quadIn);

			if(_dgnIN != NULL) {

				phdDesigner * _dgnOUT = &masterOutput;
				if(linkSchemas[i]->links[j]->quadOut != NULL && _dgnOUT->schema.indexOf(linkSchemas[i]->links[j]->quadOut) == -1) {
					_dgnOUT = findDesignerByQuadWarp(linkSchemas[i]->links[j]->quadOut);
				}

				if(_dgnOUT != NULL) {
					_line = "LINK:";
					if(linkSchemas[i]->links[j]->quadIn != NULL) {
						_line += _dgnIN->name + ":";
						_line += linkSchemas[i]->links[j]->quadIn->name + ":";
					} else {
						_line += "::";
					}
					if(linkSchemas[i]->links[j]->quadOut != NULL) {
						_line += _dgnOUT->name + ":";
						_line += linkSchemas[i]->links[j]->quadOut->name + ":";
					} else {
						_line += "::";
					}
					_line += linkSchemas[i]->links[j]->horMirror ? "SIM:" : "NAO:";
					_line += linkSchemas[i]->links[j]->verMirror ? "SIM:" : "NAO:";

					_line += "\n";
					fout.write(ofToUpper(_line).c_str(), _line.size());
				}
			}
		}
	}

	fout.close(); // close file stream
}

//--------------------------------------------------------------------------------------------------------------
void phdVisualFile::loadFromFile(string _fileName) {

	masterOutput.schema.shapes.setFocused(NULL);
	masterOutput.schema.shapes.setSelected(NULL, true);

	freeItems();

	ifstream _fin; //declare a file stream  
	_fin.open( ofToDataPath(_fileName).c_str() ); //open your text file  
     
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

	for(int i = 0; i < _data.size(); i++) {

		vector<string> _items = ofSplitString(_data[i], ":", true, true);

		if(_items.size() > 0) {
			if(_items[0] == "DESIGNERCOUNT") {
					
				// resize designer array

			} else if(_items[0] == "MASTEROUTPUT" && _items.size() == 8) {
				
				_designer = &masterOutput;
				_designer->name = _items[1];

				float _x = ofToFloat(_items[2]);
				float _y = ofToFloat(_items[3]);
				float _w = ofToFloat(_items[4]);
				float _h = ofToFloat(_items[5]);

				_designer->setup(_x,_y,_w,_h, NULL, ofToFloat(_items[6]), ofToFloat(_items[7]));

			} else if(_items[0] == "DESIGNER" && _items.size() == 8) {
				
				_designer = NULL;
				_designer = (phdDesigner*) addDesigner(new phdDesignerFbo());
				_designer->name = ofToUpper(_items[1]);

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
				if(_dgnOUT == NULL) _dgnOUT = &masterOutput;

				if(_dgnIN != NULL && _dgnOUT != NULL) {

					phdShapeQuadWarp * _quadIN  = _dgnIN->schema.findByShapeName(_items[2]);
					phdShapeQuadWarp * _quadOUT = _dgnOUT->schema.findByShapeName(_items[4]);

					phdWarpLink * _link = _linkSchema->addLink(_quadIN, _dgnIN, _quadOUT, _dgnOUT);
					_link->horMirror = (_items[5] == "SIM");
					_link->verMirror = (_items[6] == "SIM");
					_link->updated = true;
				}
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
