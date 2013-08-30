#include "phdMappingControl.h"

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
phdMappingControl::phdMappingControl() {
	setDimension(300,75);
}

phdMappingControl::~phdMappingControl() {
	/*if(getManager() != NULL) {
		int _index = getManager()->indexOf(&mapping.masterOutput);
		if(_index != -1) {
			getManager()->deleteItem(_index);
		}
	}*/
}

void phdMappingControl::setPosition(float _x, float _y) {
	setX(_x);
	setY(_y);
	updateHotspotsArea();
}

void phdMappingControl::setDimension(float _width, float _height) {
	setWidth(300); setHeight(75); // fixed size
	updateHotspotsArea();
}

void phdMappingControl::update() {
}

void phdMappingControl::draw() {

	ofPushStyle();

	ofFill();
	if(selected) {
		drawFilledBorderRectangle(hotTitleBar.x, hotTitleBar.y, hotTitleBar.w, hotTitleBar.h, ofColor(120,192,255,200), ofColor(255,255,255,255));
	} else {
		drawFilledBorderRectangle(hotTitleBar.x, hotTitleBar.y, hotTitleBar.w, hotTitleBar.h, ofColor(160,92,220,200), ofColor(255,255,255,255));
	}

	ofSetColor(255,255,128,255);
	ofDrawBitmapString(mapping.fileName, hotTitleBar.x + 6.0, hotTitleBar.y + 10.0);

	for(int i = 0; i < hotspots.size(); i++) {
		if(hitTestInfo.hotspotIndex == i) {
			drawFilledBorderRectangle(hotspots[i].x, hotspots[i].y, hotspots[i].w, hotspots[i].h, ofColor(160,92,220,200), ofColor(255,255,255,255));
			ofDrawBitmapString(hotLabels[i], hotspots[i].x + 10.0, hotspots[i].y + 18.0);
		} else {
			drawFilledBorderRectangle(hotspots[i].x, hotspots[i].y, hotspots[i].w, hotspots[i].h, ofColor(192,128,255,100), ofColor(162,162,96,255));
			ofDrawBitmapString(hotLabels[i], hotspots[i].x + 10.0, hotspots[i].y + 18.0);
		}
	}

	ofPopStyle();
}

void phdMappingControl::updateHotspotsArea() {

	hotTitleBar.setup(getX(), getY(), getWidth(), 15);

	float _x = getX();
	float _y = getY() + hotTitleBar.h;
	float _w = getWidth() / 2.0;
	float _h = (getHeight() - hotTitleBar.h) / 2.0;

	while(hotspots.size() < nLabels) hotspots.push_back(phdHotspotArea());

	for(int i = 0; i < hotspots.size(); i++) {
		hotspots[i].setup(_x, _y, _w, _h);
		_x += _w;
		if(_x + _w > getX() + getWidth()) {
			_y += _h; _x = getX();
		}
	}
}

bool phdMappingControl::updateHitTestInfo(float _x, float _y) {

	hitTestInfo.infoType = mhtOutside;
	hitTestInfo.mouseX = _x;
	hitTestInfo.mouseY = _y;
	hitTestInfo.popupIndex = -1;
	hitTestInfo.hotspotIndex = -1;

	if(getPopupMenu() != NULL && getPopupMenu()->testInsideMeAndMySubMenus(_x,_y)) {
		hitTestInfo.infoType = mhtPopup;
	}

	if(hitTestInfo.infoType != mhtPopup) {
		if(hotTitleBar.pointInside(_x,_y)) {
			hitTestInfo.infoType = mhtTitleBar;
		} else {
			for(int i = 0; i < hotspots.size(); i++) {
				if(hotspots[i].pointInside(_x, _y)) {
					hitTestInfo.infoType = mhtHotspot;
					hitTestInfo.hotspotIndex = i;
					break;
				}
			}
		}
	}

	if(hitTestInfo.infoType == mhtOutside && _x >= getX() && _x <= getX() + getWidth() && _y >= getY() && _y <= getY() + getHeight()) {
		hitTestInfo.infoType = mhtInside;
	}

	return (hitTestInfo.infoType != mhtOutside);
}

void phdMappingControl::onMouseMoved(ofMouseEventArgs& args) {
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMouseMoved(args);
		return;
	}

	updateHitTestInfo(args.x, args.y);

	if(hitTestInfo.infoType == mhtInside) {
	} else if(hitTestInfo.infoType == mhtPopup) {
	} else if(hitTestInfo.infoType == mhtTitleBar) {
	} else if(hitTestInfo.infoType == mhtHotspot) {
	}
}

void phdMappingControl::onMousePressed(ofMouseEventArgs& args) {
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMousePressed(args);
		return;
	}

	if(hitTestInfo.infoType == mhtInside) {
	} else if(hitTestInfo.infoType == mhtPopup) {
	} else if(hitTestInfo.infoType == mhtTitleBar) {
	} else if(hitTestInfo.infoType == mhtHotspot) {
	}
}

void phdMappingControl::onMouseDragged(ofMouseEventArgs& args) {
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMouseDragged(args);
		return;
	}

	if(hitTestInfo.infoType == mhtInside) {
	} else if(hitTestInfo.infoType == mhtPopup) {
	} else if(hitTestInfo.infoType == mhtTitleBar) {

		setX(getX() + args.x - hitTestInfo.mouseX);
		setY(getY() + args.y - hitTestInfo.mouseY);

		hitTestInfo.mouseX = args.x;
		hitTestInfo.mouseY = args.y;

		updateHotspotsArea();

	} else if(hitTestInfo.infoType == mhtHotspot) {

	}
}

void phdMappingControl::onMouseReleased(ofMouseEventArgs& args) {
	if(getPopupMenu() != NULL && getPopupMenu()->getVisible()) {
		getPopupMenu()->onMouseReleased(args);
		return;
	}

	if(hitTestInfo.infoType == mhtInside) {
		if(args.button == 2 && getPopupMenu() != NULL) {
			getPopupMenu()->setOwner(this);
			getPopupMenu()->popup(args.x, args.y, (void*)this, "MAPPING:");
			ofAddListener(getPopupMenu()->popEvent, this, &phdMappingControl::onPopupClick);
		}
	} else if(hitTestInfo.infoType == mhtPopup) {

	} else if(hitTestInfo.infoType == mhtTitleBar) {

	} else if(hitTestInfo.infoType == mhtHotspot) {
		if(hitTestInfo.hotspotIndex == 0) {			// load mapping

			ofFileDialogResult fileDialogResult = ofSystemLoadDialog("Escolha um arquivo", false, "");
			if(fileDialogResult.bSuccess && getManager() != NULL){

				//msg += " " + fileDialogResult.getPath();
				//msg += " " + fileDialogResult.getName();

				for(int i = getManager()->size()-1; i > -1; i--) {
					if((*getManager())[i]->getClassName() == "PHDDESIGNER") {
						getManager()->delControl(i);
					}
				}

				mapping.loadFromFile(fileDialogResult.getPath());

				getManager()->setFocusedControl(NULL);
				getManager()->setSelectedControl(NULL);

				for(int i = 0; i < mapping.designers.size(); i++) {
					mapping.designers[i]->setInputClip(NULL);
					getManager()->addControl((phdControlBase*)&(*getManager()).mapping.designers[i]);
				}

				getManager()->mapping.masterOutput.setInputClip(NULL);
				getManager()->addControl((phdControlBase*)&(*getManager()).mapping.masterOutput);
			}
		} else if(hitTestInfo.hotspotIndex == 1) {  // save mapping

			getManager()->mapping.saveToFile(getManager()->mapping.fileName);

		} else if(hitTestInfo.hotspotIndex == 2) {	// new designer

		} else if(hitTestInfo.hotspotIndex == 3) {	// new schema

		}
	}
}

void phdMappingControl::onPopupClick(phdPopupMenuEventData &_eventData) {
	if(_eventData.menu != NULL) {
		ofRemoveListener(_eventData.menu->popEvent, this, &phdMappingControl::onPopupClick);
	}
}
