#pragma once

#include "ofMain.h"

#include "phdShapeBase.h"

//--------------------------------------------------------------------------------------------------------------
class phdShapeGroup {
protected:
	vector<phdShapeBase*> shapes;
	phdShapeBase * selected;
	phdShapeBase * focused;
	bool usingEvents, visible;
public:
	phdShapeGroup();
	~phdShapeGroup();
	phdShapeBase * phdShapeGroup::operator[](unsigned i);
	int size() { return shapes.size(); }
	int shapesCount();
	void freeShapes();
	int indexOf(phdShapeBase * _shape);
	int addShape(phdShapeBase * _shape);
	void delShape(int _index);
	void draw(ofMatrix4x4 & _mat);
	void setVisible(bool _value);
	bool getVisible() { return visible; }
	void setSelected(phdShapeBase * _shape, bool _force = false);
	phdShapeBase * getSelected() { return selected; }
	void setFocused(phdShapeBase * _shape);
	phdShapeBase * getFocused() { return focused; }
	void adjustNearVertices(double _tolerance);
	/*void mouseMoved(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	void enableEvents();
	void disableEvents();*/
};

