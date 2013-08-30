#pragma once

#include "ofMain.h"

#include "phdShapeBase.h"
#include "phdPopupMenu.h"

//--------------------------------------------------------------------------------------------------------------
class phdShapeGroup {
protected:
	vector<phdShapeBase*> shapes;
	phdShapeBase * selected;
	phdShapeBase * focused;
	bool visible;
	phdPopupMenu * popupMenu;

public:
	phdShapeGroup();
	~phdShapeGroup();

	phdShapeBase * phdShapeGroup::operator[](unsigned i) { return shapes[i]; }
	int size() { return shapes.size(); }

	void freeShapes();
	int indexOf(phdShapeBase * _shape);

	void setVisible(bool _value);
	bool getVisible() { return visible; }
	void setSelected(phdShapeBase * _shape);
	phdShapeBase * getSelected() { return selected; }
	void setFocused(phdShapeBase * _shape);
	phdShapeBase * getFocused() { return focused; }

	int addShape(phdShapeBase * _shape);
	void delShape(int _index);
	void delShape(phdShapeBase * _shape);
	void adjustNearVertices(double _tolerance);

	void update();
	void draw(ofMatrix4x4 & _mat);
	
	virtual void setPopupMenu(phdPopupMenu * _menu) { popupMenu = _menu; }
	virtual phdPopupMenu * getPopupMenu() { return popupMenu; }

	void onMouseMoved(ofMouseEventArgs& args);
	void onMousePressed(ofMouseEventArgs& args);
	void onMouseDragged(ofMouseEventArgs& args);
	void onMouseReleased(ofMouseEventArgs& args);

	void onPopupClick(phdPopupMenuEventData &_eventData);
};

