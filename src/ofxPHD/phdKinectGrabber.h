#pragma once

#include "ofMain.h"

#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>

#include "phdSkelTracker.h"
#include "phdGrabberBase.h"

//---------------------------------------------------------------------------------------------------------
static void XN_CALLBACK_TYPE NewUser(xn::UserGenerator& generator, XnUserID user, void* pCookie)
{
	printf("Novo Usuário id: %i\n", (unsigned int)user);
	
	// Find a user who isn't calibrated or currently in pose
	if (generator.GetSkeletonCap().IsCalibrated(user)) return;
	if (generator.GetSkeletonCap().IsCalibrating(user)) return;
	
	// Load user's calibration from file
	cout << ofToDataPath( "UserCalibration.bin", true) << endl;
	
	string calib = ofToDataPath( "UserCalibration.bin", true);
	const char *p; 	p=&calib[0];
	XnStatus rc = generator.GetSkeletonCap().LoadCalibrationDataFromFile(user, p);
	if (rc == XN_STATUS_OK){
		printf("Auto Skeleton data loaded OK. User: %i\n", user);
		// Make sure state is coherent
		generator.GetSkeletonCap().StartTracking(user);
		generator.GetPoseDetectionCap().StopPoseDetection(user);
	}else {
		printf("Auto Skeleton failed. User: %i\n", (unsigned int)user);
	}
	
};

static void XN_CALLBACK_TYPE LostUser(xn::UserGenerator& generator, XnUserID user, void* pCookie)
{
	printf("Perdemos o usuário: %i\n", (unsigned int)user);
};

// Callback: Detected a pose
static void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie)
{
	printf("Pose %s detected for user %d\n", strPose, nId);
};

// Callback: Started calibration
static void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie)
{
	printf("Calibration started for user %d\n", nId);
};

// Callback: Finished calibration
static void XN_CALLBACK_TYPE UserCalibration_CalibrationEnd(xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie) {
    if (bSuccess) {
		printf("Calibration complete, start tracking user %d\n", nId);
		capability.SaveCalibrationData(nId, 0);
		capability.StartTracking(nId);
		printf("Calibration complete, start tracking user %d\n", nId);
    }
};

//---------------------------------------------------------------------------------------------------------
class phdKinectTracker {
public:
	static const int finderW = 640;
	static const int finderH = 480;

	static const int masksW = 320;  // using this to reduce masks size
	static const int masksH = 240;

	ofTexture usrMask;
	ofTexture objMask;
	ofTexture fullMask;

	ofTexture texAlpha;
	ofTexture texColor;
	ofTexture texHalfColor;

	xn::Context context;
	xn::DepthGenerator depthGenerator;
	xn::UserGenerator userGenerator;
	xn::ImageGenerator imageGenerator;
	xn::SceneMetaData sceneMetaData;
	xn::DepthMetaData depthMetaData;
	xn::ImageMetaData imageData;

	unsigned char * tmpUsrMaskPixels;
	unsigned char * tmpObjMaskPixels;
	unsigned char * tmpFullMaskPixels;

	unsigned char * tmpColorPixels;
	unsigned char * tmpColorAlphaPixels;
	unsigned char * tmpHalfColorAlphaPixels;

	float * tmpFloatDepthMap;

	XnDepthPixel * oldDepthMap;
	int iDepthMapSize;
	bool saveBackground;
	float lowDepth, highDepth, maxDepth;

	// verificando se os esqueletos dos usuarios estão sendo procurados
	XnUserID aUsers[10];
	XnUInt16 nUsers;
	ofVec3f skel[26];

	skelTrackedList skels;

	phdKinectTracker() {
	}

	~phdKinectTracker() {
		context.Shutdown();
		delete [] tmpUsrMaskPixels;
		delete [] tmpObjMaskPixels;
		delete [] tmpFullMaskPixels;
		delete [] tmpColorPixels;
		delete [] tmpColorAlphaPixels;
		delete [] oldDepthMap;
		delete [] tmpFloatDepthMap;
	}

	//--------------------------------------------
	bool setup() {
		usrMask.allocate(masksW, masksH, GL_LUMINANCE);
		objMask.allocate(masksW, masksH, GL_LUMINANCE);
		fullMask.allocate(masksW, masksH, GL_LUMINANCE);

		texColor.allocate(finderW, finderH, GL_RGB);
		texAlpha.allocate(finderW, finderH, GL_RGBA);

		texHalfColor.allocate(masksW, masksH, GL_RGBA);

		tmpUsrMaskPixels = new unsigned char[masksW * masksH];	
		tmpObjMaskPixels = new unsigned char[masksW * masksH];	
		tmpFullMaskPixels = new unsigned char[masksW * masksH];	

		tmpColorPixels = new unsigned char[finderW * finderH * 3];
		tmpColorAlphaPixels = new unsigned char[finderW * finderH * 4];

		tmpHalfColorAlphaPixels = new unsigned char[masksW * masksH * 4];

		iDepthMapSize = finderW * finderH;// * sizeof(XnDepthPixel);
		oldDepthMap = new XnDepthPixel[iDepthMapSize];

		tmpFloatDepthMap = new float[finderW * finderH];

		XnStatus rc = context.InitFromXmlFile("data/Sample-User.xml");
		if( rc == XN_STATUS_OK) { printf("context.InitFromXmlFile\n"); } else { return false; }//OF_EXIT_APP(0); }

		printf("Depth Generator ---> ");
		if (XN_STATUS_OK != context.FindExistingNode(XN_NODE_TYPE_DEPTH, depthGenerator)) {
			if(XN_STATUS_OK == depthGenerator.Create(context)) {
				printf("Criou um novo Depth Generator\n");
			}
		}
		printf("OK <\n");

		printf("User Generator ---> ");
		if (XN_STATUS_OK != context.FindExistingNode(XN_NODE_TYPE_USER, userGenerator)) {
			if(XN_STATUS_OK == userGenerator.Create(context)) {
				printf("Criou um novo User Generator\n");
			} else { return false; }//OF_EXIT_APP(0); }
		}
		printf("OK <\n");
		
		printf("Image Generator ---> ");
		if (XN_STATUS_OK != context.FindExistingNode(XN_NODE_TYPE_IMAGE, imageGenerator)) {
			if(XN_STATUS_OK == imageGenerator.Create(context)) {
				printf("Criou um novo Image Generator\n");
			} else { return false; }//OF_EXIT_APP(0); }
			//imageGenerator.GetMirrorCap().SetMirror((XnBool) true);
		}
		printf("OK <\n");

		printf("Start Generating ---> ");
		if(XN_STATUS_OK != context.StartGeneratingAll()) { return false; }//OF_EXIT_APP(0); }
		printf("OK <\n");

		printf("Setup Callback ---> ");
		// define os callback para usuarios novos e tal
		XnCallbackHandle hUserCBs, hCalibrationCBs, hPoseCBs;
		userGenerator.RegisterUserCallbacks(NewUser, LostUser, NULL, hUserCBs);
		userGenerator.GetSkeletonCap().RegisterCalibrationCallbacks(UserCalibration_CalibrationStart, UserCalibration_CalibrationEnd, NULL, hCalibrationCBs);
		userGenerator.GetPoseDetectionCap().RegisterToPoseCallbacks(UserPose_PoseDetected, NULL, NULL, hPoseCBs);

		userGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
		depthGenerator.GetMetaData(depthMetaData);
		printf("OK <\n");

		// usado para ajustar a vista do gerador de imagens
		if (depthGenerator.IsCapabilitySupported(XN_CAPABILITY_ALTERNATIVE_VIEW_POINT)) {
			depthGenerator.GetAlternativeViewPointCap().SetViewPoint(imageGenerator);
		}

		saveBackground = true;
		lowDepth = -120;
		highDepth = 120;

		return true;
	}

	//--------------------------------------------
	void update() {

		context.WaitAndUpdateAll();

		depthGenerator.GetMetaData(depthMetaData);
		const XnDepthPixel* pDepth = depthGenerator.GetDepthMap();

		const XnDepthPixel* pOldDepth = oldDepthMap;

		userGenerator.GetUserPixels(0, sceneMetaData);
		const XnLabel* pLabels = sceneMetaData.Data();

		imageGenerator.GetMetaData(imageData);
		const XnRGB24Pixel* pImagePix = imageData.RGB24Data();

		maxDepth = depthGenerator.GetDeviceMaxDepth();

		XnUInt64 F_;
		XnDouble pixel_size_;

		// get the focal length in mm (ZPS = zero plane distance)
		depthGenerator.GetIntProperty ("ZPD", F_);

		// get the pixel size in mm ("ZPPS" = pixel size at zero plane)
		depthGenerator.GetRealProperty ("ZPPS", pixel_size_);

		unsigned char * pHPix = tmpHalfColorAlphaPixels;
		memset(pHPix, 0, masksW*masksH*4);

		int dH = 0;
		int hA = 0;
		int hI = 0;

		for(int y = 0, i = 0, c = 0, a = 0; y < finderH; y++) {
			int h = 0;
			for(int x = 0; x < finderW; x++, i++, c+=3, a+=4, h+=((x%2)*4)) {

				tmpFloatDepthMap[i] = (pOldDepth[i] - pDepth[i]);

				tmpColorPixels[c+0] = tmpColorAlphaPixels[a+0] = (unsigned char) pImagePix->nRed;
				tmpColorPixels[c+1] = tmpColorAlphaPixels[a+1] = (unsigned char) pImagePix->nGreen;
				tmpColorPixels[c+2] = tmpColorAlphaPixels[a+2] = (unsigned char) pImagePix->nBlue;

				hA = dH+h;
				hI = (int)((hA-1) * 0.25); // to do: pass this to shift right

				pHPix[hA+0] += (unsigned char) (pImagePix->nRed   >> 2);
				pHPix[hA+1] += (unsigned char) (pImagePix->nGreen >> 2);
				pHPix[hA+2] += (unsigned char) (pImagePix->nBlue  >> 2);

				if(pLabels[i] != 0) { // is a know user pixel?

					//tmpUsrMaskPixels[i] = (unsigned char) 255;
					//tmpObjMaskPixels[i] = (unsigned char) 0;
					//tmpFullMaskPixels[i] = (unsigned char) 255;

					tmpUsrMaskPixels[hI+0] = (unsigned char) 255;
					tmpObjMaskPixels[hI+0] = (unsigned char) 0;
					tmpFullMaskPixels[hI+0] = (unsigned char) 255;

					tmpColorAlphaPixels[a+3] = (unsigned char) 255; // alpha channel

					pHPix[hA+3] = 255;

				} else if((tmpFloatDepthMap[i] < lowDepth || tmpFloatDepthMap[i] > highDepth)) { // is an object pixel in range

					//tmpUsrMaskPixels[i] = (unsigned char) 0;
					//tmpObjMaskPixels[i] = (unsigned char) 255;
					//tmpFullMaskPixels[i] = (unsigned char) 255;

					tmpUsrMaskPixels[hI+0] = (unsigned char) 0;
					tmpObjMaskPixels[hI+0] = (unsigned char) 255;
					tmpFullMaskPixels[hI+0] = (unsigned char) 255;

					tmpColorAlphaPixels[a+3] = (unsigned char) 0;

					pHPix[hA+3] = 255;

				} else {

					//tmpUsrMaskPixels[i] = (unsigned char) 0;
					//tmpObjMaskPixels[i] = (unsigned char) 0;
					//tmpFullMaskPixels[i] = (unsigned char) 0;

					tmpUsrMaskPixels[hI+0] = (unsigned char) 0;
					tmpObjMaskPixels[hI+0] = (unsigned char) 0;
					tmpFullMaskPixels[hI+0] = (unsigned char) 0;

					tmpColorAlphaPixels[a+3] = (unsigned char) 0;

					pHPix[hA+3] = 0;
				}
				pImagePix++;
			}
			if(y % 2 != 0) dH += h;
		}

		// saves depth map as background
		if(saveBackground) {
			memcpy(oldDepthMap, pDepth, iDepthMapSize*sizeof(XnDepthPixel));
			saveBackground = false;
		}

		// atualizando as texturas
		usrMask.loadData(tmpUsrMaskPixels, masksW, masksH, GL_LUMINANCE);
		objMask.loadData(tmpObjMaskPixels, masksW, masksH, GL_LUMINANCE);
		fullMask.loadData(tmpFullMaskPixels, masksW, masksH, GL_LUMINANCE);

		texHalfColor.loadData(tmpHalfColorAlphaPixels, masksW, masksH, GL_RGBA);

		texColor.loadData(tmpColorPixels, finderW, finderH, GL_RGB);
		texAlpha.loadData(tmpColorAlphaPixels, finderW, finderH, GL_RGBA);

		int _now = ofGetElapsedTimeMillis();

		// verificando se os esqueletos dos usuarios estão sendo procurados
		XnUserID aUsers[10];
		//XnUInt16 nUsers = 10;
		userGenerator.GetUsers(aUsers, nUsers);

		//if(lock()) {

		skelJoints * _joints = new skelJoints();

		for(int i = 0; i < (int)nUsers; i++) {
			if (userGenerator.GetSkeletonCap().IsTracking(aUsers[i])) {
				XnSkeletonJointPosition jp;
				XnPoint3D pt;

				_joints->clear();

				for(int j = 1; j < PH_NUMBER_OF_JOINTS+1; j++) { // 1 based index
					userGenerator.GetSkeletonCap().GetSkeletonJointPosition(aUsers[i], (XnSkeletonJoint)j, jp);
					pt = jp.position;
					_joints->setWorldJoint(j, pt.X, pt.Y, pt.Z);
					if (jp.fConfidence > 0.2f) {
						depthGenerator.ConvertRealWorldToProjective(1, &pt, &pt);
						_joints->setScreenJoint(j, pt.X, pt.Y, pt.Z);
					}
				}

				skelTracked * _sk = skels.addSkel(aUsers[i], 100);
				_sk->updateJoints(_joints, _now);
			}
		}

		delete (_joints);
		//unlock(); }
	}

	//--------------------------------------------
	void draw() {
		int tx = 0, ty = 0, tw = 160, th = 120;

		ofSetColor(255,255,255,255);
		usrMask.bind();
		glBegin(GL_QUADS);
			glTexCoord2f(0,  0);			glVertex2f(tx+tw*0, ty+th*0);
			glTexCoord2f(masksW, 0);		glVertex2f(tx+tw*1, ty+th*0);
			glTexCoord2f(masksW, masksH);	glVertex2f(tx+tw*1, ty+th*1);
			glTexCoord2f(0,  masksH);		glVertex2f(tx+tw*0, ty+th*1);
		glEnd();
		usrMask.unbind();

		texColor.bind();
		glBegin(GL_QUADS);
			glTexCoord2f(0,  0);			glVertex2f(tx+tw*0, ty+th*1);
			glTexCoord2f(finderW, 0);		glVertex2f(tx+tw*1, ty+th*1);
			glTexCoord2f(finderW, finderH);	glVertex2f(tx+tw*1, ty+th*2);
			glTexCoord2f(0,  finderH);		glVertex2f(tx+tw*0, ty+th*2);
		glEnd();
		texColor.unbind();

		ofEnableAlphaBlending();
		texAlpha.bind();
		glBegin(GL_QUADS);
			glTexCoord2f(0,  0);			glVertex2f(tx+tw*0, ty+th*2);
			glTexCoord2f(finderW, 0);		glVertex2f(tx+tw*1, ty+th*2);
			glTexCoord2f(finderW, finderH);	glVertex2f(tx+tw*1, ty+th*3);
			glTexCoord2f(0,  finderH);		glVertex2f(tx+tw*0, ty+th*3);
		glEnd();
		texAlpha.unbind();

		objMask.bind();
		glBegin(GL_QUADS);
			glTexCoord2f(0,  0);			glVertex2f(tx+tw*0, ty+th*3);
			glTexCoord2f(masksW, 0);		glVertex2f(tx+tw*1, ty+th*3);
			glTexCoord2f(masksW, masksH);	glVertex2f(tx+tw*1, ty+th*4);
			glTexCoord2f(0,  masksH);		glVertex2f(tx+tw*0, ty+th*4);
		glEnd();
		objMask.unbind();

		fullMask.bind();
		glBegin(GL_QUADS);
			glTexCoord2f(0,  0);			glVertex2f(tx+tw*0, ty+th*4);
			glTexCoord2f(masksW, 0);		glVertex2f(tx+tw*1, ty+th*4);
			glTexCoord2f(masksW, masksH);	glVertex2f(tx+tw*1, ty+th*5);
			glTexCoord2f(0,  masksH);		glVertex2f(tx+tw*0, ty+th*5);
		glEnd();
		fullMask.unbind();

		texHalfColor.bind();
		glBegin(GL_QUADS);
			glTexCoord2f(0,  0);			glVertex2f(tx+tw*0, ty+th*5);
			glTexCoord2f(masksW, 0);		glVertex2f(tx+tw*1, ty+th*5);
			glTexCoord2f(masksW, masksH);	glVertex2f(tx+tw*1, ty+th*6);
			glTexCoord2f(0,  masksH);		glVertex2f(tx+tw*0, ty+th*6);
		glEnd();
		texHalfColor.unbind();

		int w = finderW;
		int h = finderH;
		int step = 1;

		// depth map
		step = 4;
		glPointSize(3);
		for(int y = 25; y < finderH-5; y+=step) {
			int i = y * finderW + 40;
			int a = i * 4;
			glBegin(GL_LINE_STRIP);
			for(int x = 40; x < finderW-12; x+=step, i+=step ) {
				a = i * 4;
				glColor4ub((unsigned char)tmpColorAlphaPixels[a+0], (unsigned char)tmpColorAlphaPixels[a+1], (unsigned char)tmpColorAlphaPixels[a+2], 255);//(unsigned char)tmpPointCloud[i].a);
				glVertex3f(x+200, y+200, tmpFloatDepthMap[i]*.2);
			}
			glEnd();
		}

		ofSetColor(255,0,0,255);
		for(int i = 0; i < skels.items.size(); i++) {
			skels.items[i]->curJoints->drawCircle(4.0);
		}
	
		glColor3f(1.0f, 1.0f, 1.0f);
		ofDisableAlphaBlending();

		ofSetColor(255,0,0,255);
		ofDrawBitmapString(
			"FPS:" + ofToString(ofGetFrameRate(), 2) +
			"\n Users:" + ofToString(nUsers) +
			"\n lowDepth:" + ofToString(lowDepth) +
			"\n highDepth:" + ofToString(highDepth) +
			"\n maxepth:" + ofToString(maxDepth), 180, 20);
		}
		//--------------------------------------------
};

//---------------------------------------------------------------------------------------------------------
class phdKinectGrabber : public phdGrabberBase {
protected:
	bool hasKinect;
	bool bIsInitialized;
	phdKinectTracker kinect;

public:
	phdKinectGrabber() {
		hasKinect = bIsInitialized = false;
		internalWidth = 320;
		internalHeight = 240;
	}

	~phdKinectGrabber() { }

	string getClassName() { return "PHDKINECTGRABBER"; }

	phdKinectTracker & getTracker() { return kinect; } 

	void configureGrabber() {
		if(!bIsInitialized) {
			hasKinect = kinect.setup();
		}

		bIsInitialized = true;
	};

	bool getIsInitialized() { return hasKinect && bIsInitialized; }

	void setWidth(float _width) { configureGrabber(); }
	float getWidth() { return internalWidth; }

	void setHeight(float _height) { configureGrabber(); }
	float getHeight() { return internalHeight; }

	inline int genImagesCount() { return 2; }
	inline int genMasksCount() { return 3; }
	inline bool getData() { return true; }

	void update(phdGenParams * _params = NULL) {
		int _frame = ofGetFrameNum();
		if(bIsInitialized && hasKinect && _frame != lastUpdatedFrame) {

			if(_params != NULL && _params->updateBackground) {
				kinect.saveBackground = true;
				_params->updateBackground = false;
			}

			kinect.update();
			lastUpdatedFrame = _frame;
		}
	}

	void draw(phdGenParams * _params = NULL) {
		draw( 0, 0, getWidth(), getHeight(), _params);
	}

	void draw(float x, float y, float w, float h, phdGenParams * _params = NULL) {
		if(hasKinect && bIsInitialized) {
			//kinect.draw(x,y,w,h);
			kinect.texColor.draw(x, y, w, h);
		}
	}

	void setSetupString(string _setupString) {
	}

	string getSetupString() {
		return "GRABBER|" + getClassName() + "|";
	}

	void getNumericData(vector<float> & _results, vector<string> & _names) { }
	void getStringData(vector<string> & _results, vector<string> & _names) { }
};

//--------------------------------------------------------------------------------------------------------------
