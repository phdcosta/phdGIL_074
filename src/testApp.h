#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#include "phdGenFbo.h"
#include "phdClipTrack.h"
#include "phdBlend.h"
#include "phdFilter.h"
#include "phdViewport.h"
#include "phdDesigner.h"
#include "phdMapping.h"
#include "phdProjector.h"
#include "phdControlManager.h"

#define PM_MANAGER		1000
#define PM_DESIGNER		2000
#define PM_OUTPUT		3000
#define PM_SHAPEIN		4000
#define PM_SHAPEMODE	4100
#define PM_SHAPEOUT		5000
#define PM_MIRROR		5100
#define PM_CLIP			6000
#define PM_GRABBERS		6100
#define PM_GENERATORS	6200
#define PM_TRACK		7000
#define PM_MAPPING		8000

enum phdViewModeType { vmtControls, vmtProjectorContents, vmtProjectorWarpers, vmtInputGrabberWarpers };

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

		void managerPopupClick(phdPopupMenuEventData &_eventData);
		void designerPopupClick(phdPopupMenuEventData &_eventData);
		void outputPopupClick(phdPopupMenuEventData &_eventData);
		void shapeInPopupClick(phdPopupMenuEventData &_eventData);
		void shapeOutPopupClick(phdPopupMenuEventData &_eventData);
		void mirrorPopupClick(phdPopupMenuEventData &_eventData);
		void shapeModePopupClick(phdPopupMenuEventData &_eventData);
		void clipPopupClick(phdPopupMenuEventData &_eventData);
		void generatorsPopupClick(phdPopupMenuEventData &_eventData);
		void grabbersPopupClick(phdPopupMenuEventData &_eventData);
		void trackPopupClick(phdPopupMenuEventData &_eventData);
		void mappingPopupClick(phdPopupMenuEventData &_eventData);

		string msg;

		phdPopupMenu managerPopup, designerPopup, outputPopup;
		phdPopupMenu shapeInPopup, shapeOutPopup, mirrorPopup, shapeModePopup;
		phdPopupMenu clipPopup, trackPopup, mappingPopup, generatorsPopup, grabbersPopup;

		phdControlManager controlManager;
		phdMappingControl * _mapping;

		bool bEditingGrabberWarpers;
		bool bShowProjectors;
		bool bShowControls;

		void updateOutputWindowSize(int _width, int _height);
		float outputWidth;
		float outputHeight;
};

