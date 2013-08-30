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
		if(_parent != NULL) _result = _parent->caller;
		_parent = _parent->parent;
	}
	return _result;
}

//--------------------------------------------------------------------------------------------------------------
phdPopupMenu::phdPopupMenu() {
	usingEvents = false;
	visible = false;
	parent = NULL;
}

phdPopupMenu::phdPopupMenu(phdPopupMenu * _parent) {
	parent = _parent;
	usingEvents = false;
	visible = false;
}

phdPopupMenu::~phdPopupMenu() {
	freeItems();
	items.clear();
}

void phdPopupMenu::freeItems() {
	for(int i = 0; i <items.size(); i++) {
		if(items[i]->subMenu != NULL) items[i]->subMenu->freeItems();
		delete items[i];
	}
	items.clear();
}

void phdPopupMenu::setVisible(bool _value) {
	if(_value != visible) {
		visible = _value;
		if(!visible) {
			disableEvents();
			for(int i = 0; i < items.size(); i++) {
				if(items[i]->subMenu != NULL) items[i]->subMenu->setVisible(false); // ocults every child
			}
		} else {
			if(items.size() > 0) {
				enableEvents();
			}
		}
	}
}

phdMenuItem * phdPopupMenu::addItem(phdMenuItem * _item) {
	if(_item != NULL) {
		items.push_back(_item);
		_item->parent = (phdPopupMenu*)this;
		updateDimensions();
	}
	return _item;
}

phdMenuItem * phdPopupMenu::addItem(string _name, int _id, phdPopupMenu * _subMenu, bool _grouped) {
	phdMenuItem * _result = addItem(new phdMenuItem(_name, _id, (phdPopupMenu*)this, _subMenu, _grouped));
	return _result;
}

void phdPopupMenu::updateDimensions() {

	nCols = 1 + (int)(items.size() / menuMaxItemsPerCol); // 15 max in a col

	nRows = (int)(items.size() / nCols);
	if(items.size() % nCols != 0) nRows += 1;

	colWidth = getWidth();
	menuHeight = getHeight();  // actualize width / height
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
			if(hitTestInfo.focusedIndex == i) {
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

void phdPopupMenu::setFocused(phdMenuItem * _item) {
	//if(_item != hitTestInfo.focused) {
		hitTestInfo.focused = _item;
	//}
}

void phdPopupMenu::setClicked(phdMenuItem * _item) {

	// hides every all submenus from items of this menu
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

void phdPopupMenu::updateHitTestInfo(float _x, float _y) {
	hitTestInfo.infoType = pphOutside;
	hitTestInfo.focusedIndex = -1;
	if(_x >= x && _x <= x+colWidth*nCols && _y >= y && _y <= y+menuHeight) {
		hitTestInfo.infoType = pphInside;
		if(items.size() == 0) {
			hitTestInfo.focusedIndex = -1;
			setFocused(NULL);
		} else {
			//hitTestInfo.focusedIndex = ((_y-y) / h) * items.size();

			int _r = (int)((_y-y) / menuItemHeight);
			int _c = (int)((_x-x) / colWidth);

			hitTestInfo.focusedIndex = _c * nRows + _r;

			if(hitTestInfo.focusedIndex > -1 && hitTestInfo.focusedIndex < items.size()) {
				setFocused(items[hitTestInfo.focusedIndex]);
			} else {
				hitTestInfo.focusedIndex = -1;
				setFocused(NULL);
			}
		}
	} else {
		setFocused(NULL);
	}
}

void phdPopupMenu::mouseMoved(ofMouseEventArgs& args) {
	updateHitTestInfo(args.x, args.y);
}

void phdPopupMenu::mousePressed(ofMouseEventArgs& args) {
	updateHitTestInfo(args.x, args.y);
}

void phdPopupMenu::mouseDragged(ofMouseEventArgs& args) {
	updateHitTestInfo(args.x, args.y);
}

bool phdPopupMenu::testInsideMeAndMySubMenus(float _x, float _y) {
	bool _result = (_x >= x && _x <= x+colWidth*nCols && _y >= y && _y <= y+menuHeight);
	for(int i = 0; i < items.size(); i++) {
		if(items[i]->subMenu != NULL) { _result = (_result || items[i]->subMenu->testInsideMeAndMySubMenus(_x,_y)); }
	}
	return _result;
}

void phdPopupMenu::mouseReleased(ofMouseEventArgs& args) {
	updateHitTestInfo(args.x, args.y);
	if(hitTestInfo.infoType == pphInside && hitTestInfo.focusedIndex != -1) {
		setClicked(items[hitTestInfo.focusedIndex]);
	} else if(hitTestInfo.infoType == pphOutside) {
		bool _found = testInsideMeAndMySubMenus(args.x,args.y);
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

void phdPopupMenu::enableEvents() {
	if(!usingEvents) {
		ofAddListener(ofEvents().mouseMoved, this, &phdPopupMenu::mouseMoved);
		ofAddListener(ofEvents().mousePressed, this, &phdPopupMenu::mousePressed);
		ofAddListener(ofEvents().mouseDragged, this, &phdPopupMenu::mouseDragged);
		ofAddListener(ofEvents().mouseReleased, this, &phdPopupMenu::mouseReleased);
		usingEvents = true;
	}
}

void phdPopupMenu::disableEvents() {
	if(usingEvents) {
		ofRemoveListener(ofEvents().mouseMoved, this, &phdPopupMenu::mouseMoved);
		ofRemoveListener(ofEvents().mousePressed, this, &phdPopupMenu::mousePressed);
		ofRemoveListener(ofEvents().mouseDragged, this, &phdPopupMenu::mouseDragged);
		ofRemoveListener(ofEvents().mouseReleased, this, &phdPopupMenu::mouseReleased);
		usingEvents = false;
	}
}
