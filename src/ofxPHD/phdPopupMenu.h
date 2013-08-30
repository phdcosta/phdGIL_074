#pragma once

#include "ofMain.h"

#include "phdGeom.h"
//#include "phdControlBase.h"
//#include "phdControlManager.h"

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
	phdPopupMenu * focusedMenu;
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

static const int menuItemHeight = 18;
static const int menuItemCharWidth = 8;
static const int menuMaxItemsPerCol = 25;

//--------------------------------------------------------------------------------------------------------------
class phdPopupMenu {
protected:
	int nRows, nCols;
	bool visible;
	float x, y;
	float colWidth, menuHeight;

	vector<phdMenuItem*> items;

	void updateMenuDimensions();

public:
	ofEvent<phdPopupMenuEventData> popEvent;

	void * owner;
	phdPopupMenu * parent;
	phdPopupMenuHitTestInfo hitTestInfo;

	string name;

	void * caller;
	string callerTag;

	phdPopupMenu(phdPopupMenu * _parent = NULL);
	~phdPopupMenu();

	phdMenuItem * phdPopupMenu::operator[](unsigned i) { return items[i]; }
	inline int size() { return items.size(); }

	void freeItems();

	void setOwner(void * _owner) { owner = _owner; }

	phdPopupMenu * getRootMenu() {
		phdPopupMenu * _result = this;
		while(_result->parent != NULL) { // while has parent
			_result = _result->parent;
		}
		return _result;
	}

	void setVisible(bool _value);
	bool getVisible();
	float getWidth();
	float getHeight();
	void setPosition(float _x, float _y);

	bool updateHitTestInfo(float _x, float _y);

	void update();
	void draw();

	void onMouseMoved(ofMouseEventArgs& args);
	void onMousePressed(ofMouseEventArgs& args);
	void onMouseDragged(ofMouseEventArgs& args);
	void onMouseReleased(ofMouseEventArgs& args);

	phdMenuItem * addItem(phdMenuItem * _item);
	phdMenuItem * addItem(string _name, int _id, phdPopupMenu * _subMenu, bool _grouped);
	void setFocusedItem(phdMenuItem * _item);
	void setClickedItem(phdMenuItem * _item);
	phdPopupMenu * testInsideMeAndMySubMenus(float _x, float _y);

	void popup(float _x, float _y, void * _caller, string _callerTag);
};

//--------------------------------------------------------------------------------------------------------------
class phdPopupMenuListItem {
public:
	string itemName;
	phdPopupMenu * itemMenu;

	phdPopupMenuListItem() {
		itemName = "";
		itemMenu = NULL;
	}

	phdPopupMenuListItem(string _name, phdPopupMenu * _menu) {
		itemName = _name;
		itemMenu = _menu;
	};

	~phdPopupMenuListItem() {
		itemName = "";
		itemMenu = NULL;
	}
};


//--------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------
class phdPopupMenuList {
protected:
	vector<phdPopupMenuListItem*> items;

public:
	phdPopupMenuList();
	~phdPopupMenuList();

	phdPopupMenuListItem * phdPopupMenuList::operator[](unsigned i) { return items[i]; }
	int phdPopupMenuList::size() { return items.size(); }

	void freeItems();
	void clear();

	int indexByPopup(phdPopupMenu * _item);
	int indexByName(string _name);
	phdPopupMenu * getPopupByName(string _name);

	phdPopupMenuListItem * addItem(string _name, phdPopupMenu * _menu);
	void delItem(string _name);
};







