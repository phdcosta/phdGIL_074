#pragma once

#include "ofMain.h"

#include "phdDesigner.h"
#include "phdDesignerFbo.h"

//--------------------------------------------------------------------------------------------------------------
class phdPopupMenu;

//--------------------------------------------------------------------------------------------------------------
class phdMenuItem {
public:
	string name;
	int id;
	bool selected;
	bool grouped;
	phdPopupMenu * subMenu;
	phdPopupMenu * parent;
	phdMenuItem();
	phdMenuItem(string _name, int _id, phdPopupMenu * _parent, phdPopupMenu * _subMenu, bool _grouped);
	~phdMenuItem();
	string getFullPath();
	int getLevel();
	string getCaller();
};

enum phdPopupHitTestInfoType { pphOutside, pphInside };

//--------------------------------------------------------------------------------------------------------------
class phdPopupMenuHitTestInfo {
public:
	phdPopupHitTestInfoType infoType;
	phdMenuItem * focused;
	phdMenuItem * clicked;
	int focusedIndex;
	int clickedIndex;

	phdPopupMenuHitTestInfo() {
		infoType = pphOutside;
		focused = NULL;
		clicked = NULL;
		focusedIndex = -1;
		clickedIndex = -1;
	}
};

//--------------------------------------------------------------------------------------------------------------
enum phdPopupEventType { petClickOnItem, petClickOutside, petPopupSubMenu, petSetItemsBeforePopup };

//--------------------------------------------------------------------------------------------------------------
class phdPopupMenuEventData {
public:
	phdMenuItem * item;
	phdPopupMenu * menu;
	phdPopupEventType eventType;
};

//--------------------------------------------------------------------------------------------------------------
class phdPopupMenu {
protected:
	int nRows, nCols;
	void updateDimensions();
public:
	static const int menuItemHeight = 18;
	static const int menuItemCharWidth = 8;
	static const int menuMaxItemsPerCol = 25;

	ofEvent<phdPopupMenuEventData> popEvent;
	ofEvent<phdDesignerEventData> dgnEvent;

	phdPopupMenu * parent;
	vector<phdMenuItem*> items;
	phdPopupMenuHitTestInfo hitTestInfo;
	float x, y;
	float colWidth, menuHeight;
	bool visible;
	bool usingEvents;
	string name;
	string caller;

	phdPopupMenu();
	phdPopupMenu(phdPopupMenu * _parent);
	~phdPopupMenu();
	void freeItems();
	void setVisible(bool _value);
	phdMenuItem * addItem(phdMenuItem * _item);
	phdMenuItem * addItem(string _name, int _id, phdPopupMenu * _subMenu, bool _grouped);
	float getWidth();
	float getHeight();
	void draw();
	void setPosition(float _x, float _y);
	void setFocused(phdMenuItem * _item);
	void setClicked(phdMenuItem * _item);
	bool testInsideMeAndMySubMenus(float _x, float _y);
	void updateHitTestInfo(float _x, float _y);
	void mouseMoved(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	void enableEvents();
	void disableEvents();
};
