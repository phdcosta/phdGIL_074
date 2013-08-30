#pragma once

#include "ofMain.h"

#include "phdAttributes.h"
#include "phdPopupMenu.h"
#include "phdBlend.h"

//--------------------------------------------------------------------------------------------------------------
class phdControlBase;
class phdControlManager;

//--------------------------------------------------------------------------------------------------------------
static const int linkSize = 10;

//--------------------------------------------------------------------------------------------------------------
class phdControlLink {
protected:
	string name;
public:
	float x, y;
	phdControlBase * parentControl;
	phdControlLink * remoteLink;

	phdControlLink(phdControlBase * _parent = NULL, phdControlLink * _remote = NULL);

	void setRemoteLink(phdControlLink * _remoteLink);
	void setPosition(float _x, float _y) { x = _x; y = _y; }

	bool pointInside(float _x, float _y);
	void draw();
};

//--------------------------------------------------------------------------------------------------------------
enum phdControlLinksDrawMode { ldmHorLeft, ldmHorRight, ldmVerBottom, ldmVerTop };

//--------------------------------------------------------------------------------------------------------------
class phdControlLinks {
protected:
	float x, y;
	phdControlLinksDrawMode drawMode;
	phdControlBase * parent;
	vector<phdControlLink> links;

public:
	int focusedIndex;

	phdControlLinks(phdControlBase * _parent = NULL, phdControlLinksDrawMode _drawMode = ldmVerBottom);

	phdControlLink * phdControlLinks::operator[](unsigned i) { return &links[i]; }
	inline int size() { return links.size(); }

	void setParentControl(phdControlBase * _parent = NULL);
	phdControlLink * addLink(phdControlLink * _remoteLink = NULL);
	void linkIndexedToRemoteLink(int _index, phdControlLink * _remoteLink);

	void setDrawMode(phdControlLinksDrawMode _drawMode);
	void setPosition(float _x, float _y);
	bool updateHitTestInfo(float _x, float _y);
	void draw(float _x, float _y);
};

//--------------------------------------------------------------------------------------------------------------
enum phdHitTestInfoType {
	ghtOutside, ghtInside,
	ghtMenu, ghtDragControl,
	ghtVerticalScroll, ghtHorizontalScroll, ghtResize, ghtCommand,
	ghtInputLink, ghtOutputLink,
	ghtPopupMenu
};

//--------------------------------------------------------------------------------------------------------------
class phdControlBase {
protected:
	string name;
	float x, y, width, height;
	bool selected, focused, dragging, visible;
	phdPopupMenu * popupMenu;
	phdControlManager * manager;

public:
	phdControlLinks inputLinks;
	phdControlLinks outputLinks;

	phdControlBase(phdControlManager * _manager = NULL);
	~phdControlBase() { }

	virtual string getClassName() { return "PHDCONTROLBASE"; }

	virtual void setControlName(string _name) { name = _name; }
	virtual string getControlName() { return name; }

	virtual void setX(float _x) { x = _x; }
	virtual float getX() { return x; }

	virtual void setY(float _y) { y = _y; }
	virtual float getY() { return y; }

	virtual void setWidth(float _width) { width = _width; }
	virtual float getWidth() { return width; }

	virtual void setHeight(float _height) { height = _height; }
	virtual float getHeight() { return height; }

	virtual void setSelected(bool _selected) { selected = _selected; }
	virtual bool getSelected() { return selected; }

	virtual void setFocused(bool _focused) { focused = _focused; }
	virtual bool getFocused() { return focused; }

	virtual void setVisible(bool _visible) { visible = _visible; }
	virtual bool getVisible() { return visible; }

	virtual void setPosition(float _x, float _y) { setX(_x); setY(_y); }
	virtual void setDimension(float _width, float _height) { setWidth(_width); setHeight(_height); }

	virtual void setName(string _name) { name = _name; }
	virtual string getName() { return name; }

	virtual void setPopupMenu(phdPopupMenu * _menu) { popupMenu = _menu; }
	virtual phdPopupMenu * getPopupMenu() { return popupMenu; }

	virtual void setManager(phdControlManager * _manager) { manager = _manager; }
	virtual phdControlManager * getManager() { return manager; }

	virtual bool updateHitTestInfo(float _x, float _y);
	virtual void populateGui(phdControlBase * _gui);

	virtual void onMouseMoved(ofMouseEventArgs& args);
	virtual void onMousePressed(ofMouseEventArgs& args);
	virtual void onMouseDragged(ofMouseEventArgs& args);
	virtual void onMouseReleased(ofMouseEventArgs& args);

	virtual void update();
	virtual void draw();

	virtual void onPopupClick(phdPopupMenuEventData &_eventData);

	virtual string getSetupString();
	virtual void setupFromString(string _setupString) { };
	virtual void saveToStrings(vector<string> & _strings, string _title);

	virtual void saveToAttributes(phdAttributeList & _attributes);
	virtual void loadFromAttributes(phdAttributeList & _attributes);
};

//----------------------------------------------------------------------------------------------------------------------
enum phdControlManagerDrawMode { cmdmControls, cmdmWarpers, cmdmBoth };

//----------------------------------------------------------------------------------------------------------------------
class phdControlManager {
protected:
	vector<phdControlBase*> controls;
	phdControlBase * selectedControl;
	phdControlBase * focusedControl;
	bool usingEvents;
	phdPopupMenu * popupMenu;
	phdBlend blend;

	phdControlManagerDrawMode drawMode;

public:
	phdControlManager();
	~phdControlManager();

	phdControlBase * phdControlManager::operator[](unsigned i) { return controls[i]; }
	inline int size() { return controls.size(); }

	void freeControls();
	void deleteItem(int _index, bool _freeMem);

	void setDrawMode(phdControlManagerDrawMode _drawMode);
	phdControlManagerDrawMode getDrawMode() { return drawMode; }
	void setWarpersDrawMode(string _drawMode);

	void resetFocus() { focusedControl = NULL; selectedControl = NULL; }

	void setSelectedControl(phdControlBase * _control, bool _autoFocus = true);
	phdControlBase * getSelectedControl();

	void setFocusedControl(phdControlBase * _control, bool _autoFocus = true);
	phdControlBase * getFocusedControl();

	void setPopupMenu(phdPopupMenu * _menu) { popupMenu = _menu; }
	phdPopupMenu * getPopupMenu() { return popupMenu; }

	phdBlend * getBlend() { return &blend; }

	int indexOf(phdControlBase * _control);
	int addControl(phdControlBase * _control);
	void delControl(phdControlBase * _control);
	void delControl(int _index);

	void update();
	void draw();

	void drawWarpers();

	void mouseMoved(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);

	void enableEvents();
	void disableEvents();

	void onPopupClick(phdPopupMenuEventData &_eventData);
};

