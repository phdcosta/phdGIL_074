#pragma once

#include "ofMain.h"

#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>

//--------------------------------------------------
//--------------------------------------------------
const static char * jointNames[25] =
{ "PH_SKEL_NONE",
  "XN_SKEL_HEAD", "XN_SKEL_NECK", "XN_SKEL_TORSO", "XN_SKEL_WAIST",
  "XN_SKEL_LEFT_COLLAR", "XN_SKEL_LEFT_SHOULDER", "XN_SKEL_LEFT_ELBOW", "XN_SKEL_LEFT_WRIST", "XN_SKEL_LEFT_HAND", "XN_SKEL_LEFT_FINGERTIP",
  "XN_SKEL_RIGHT_COLLAR", "XN_SKEL_RIGHT_SHOULDER", "XN_SKEL_RIGHT_ELBOW", "XN_SKEL_RIGHT_WRIST", "XN_SKEL_RIGHT_HAND", "XN_SKEL_RIGHT_FINGERTIP",
  "XN_SKEL_LEFT_HIP", "XN_SKEL_LEFT_KNEE", "XN_SKEL_LEFT_ANKLE", "XN_SKEL_LEFT_FOOT", 
  "XN_SKEL_RIGHT_HIP", "XN_SKEL_RIGHT_KNEE", "XN_SKEL_RIGHT_ANKLE", "XN_SKEL_RIGHT_FOOT" };

const static int PH_NUMBER_OF_JOINTS = 24;

class skelJoints {
public:
	ofVec3f wrdJoints[PH_NUMBER_OF_JOINTS+1];
	ofVec3f scrJoints[PH_NUMBER_OF_JOINTS+1];
	
	skelJoints();
	~skelJoints();
	void clear();
	void sum(skelJoints * _joints);
	void divBy(float _divisor);
	void setJoints(skelJoints * _joints);
	void setWorldJoint(int _index, float _x, float _y, float _z);
	void setScreenJoint(int _index, float _x, float _y, float _z);
	string getJointName(int _index);
	void drawCircle(int _size);
	void drawRect(int _size);
	bool compareJoints(skelJoints * _compare);
	bool compareScrJoints(skelJoints * _compare);
	bool compareWrdJoints(skelJoints * _compare);
};

//**********************************************************************
//**********************************************************************
class skelJointsHistoryItem {
public:
	int millis;
	skelJoints * joints;

	skelJointsHistoryItem();
	~skelJointsHistoryItem();
	void updateJoints(skelJoints * _values, int _millis);
};

//**********************************************************************
//**********************************************************************
class skelJointsHistory {
public:
	vector<skelJointsHistoryItem*> items;
	skelJointsHistoryItem * curItem;
	int historySize;
	int historyIndex;

	int recordSize;
	vector<skelJointsHistoryItem*> record;

	skelJointsHistory(int _size);
	~skelJointsHistory();
	void freeItems();
	void freeRecord();
	void setIndex(int _index);
	void beginRecord(int _size);
	void updateRecord(skelJoints * _values, int _millis);
	skelJointsHistoryItem * getNextCircularItem();
	int getNextIndex(int _index);
	int getPrevIndex(int _index);
	void calcAverage(int _now, int _duration, skelJoints * _average);
	bool changedInLastNUpdates(int _updates, skelJoints * _values);
	void drawCircle(int _size);
	void drawRect(int _size);
};

//**********************************************************************
//**********************************************************************
class skelJointsRecorder {
public:
	bool recording;
	int recordLimit;
	int recordStart;
	int recordEnd;
	vector<skelJointsHistoryItem*> items;

	int playElapsed;
	bool playing;

	skelJointsRecorder();
	~skelJointsRecorder();
	void freeItems();
	void startRecording(int _now, int _duration, int _limit);
	void updateRecord(skelJoints * _values, int _now);
	void stopRecording();
	int getRecordLength();
	skelJoints * getByPercentagePos(float _value);
	skelJoints * getByElapsedTime(int _elapsed);
	void startPlaying();
	void updatePlaying(int _mov, bool _loop);
	void stopPlaying();
};

//**********************************************************************
//**********************************************************************
class skelTracked {
public:
	XnUserID user;
	bool tracking;
	int lastUpdate;

	skelJoints * curJoints;
	skelJoints * oldJoints;
	skelJointsHistory * history;

	skelTracked(XnUserID _user, int _historySize);
	~skelTracked();
	void saveCoords();
	bool compareCoords(int _now);
	void setWorldJoint(int _index, float _x, float _y, float _z);
	void setScreenJoint(int _index, float _x, float _y, float _z);
	void updateJoints(skelJoints * _sk, int _millis);
	bool isActive(int _now);
};

//**********************************************************************
//**********************************************************************
class skelTrackedList {
public:
	vector<skelTracked*> items;

	skelTrackedList();
	~skelTrackedList();

	void freeItems();
	skelTracked * addSkel(XnUserID _user, int _historySize);
	skelTracked * findSkelByUser(XnUserID _user);
	int countTracked();
	int countActive();
};

//**********************************************************************
//**********************************************************************
class skelPoseCondition {
public:
	string name;

	skelPoseCondition();
	~skelPoseCondition();
	void print(string _msg);
	virtual void initialize();
	virtual bool testCondition(skelTracked _sk);
};

//--------------------------------------------------
// mão esquerda acima da cabeça
//--------------------------------------------------
class skelPoseLeftHandOverHead : public skelPoseCondition {
	void initialize();
	bool testCondition(skelTracked _sk);
};

//--------------------------------------------------
// mão esquerda abaixo da cabeça
//--------------------------------------------------
class skelPoseLeftHandBelowHead : public skelPoseCondition {
	void initialize();
	bool testCondition(skelTracked _sk);
};

//--------------------------------------------------
// mão direita acima da cabeça
//--------------------------------------------------
class skelPoseRightHandOverHead : public skelPoseCondition {
	void initialize();
	bool testCondition(skelTracked _sk);
};

//--------------------------------------------------
// mão direita acima da cabeça
//--------------------------------------------------
class skelPoseRightHandBelowHead : public skelPoseCondition {
	void initialize();
	bool testCondition(skelTracked _sk);
};

//--------------------------------------------------
// ombro esquerdo à esquerda do ombro direito = está de frente para a câmera
//--------------------------------------------------
class skelPoseDeFrente : public skelPoseCondition {
	void initialize();
	bool testCondition(skelTracked _sk);
};
//--------------------------------------------------
// ombro direito à esquerda do ombro esquerda = está de costas para a câmera
//--------------------------------------------------
class skelPoseDeCostas : public skelPoseCondition {
	void initialize();
	bool testCondition(skelTracked _sk);
};

//--------------------------------------------------
// mao esquerda entre os cotovelos
//--------------------------------------------------
class skelPoseLeftHandInsideElbows : public skelPoseCondition {
	void initialize();
	bool testCondition(skelTracked _sk);
};

//--------------------------------------------------
// mao esquerda fora dos cotovelos
//--------------------------------------------------
class skelPoseLeftHandOutsideElbows : public skelPoseCondition {
	void initialize();
	bool testCondition(skelTracked _sk);
};

//--------------------------------------------------
// mao direita entre os cotovelos
//--------------------------------------------------
class skelPoseRightHandInsideElbows : public skelPoseCondition {
	void initialize();
	bool testCondition(skelTracked _sk);
};

//--------------------------------------------------
// mao direita fora os cotovelos
//--------------------------------------------------
class skelPoseRightHandOutsideElbows : public skelPoseCondition {
	void initialize();
	bool testCondition(skelTracked _sk);
};

//--------------------------------------------------
// mão esquerda dentro do poligono do torax
//--------------------------------------------------
class skelPoseMaoEsquerdaNoTorax : public skelPoseCondition {
	void initialize();
	bool myPolygonHitTest(int x, int y, vector<ofVec2f> pnts);
	bool testCondition(skelTracked _sk);
};

//**********************************************************************

class skelPose {
public:
	vector<skelPoseCondition*> conditions;
	string name;
	int id;

	skelPose(string _name, int _id);
	~skelPose();
	
	void addCondition(skelPoseCondition * _pc);
	bool testPose(skelTracked _sk);
	void setName(string _name);
	void print(string _msg);
};

class skelPoseList {
public:
	vector<skelPose*> items;

	skelPoseList();
	~skelPoseList();

	skelPose * addPose(string _name, int _id);
	int findPoseByName(string _name);
};

//**********************************************************************
enum actionPoseStatusEnum { asNone, asStarted, asFinished };
enum actionFinishedEnum { afNone, afTimeOutPose, afSpecificPose, afAnyPose, afStayInPose };

//--------------------------------------------------
// lista de ações que formam um gesto
//--------------------------------------------------

enum poseActionStatusEnum { paNotStarted, paStarted, paRunning, paFinished, paFailed };
const static char* actionStatusNames[5] = {"paNotStarted", "paStarted", "paRunning", "paFinished", "paFailed"};

class skelPoseActionItem {
public:
	skelPose * pose;
	actionFinishedEnum expectedEnd;
	int duration;

	skelPoseActionItem(skelPose * _pose, int _duration, actionFinishedEnum _expectedEnd);
	~skelPoseActionItem();
};

class skelPoseAction {
public:
	vector<skelPoseActionItem*> items;
	int startTime;
	int curPoseIndex;
	skelPose * curPoseObject;
	poseActionStatusEnum status;
	bool comecamosContarTempoNaPose;
	int elapsedTime;
	string name;
	int id;

	int poseAtual, poseAnterior;

	skelPoseAction();
	~skelPoseAction();

	skelPoseActionItem * addItem(skelPose* _pose, int _duration, actionFinishedEnum _expectedEnd);
	void restart();

	poseActionStatusEnum testAction(skelTracked _sk);
	void draw(float _x, float _y);
};

//--------------------------------------------------
//--------------------------------------------------
class skelPoseActionList {
public:
	vector<skelPoseAction*> actions;

	skelPoseActionList();
	~skelPoseActionList();

	int findById(int _id);
	int findByName(string _name);
	skelPoseAction * addAction(string _name, int _id);
};

