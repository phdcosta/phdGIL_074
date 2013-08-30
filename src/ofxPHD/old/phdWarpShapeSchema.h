#pragma once

#include "ofMain.h"

#include "phdShapeQuadWarp.h"
#include "phdShapeGroup.h"

//-----------------------------------------------------------------------------------------------------------
// schema of warp shapes - for models or textures
//-----------------------------------------------------------------------------------------------------------
class phdWarpShapeSchema {
public:
	float width, height;  // width and height for schema - scale warp from visual -> visual
	phdShapeGroup shapes;
	string name;

	phdWarpShapeSchema();	
	~phdWarpShapeSchema();
	phdShapeQuadWarp * phdWarpShapeSchema::operator[] (int _index) { return (phdShapeQuadWarp*) shapes[_index]; }
	int size() { return shapes.size(); }
	void setup(string _name, float _width, float _height);
	phdShapeQuadWarp * addShape(string _name);
	phdShapeQuadWarp * addShape(string _name, ofPolyline & _vertices);
	int indexOf(phdShapeQuadWarp * _value);
	phdShapeQuadWarp * findByShapeName(string _name);
	void draw(ofMatrix4x4 & _mat);	// draw border of shape schema
};
