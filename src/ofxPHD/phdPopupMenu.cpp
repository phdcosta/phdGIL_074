#include "phdPopupMenu.h"

//--------------------------------------------------------------------------------------------------------------
phdMenuItem::phdMenuItem() {
	name = "menuItem";
	id = -1;
	parent = NULL;
	subMenu = NULL;
	grouped = false;
	selected = false;
}

phdMenuItem::phdMenuItem(string _name, int _id, phdPopupMenu * _parent, phdPopupMenu * _subMenu, bool _grouped) {
	name = _name;
	id = _id;
	parent = _parent;
	subMenu = _subMenu;
	grouped = _grouped;
	selected = false;
	if(subMenu != NULL) subMenu->parent = _parent;
}

phdMenuItem::~phdMenuItem() {
	parent = NULL;
}

string phdMenuItem::getFullPath() {
	phdPopupMenu * _parent = parent;
	string _path = name + ":"; // item name
	while(_parent != NULL) { // while has parent
		if(_parent != NULL) _path = _parent->name + ":" + _path; // insert menuName on path
		_parent = _parent->parent;
	}
	return _path;
}

int phdMenuItem::getLevel() {
	phdPopupMenu * _parent = parent;
	int _result = -1; // item level
	while(_parent != NULL) { // while has parent
		if(_parent != NULL) _result += 1; // sum level
		_parent = _parent->parent;
	}
	return _result;
}

string phdMenuItem::getCaller() {
	phdPopupMenu * _parent = parent;
	string _result = "";
	while(_parent != NULL) { // while has parent
		if(_parent != NULL) _result = _parent->callerTag;
		_parent = _parent->parent;
	}
	return _result;
}

//--------------------------------------------------------------------------------------------------------------
phdPopupMenu::phdPopupMenu(phdPopupMenu * _parent) {
	owner = NULL;
	parent = _parent;
	visible = false;
}

phdPopupMenu::~phdPopupMenu() {
	freeItems();
	items.clear();
}

void phdPopupMenu::freeItems() {
	for(int i = 0; i <items.size(); i++) {
		//if(items[i]->subMenu != NULL) items[i]->subMenu->freeItems(); // free items dont work with menu testApp.h
		delete items[i];
	}
	items.clear();
}

void phdPopupMenu::setVisible(bool _value) {
	if(_value != visible) {
		visible = _value;
		if(!visible) {
			for(int i = 0; i < items.size(); i++) {
				if(items[i]->subMenu != NULL) items[i]->subMenu->setVisible(false); // ocults every subMenu
			}
		} else {
			if(items.size() > 0) {
			}
		}
	}
}

bool phdPopupMenu::getVisible() {
	return visible;
}

float phdPopupMenu::getWidth() { // get max item width
	float _w = 0;
	for(int i = 0; i < items.size(); i++) {
		float _iw = (items[i]->name.size() + 1) * menuItemCharWidth;
		if(items[i]->subMenu != NULL) _iw += menuItemCharWidth * 2;
		if(_iw > _w) _w = _iw;
	}
	return _w;
}

float phdPopupMenu::getHeight() { // get a col height ==== menu height
	//return items.size() * menuItemHeight;
	return nRows * menuItemHeight;
}

void phdPopupMenu::setPosition(float _x, float _y) {
	x = _x; y = _y;
}

void phdPopupMenu::update() {
	if(getVisible()) updateHitTestInfo(ofGetMouseX(), ofGetMouseY());
}

void phdPopupMenu::draw() {
	if(visible && items.size() > 0) {
		for(int i = 0; i < nCols; i++) {
			drawFilledBorderRectangle(x + (i*colWidth), y, colWidth, menuHeight, ofColor(128,64,255,128), ofColor(230,198,255,220));
		}
		for(int i = 0; i < items.size(); i++) {

			int _c = (int)i / nRows;
			int _r = (int)i % nRows;

			float _itemX = x + ((int)(i / nRows) * colWidth);
			float _itemY = y + ((i % nRows) * menuItemHeight);
			if(hitTestInfo.focusedMenu == this && hitTestInfo.focusedIndex == i) {
				if(items[i]->selected) {
					drawFilledBorderRectangle(_itemX, _itemY, colWidth, menuItemHeight, ofColor(128,192,255,128), ofColor(230,198,255,220));
				} else {
					drawFilledBorderRectangle(_itemX, _itemY, colWidth, menuItemHeight, ofColor(128,64,255,128), ofColor(230,198,255,220));
				}
				ofSetColor(255,255,64,255);
			} else {
				if(items[i]->selected) {
					drawFilledBorderRectangle(_itemX, _itemY, colWidth, menuItemHeight, ofColor(128,192,255,128), ofColor(230,198,255,220));
				}
				ofSetColor(255,255,255,255);
			}
			string _str = items[i]->name; if(items[i]->subMenu != NULL) _str += " >";
			ofDrawBitmapString(_str, _itemX+2, _itemY+12);
			if(items[i]->subMenu != NULL) {
				items[i]->subMenu->setPosition(x + colWidth*nCols + 2, _itemY);
				items[i]->subMenu->draw();
			}
		}
	}
}

bool phdPopupMenu::updateHitTestInfo(float _x, float _y) {

	hitTestInfo.infoType = pphOutside;
	hitTestInfo.focusedIndex = -1;
	hitTestInfo.focusedMenu = this;

	if(_x >= x && _x <= x+colWidth*nCols && _y >= y && _y <= y+menuHeight) {
		hitTestInfo.infoType = pphInside;
		if(items.size() == 0) {
			setFocusedItem(NULL);
		} else {
			//hitTestInfo.focusedIndex = ((_y-y) / h) * items.size();

			int _r = (int)((_y-y) / menuItemHeight);
			int _c = (int)((_x-x) / colWidth);

			hitTestInfo.focusedIndex = _c * nRows + _r;

			if(hitTestInfo.focusedIndex > -1 && hitTestInfo.focusedIndex < items.size()) {
				setFocusedItem(items[hitTestInfo.focusedIndex]);
			} else {
				setFocusedItem(NULL);
			}
		}
	} else {

		for(int i = 0; i < items.size(); i++) {
			if(items[i] != NULL && items[i]->subMenu != NULL) { // has a submenu
				if(items[i]->subMenu->getVisible() && items[i]->subMenu->updateHitTestInfo(_x,_y)) { // if inside submenu

					items[i]->subMenu->caller = caller;
					items[i]->subMenu->callerTag = callerTag;
					items[i]->subMenu->owner = owner;

					hitTestInfo = items[i]->subMenu->hitTestInfo;

					if(hitTestInfo.focusedMenu != NULL) {
						hitTestInfo.focusedMenu->setFocusedItem(hitTestInfo.focused);
					}

					//i = items.size(); // cai fora
				}
			}
		}

		setFocusedItem(NULL);
	}

	return (hitTestInfo.infoType != pphOutside);
}

void phdPopupMenu::onMouseMoved(ofMouseEventArgs& args) {
	updateHitTestInfo(args.x, args.y);
	if(hitTestInfo.infoType == pphOutside) {
		phdPopupMenu * _popup = NULL;
		_popup = testInsideMeAndMySubMenus(args.x,args.y);
		bool _found = (_popup != NULL);
		if(_found) { // click inside - a sub menu
			if(_popup != NULL) { _popup->onMouseMoved(args); }
		}
	}
}

void phdPopupMenu::onMousePressed(ofMouseEventArgs& args) {
	if(!updateHitTestInfo(args.x, args.y) && getVisible()) {
		phdPopupMenu * _popup = NULL;
		_popup = testInsideMeAndMySubMenus(args.x,args.y);
		bool _found = (_popup != NULL);
		setVisible(_found);
		if(!_found) { // click outside - pass menu
			phdPopupMenuEventData _pmed;
			_pmed.item = NULL;
			_pmed.menu = (phdPopupMenu*) this;
			_pmed.eventType = petClickOutside;
			ofNotifyEvent(popEvent, _pmed, this);
		}
	}
}

void phdPopupMenu::onMouseDragged(ofMouseEventArgs& args) {
	updateHitTestInfo(args.x, args.y);
	if(hitTestInfo.infoType == pphOutside) {
		phdPopupMenu * _popup = NULL;
		_popup = testInsideMeAndMySubMenus(args.x,args.y);
		bool _found = (_popup != NULL);
		if(_found) { // click inside - a sub menu
			if(_popup != NULL) { _popup->onMouseDragged(args); }
		}
	}
}

void phdPopupMenu::onMouseReleased(ofMouseEventArgs& args) {
	updateHitTestInfo(args.x, args.y);
	if(hitTestInfo.infoType == pphInside && hitTestInfo.focusedIndex != -1) {

		if(hitTestInfo.focusedMenu != NULL) {
			hitTestInfo.focusedMenu->setClickedItem(hitTestInfo.focusedMenu->items[hitTestInfo.focusedIndex]);
		}

		//setClickedItem(items[hitTestInfo.focusedIndex]);

	} else if(hitTestInfo.infoType == pphOutside) {
		phdPopupMenu * _popup = NULL;
		_popup = testInsideMeAndMySubMenus(args.x,args.y);
		bool _found = (_popup != NULL);
		setVisible(_found);
		if(!_found) { // click outside - pass menu
			phdPopupMenuEventData _pmed;
			_pmed.item = NULL;
			_pmed.menu = (phdPopupMenu*) this;
			_pmed.eventType = petClickOutside;
			ofNotifyEvent(popEvent, _pmed, this);
		} else {
			if(_popup != NULL) { _popup->onMouseReleased(args); }
		}
	}
}

phdMenuItem * phdPopupMenu::addItem(phdMenuItem * _item) {
	if(_item != NULL) {
		items.push_back(_item);
		_item->parent = (phdPopupMenu*)this;
		updateMenuDimensions();
	}
	return _item;
}

phdMenuItem * phdPopupMenu::addItem(string _name, int _id, phdPopupMenu * _subMenu, bool _grouped) {
	phdMenuItem * _result = addItem(new phdMenuItem(_name, _id, (phdPopupMenu*)this, _subMenu, _grouped));
	return _result;
}

void phdPopupMenu::setFocusedItem(phdMenuItem * _item) {
	//if(_item != hitTestInfo.focused) {
		hitTestInfo.focused = _item;
	//}
}

void phdPopupMenu::setClickedItem(phdMenuItem * _item) {

	// hides all submenus from items of this menu
	for(int i = 0; i < items.size(); i++) {
		items[i]->selected = false; // unselect all
		if(items[i]->subMenu != NULL) items[i]->subMenu->setVisible(false);
	}

	hitTestInfo.clicked = _item;

	if(hitTestInfo.clicked != NULL) {
		if(hitTestInfo.clicked->subMenu != NULL/* && hitTestInfo.clicked->subMenu->items.size() > 0*/) { // if has a submenu

			phdPopupMenuEventData _pmed;
			_pmed.menu = (phdPopupMenu*) this;
			_pmed.item = hitTestInfo.clicked;
			_pmed.eventType = petPopupSubMenu;
			ofNotifyEvent(popEvent, _pmed, this);

			hitTestInfo.clicked->subMenu->setVisible(!hitTestInfo.clicked->subMenu->visible); // shows or hides it

		} else {
			
			// if belongs to a group -> sinalize it
			if(hitTestInfo.clicked->grouped) hitTestInfo.clicked->selected = true;

			phdPopupMenu * _parent = hitTestInfo.clicked->parent;
			while(_parent != NULL && _parent->parent != NULL) { // get root
				_parent = _parent->parent;
			}
			if(_parent != NULL) {
				_parent->setVisible(false); // hides all
			}

			phdPopupMenuEventData _pmed;
			_pmed.menu = (phdPopupMenu*) this;
			_pmed.item = hitTestInfo.clicked;
			_pmed.eventType = petClickOnItem;
			ofNotifyEvent(popEvent, _pmed, this);
		}
	}
}

phdPopupMenu * phdPopupMenu::testInsideMeAndMySubMenus(float _x, float _y) {

	bool _result = (_x >= x && _x <= x+colWidth*nCols && _y >= y && _y <= y+menuHeight);

	if(_result) {
		return this;
	}

	for(int i = 0; i < items.size(); i++) {
		//if(items[i]->subMenu != NULL) { _result = (_result || items[i]->subMenu->testInsideMeAndMySubMenus(_x,_y)); }
		if(items[i]->subMenu != NULL && items[i]->subMenu->getVisible()) {
			return items[i]->subMenu->testInsideMeAndMySubMenus(_x,_y);
		}
	}

	return NULL;
}

void phdPopupMenu::updateMenuDimensions() {

	nCols = 1 + (int)(items.size() / menuMaxItemsPerCol); // 15 max in a col

	nRows = (int)(items.size() / nCols);
	if(items.size() % nCols != 0) nRows += 1;

	colWidth = getWidth();
	menuHeight = getHeight();  // actualize width / height
}

void phdPopupMenu::popup(float _x, float _y, void * _caller, string _callerTag) {
	if(!getVisible()) {
		caller = (void*) _caller;
		callerTag = _callerTag;
		setPosition(_x, _y);
		setVisible(true);
	}
}

//--------------------------------------------------------------------------------------------------------------
phdPopupMenuList::phdPopupMenuList() {

}

phdPopupMenuList::~phdPopupMenuList() {
	freeItems();
}

void phdPopupMenuList::freeItems() {
	for (vector<phdPopupMenuListItem*>::iterator i = items.begin(); i != items.end(); i++) {
		delete(*i); (*i) = NULL;
	}
	items.clear();
}

void phdPopupMenuList::clear() {
	items.clear();
}

int phdPopupMenuList::indexByPopup(phdPopupMenu * _item) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i]->itemMenu == _item) return i;
	}
	return -1;
}

int phdPopupMenuList::indexByName(string _name) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i]->itemName == _name) return i;
	}
	return -1;
}

phdPopupMenu * phdPopupMenuList::getPopupByName(string _name) {
	for(int i = 0; i < items.size(); i++) {
		if(items[i]->itemName == _name) return items[i]->itemMenu;
	}
	return NULL;
}

phdPopupMenuListItem * phdPopupMenuList::addItem(string _name, phdPopupMenu * _menu) {
	int _index = indexByName(_name);
	if(_index == -1) {
		items.push_back(new phdPopupMenuListItem(_name, _menu));
		return items[items.size()-1];
	}
	return items[_index];
}

void phdPopupMenuList::delItem(string _name) {
	int _index = indexByName(_name);
	if(_index > -1 && _index < items.size()) {
		delete items[_index];
		items.erase(items.begin() + _index);
	}
}
