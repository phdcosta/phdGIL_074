#include "phdWarpShapeSchema.h"

//-----------------------------------------------------------------------------------------------------------
// schema of warp shapes - for models or textures
//-----------------------------------------------------------------------------------------------------------
phdWarpShapeSchema::phdWarpShapeSchema() {
	width = 512;
	height = 384;
}
	
phdWarpShapeSchema::~phdWarpShapeSchema() { }

void phdWarpShapeSchema::setup(string _name, float _width, float _height) {
	name = _name;
	width = _width;
	height = _height;
}

phdShapeQuadWarp * phdWarpShapeSchema::addShape(string _name) {
	phdShapeQuadWarp * _result = new phdShapeQuadWarp();
	_result->name = _name;
	shapes.addShape(_result);
	return _result;
};

phdShapeQuadWarp * phdWarpShapeSchema::addShape(string _name, ofPolyline & _vertices) {
	phdShapeQuadWarp * _result = new phdShapeQuadWarp();
	_result->setVertices(_vertices);
	_result->name = _name;
	shapes.addShape(_result);
	return _result;
}

int phdWarpShapeSchema::indexOf(phdShapeQuadWarp * _value) {
	for(int i = 0; i < shapes.size(); i++) {
		if(shapes[i] == _value) return i;
	}
	return -1;
}

phdShapeQuadWarp * phdWarpShapeSchema::findByShapeName(string _name) {
	for(int i = 0; i < shapes.size(); i++) {
		if(shapes[i]->name == _name) return (phdShapeQuadWarp*) shapes[i];
	}
	return NULL;
}

void phdWarpShapeSchema::draw(ofMatrix4x4 & _mat) {
	shapes.draw(_mat);
}

