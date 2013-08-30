#pragma once

#include "ofMain.h"

#include "phdSkelTracker.h"

//------------------------------------------------------------------------------------------------------
skelJoints::skelJoints() {
	clear();
}

skelJoints::~skelJoints() {
}

void skelJoints::clear() {
	for(int i = 1; i < PH_NUMBER_OF_JOINTS+1; i++) { // 1 based index
		wrdJoints[i].set(0.0);
		scrJoints[i].set(0.0);
	}
}

void skelJoints::sum(skelJoints * _joints) {
	for(int i = 1; i < PH_NUMBER_OF_JOINTS+1; i++) { // 1 based index
		wrdJoints[i] += _joints->wrdJoints[i];
		scrJoints[i] += _joints->scrJoints[i];
	}
}

void skelJoints::divBy(float _divisor) {
	for(int i = 1; i < PH_NUMBER_OF_JOINTS+1; i++) { // 1 based index
		wrdJoints[i] /= _divisor;
		scrJoints[i] /= _divisor;
	}
}

void skelJoints::setJoints(skelJoints * _joints) {
	for(int i = 1; i < PH_NUMBER_OF_JOINTS+1; i++) { // 1 based index
		wrdJoints[i] != _joints->wrdJoints[i];
		scrJoints[i] != _joints->scrJoints[i];
	}
}

void skelJoints::setWorldJoint(int _index, float _x, float _y, float _z) {
	wrdJoints[_index].set(_x, _y, _z);
}

void skelJoints::setScreenJoint(int _index, float _x, float _y, float _z) {
	scrJoints[_index].set(_x, _y, _z);
}

string skelJoints::getJointName(int _index) {
	return jointNames[_index];
}

void skelJoints::drawCircle(int _size) {
	for(int i = 1; i < PH_NUMBER_OF_JOINTS+1; i++) { // 1 based index
		ofVec3f v = scrJoints[i];
		ofCircle(v.x, v.y, _size);
	}
}

void skelJoints::drawRect(int _size) {
	for(int i = 1; i < PH_NUMBER_OF_JOINTS+1; i++) { // 1 based index
		ofVec3f v = scrJoints[i];
		ofRect(v.x, v.y, _size, _size);
	}
}

bool skelJoints::compareJoints(skelJoints * _compare) {
	for(int i = 1; i < PH_NUMBER_OF_JOINTS+1; i++) { // 1 based index
		if(scrJoints[i] != _compare->scrJoints[i] || wrdJoints[i] != _compare->wrdJoints[i]) return false;
	}
	return true;
}

bool skelJoints::compareScrJoints(skelJoints * _compare) {
	for(int i = 1; i < PH_NUMBER_OF_JOINTS+1; i++) { // 1 based index
		if(scrJoints[i] != _compare->scrJoints[i]) return false;
	}
	return true;
}

bool skelJoints::compareWrdJoints(skelJoints * _compare) {
	for(int i = 1; i < PH_NUMBER_OF_JOINTS+1; i++) { // 1 based index
		if(wrdJoints[i] != _compare->wrdJoints[i]) return false;
	}
	return true;
}

//------------------------------------------------------------------------------------------------------
skelJointsHistoryItem::skelJointsHistoryItem() {
	joints = new skelJoints;
	millis = ofGetElapsedTimeMillis();
}

skelJointsHistoryItem::~skelJointsHistoryItem() {
	delete joints;
}

void skelJointsHistoryItem::updateJoints(skelJoints * _values, int _millis) {
	for(int i = 1; i < PH_NUMBER_OF_JOINTS+1; i++) { // 1 based index
		joints->wrdJoints[i] = _values->wrdJoints[i];
		joints->scrJoints[i] = _values->scrJoints[i];
	}
	millis = _millis;
}

//------------------------------------------------------------------------------------------------------
skelJointsHistory::skelJointsHistory(int _size) {
	historySize = _size;
	curItem = NULL;

	recordSize = 0;
}

skelJointsHistory::~skelJointsHistory() {
	curItem = NULL;
	freeItems();
	freeRecord();
}

void skelJointsHistory::freeItems() {
	for (vector<skelJointsHistoryItem*>::iterator i = items.begin(); i != items.end(); i++) {
		delete(*i);
	}
	items.clear();
}

void skelJointsHistory::freeRecord() {
	for (vector<skelJointsHistoryItem*>::iterator i = record.begin(); i != record.end(); i++) {
		delete(*i);
	}
	record.clear();
}

void skelJointsHistory::setIndex(int _index) {
	historyIndex = _index % items.size();
	curItem = items[historyIndex];
}

void skelJointsHistory::beginRecord(int _size) {
	freeRecord();
	recordSize = _size;
}

void skelJointsHistory::updateRecord(skelJoints * _values, int _millis) {
	if(record.size() < recordSize) {
		skelJointsHistoryItem * result = new skelJointsHistoryItem();
		result->updateJoints(_values, _millis);
		record.push_back(result);
	}
}

skelJointsHistoryItem * skelJointsHistory::getNextCircularItem() {
	skelJointsHistoryItem * result;
	if(items.size() < historySize) {
		result = new skelJointsHistoryItem();
		items.push_back(result);
		setIndex(items.size()-1);
	} else {
		setIndex((historyIndex + 1) % historySize);
		result = items[historyIndex];
	}
	return result;
}

int skelJointsHistory::getNextIndex(int _index) {
	int result = (_index + 1) % items.size();
	return result;
}

int skelJointsHistory::getPrevIndex(int _index) {
	int result = _index;
	if(result == 0) result = items.size();
	result -= 1;
	return result;
}

void skelJointsHistory::calcAverage(int _now, int _duration, skelJoints * _average) {
	int _count = 0;
	_average->clear();
    for (vector<skelJointsHistoryItem*>::iterator i = items.begin(); i != items.end(); i++) {
		if(_now - (*i)->millis < _duration) {
			_average->sum((*i)->joints);
			_count += 1;
		}
	}
	if(_count > 0) _average->divBy(_count);
}

bool skelJointsHistory::changedInLastNUpdates(int _updates, skelJoints * _values) {
	int _index = getPrevIndex(historyIndex);
	for(int i = 0; i < _updates; i++) {
		if(!_values->compareJoints(items[_index]->joints)) return true;
		_index = getPrevIndex(_index);
	}
	return false;
}

void skelJointsHistory::drawCircle(int _size) {
	int k1 = historyIndex;
	int k2 = getNextIndex(k1);
	int k3 = 0;
	while(k1 != k2) {
		//ofSetColor(fColors[k3 % 25][0], fColors[k3 % 25][1], fColors[k3 % 25][2], 255);
		items[k2]->joints->drawCircle(_size);
		k2 = getNextIndex(k2);
		k3 += 1;
	}
}

void skelJointsHistory::drawRect(int _size) {
	int k1 = historyIndex;
	int k2 = getNextIndex(k1);
	int k3 = 0;
	while(k1 != k2) {
		//ofSetColor(fColors[k3 % 25][0], fColors[k3 % 25][1], fColors[k3 % 25][2], 255);
		items[k2]->joints->drawRect(_size);
		k2 = getNextIndex(k2);
		k3 += 1;
	}
}

//------------------------------------------------------------------------------------------------------
skelJointsRecorder::skelJointsRecorder() {
	recordLimit = 0;
	recording = false;
	playElapsed = 0;
	playing = false;
}

skelJointsRecorder::~skelJointsRecorder() {
	freeItems();
}

void skelJointsRecorder::freeItems() {
	for (vector<skelJointsHistoryItem*>::iterator i = items.begin(); i != items.end(); i++) {
		delete(*i);
	}
	items.clear();
}

void skelJointsRecorder::startRecording(int _now, int _duration, int _limit) {
	freeItems();
	playing = false;
	recording = true;
	recordStart = _now;
	recordEnd = recordStart + _duration;
	recordLimit = _limit;
	if(recordLimit < 1) recordLimit = 900; // 900 items maximum about 30s x 30fps
}

void skelJointsRecorder::updateRecord(skelJoints * _values, int _now) {
	recording = (recording && items.size() < recordLimit && _now < recordEnd);
	if(recording) {
		skelJointsHistoryItem * result = new skelJointsHistoryItem();
		result->updateJoints(_values, _now);
		items.push_back(result);
	}
}

void skelJointsRecorder::stopRecording() {
	recording = false;
}

int skelJointsRecorder::getRecordLength() {
	if(items.size() > 0) {
		return items[items.size()-1]->millis - items[0]->millis;
	}
	return 0;
}

skelJoints * skelJointsRecorder::getByPercentagePos(float _value) {
	if(items.size() > 0) {
		int _pos = (int)(items.size() * fabs(_value));
		return items[_pos % items.size()]->joints;
	}
	return NULL;
}

skelJoints * skelJointsRecorder::getByElapsedTime(int _elapsed) {
	if(items.size() > 0) {
		for(int i = 0; i < items.size(); i++) {
			if((items[0]->millis+_elapsed) <= items[i]->millis) return items[i]->joints;
		}
		return items[items.size()-1]->joints;
	}
	return NULL;
}

void skelJointsRecorder::startPlaying() {
	if(!recording) {
		playElapsed = 0;
		playing = true;
	}
}

void skelJointsRecorder::updatePlaying(int _mov, bool _loop) {
	if(!recording && playing) {
		playElapsed += _mov;
		if(playElapsed > getRecordLength()) {
			if(_loop) {
				playElapsed = 0;
			} else {
				playElapsed = getRecordLength();
			}
		}
	}
}

void skelJointsRecorder::stopPlaying() {
	if(!recording && playing) {
		playElapsed = 0;
		playing = false;
	}
}

//------------------------------------------------------------------------------------------------------
skelTracked::skelTracked(XnUserID _user, int _historySize) {
	user = _user;
	tracking = false;
	curJoints = new skelJoints();
	oldJoints = new skelJoints();
	history = new skelJointsHistory(_historySize);
}

skelTracked::~skelTracked() {
	delete(curJoints); curJoints = NULL;
	delete(oldJoints); oldJoints = NULL;
	delete(history); history = NULL;
}

void skelTracked::saveCoords() {
	for(int i = 1; i < PH_NUMBER_OF_JOINTS+1; i++) { // 1 based index
		oldJoints->scrJoints[i] = curJoints->scrJoints[i];
		oldJoints->wrdJoints[i] = curJoints->wrdJoints[i];
	}
}

bool skelTracked::compareCoords(int _now) {
	bool result = curJoints->compareJoints(oldJoints);
	if(!result) {
		lastUpdate = _now;
	};
	return result;
}

void skelTracked::setWorldJoint(int _index, float _x, float _y, float _z) {
	curJoints->setWorldJoint(_index, _x, _y, _z);
}

void skelTracked::setScreenJoint(int _index, float _x, float _y, float _z) {
	curJoints->setScreenJoint(_index, _x, _y, _z);
}

void skelTracked::updateJoints(skelJoints * _sk, int _millis) {
	if(_sk != NULL) {
		oldJoints->setJoints(curJoints);
		curJoints->setJoints(_sk);
		history->getNextCircularItem()->updateJoints(curJoints, _millis);
	}
}

bool skelTracked::isActive(int _now) {
	//return ((_now - lastUpdate) < 1000);
	return history->changedInLastNUpdates(10, curJoints);
}

//------------------------------------------------------------------------------------------------------
skelTrackedList::skelTrackedList() {
}

skelTrackedList::~skelTrackedList() {
	freeItems();
}

void skelTrackedList::freeItems() {
	for (vector<skelTracked*>::iterator i = items.begin(); i != items.end(); i++) {
		delete(*i);
	}
	items.clear();
}

skelTracked * skelTrackedList::addSkel(XnUserID _user, int _historySize) {
	skelTracked * result;
	result = findSkelByUser(_user);
	if(result == NULL) {
		result = new skelTracked(_user, _historySize);
		items.push_back(result);
	}
	return result;
}

skelTracked * skelTrackedList::findSkelByUser(XnUserID _user) {
    for (vector<skelTracked*>::iterator i = items.begin(); i != items.end(); i++) {
		if((*i)->user == _user) { return (*i); }
	}
	return NULL;
}

int skelTrackedList::countTracked() {
	int result = 0;
    for (vector<skelTracked*>::iterator i = items.begin(); i != items.end(); i++) {
		if((*i)->tracking) { result += 1; }
	}
	return result;
}

int skelTrackedList::countActive() {
	int result = 0;
	int _now = ofGetElapsedTimeMillis();
    for (vector<skelTracked*>::iterator i = items.begin(); i != items.end(); i++) {
		if((*i)->isActive(_now)) { result += 1; }
	}
	return result;
}

//**********************************************************************
//**********************************************************************
skelPoseCondition::skelPoseCondition() {
}

skelPoseCondition::~skelPoseCondition() {
}

void skelPoseCondition::print(string _msg) {
	string msg = _msg + ":" + name + "\n";
	printf(msg.c_str());
}

void skelPoseCondition::initialize() {
	name = "alwaysFalse";
}

bool skelPoseCondition::testCondition(skelTracked _sk) {
	return false;
}

//--------------------------------------------------
// mão esquerda acima da cabeça
//--------------------------------------------------
void skelPoseLeftHandOverHead::initialize() {
	name = "LeftHandOverHead";
}

bool skelPoseLeftHandOverHead::testCondition(skelTracked _sk) {
	return (_sk.curJoints->scrJoints[XN_SKEL_LEFT_HAND].y < _sk.curJoints->scrJoints[XN_SKEL_HEAD].y);
}

//--------------------------------------------------
// mão esquerda abaixo da cabeça
//--------------------------------------------------
void skelPoseLeftHandBelowHead::initialize() {
	name = "LeftHandBelowHead";
}

bool skelPoseLeftHandBelowHead::testCondition(skelTracked _sk) {
	return (_sk.curJoints->scrJoints[XN_SKEL_LEFT_HAND].y > _sk.curJoints->scrJoints[XN_SKEL_HEAD].y);
}

//--------------------------------------------------
// mão direita acima da cabeça
//--------------------------------------------------
void skelPoseRightHandOverHead::initialize() {
	name = "RightHandOverHead";
}

bool skelPoseRightHandOverHead::testCondition(skelTracked _sk) {
	return (_sk.curJoints->scrJoints[XN_SKEL_RIGHT_HAND].y < _sk.curJoints->scrJoints[XN_SKEL_HEAD].y);
}

//--------------------------------------------------
// mão direita acima da cabeça
//--------------------------------------------------
void skelPoseRightHandBelowHead::initialize() {
	name = "RightHandBelowHead";
}

bool skelPoseRightHandBelowHead::testCondition(skelTracked _sk) {
	return (_sk.curJoints->scrJoints[XN_SKEL_RIGHT_HAND].y > _sk.curJoints->scrJoints[XN_SKEL_HEAD].y);
}

//--------------------------------------------------
// ombro esquerdo à esquerda do ombro direito = está de frente para a câmera
//--------------------------------------------------
void skelPoseDeFrente::initialize() {
	name = "De_Frente";
}

bool skelPoseDeFrente::testCondition(skelTracked _sk) {
	return (_sk.curJoints->scrJoints[XN_SKEL_LEFT_SHOULDER].x < _sk.curJoints->scrJoints[XN_SKEL_RIGHT_SHOULDER].x);
}

//--------------------------------------------------
// ombro direito à esquerda do ombro esquerda = está de costas para a câmera
//--------------------------------------------------
void skelPoseDeCostas::initialize() {
	name = "De_Costas";
}

bool skelPoseDeCostas::testCondition(skelTracked _sk) {
	return (_sk.curJoints->scrJoints[XN_SKEL_LEFT_SHOULDER].x > _sk.curJoints->scrJoints[XN_SKEL_RIGHT_SHOULDER].x);
}

//--------------------------------------------------
// mao esquerda entre os cotovelos
//--------------------------------------------------
void skelPoseLeftHandInsideElbows::initialize() {
	name = "LeftHandInsideElbows";
}

bool skelPoseLeftHandInsideElbows::testCondition(skelTracked _sk) {
	return ((_sk.curJoints->scrJoints[XN_SKEL_LEFT_HAND].x > _sk.curJoints->scrJoints[XN_SKEL_LEFT_ELBOW].x) &&
		    (_sk.curJoints->scrJoints[XN_SKEL_LEFT_HAND].x < _sk.curJoints->scrJoints[XN_SKEL_RIGHT_ELBOW].x));
}

//--------------------------------------------------
// mao esquerda fora dos cotovelos
//--------------------------------------------------
void skelPoseLeftHandOutsideElbows::initialize() {
	name = "LeftHandOutsideElbows";
}

bool skelPoseLeftHandOutsideElbows::testCondition(skelTracked _sk) {
	return !((_sk.curJoints->scrJoints[XN_SKEL_LEFT_HAND].x > _sk.curJoints->scrJoints[XN_SKEL_LEFT_ELBOW].x) &&
		     (_sk.curJoints->scrJoints[XN_SKEL_LEFT_HAND].x < _sk.curJoints->scrJoints[XN_SKEL_RIGHT_ELBOW].x));
}

//--------------------------------------------------
// mao direita entre os cotovelos
//--------------------------------------------------
void skelPoseRightHandInsideElbows::initialize() {
	name = "RightHandInsideElbows";
}

bool skelPoseRightHandInsideElbows::testCondition(skelTracked _sk) {
	return ((_sk.curJoints->scrJoints[XN_SKEL_RIGHT_HAND].x > _sk.curJoints->scrJoints[XN_SKEL_LEFT_ELBOW].x) &&
		    (_sk.curJoints->scrJoints[XN_SKEL_RIGHT_HAND].x < _sk.curJoints->scrJoints[XN_SKEL_RIGHT_ELBOW].x));
}

//--------------------------------------------------
// mao direita fora os cotovelos
//--------------------------------------------------
void skelPoseRightHandOutsideElbows::initialize() {
	name = "RightHandOutsideElbows";
}

bool skelPoseRightHandOutsideElbows::testCondition(skelTracked _sk) {
	return !((_sk.curJoints->scrJoints[XN_SKEL_RIGHT_HAND].x > _sk.curJoints->scrJoints[XN_SKEL_LEFT_ELBOW].x) &&
		     (_sk.curJoints->scrJoints[XN_SKEL_RIGHT_HAND].x < _sk.curJoints->scrJoints[XN_SKEL_RIGHT_ELBOW].x));
}

//--------------------------------------------------
// mão esquerda dentro do poligono do torax
//--------------------------------------------------
void skelPoseMaoEsquerdaNoTorax::initialize() {
	name = "Mao_esquerda_no_torax";
}

bool skelPoseMaoEsquerdaNoTorax::myPolygonHitTest(int x, int y, vector<ofVec2f> pnts) {
    bool isInside = false;
    int i;
    ofVec2f pNew, pOld, p1, p2;

    if (pnts.size() < 3) { return(isInside); }

    pOld = pnts[pnts.size()-1];

    for (i = 0; i < pnts.size(); i++) {
        pNew = pnts[i];
        if (pNew.x > pOld.x) {
            p1 = pOld;
            p2 = pNew;
        } else {
            p1 = pNew;
            p2 = pOld;
        }

        if (((pNew.x < x) == (x <= pOld.x)) && (((y-p1.y)*(p2.x-p1.x)) < ((p2.y-p1.y)*(x-p1.x)))) {
            isInside = !isInside;
        }
    }
    return (isInside);
}

bool skelPoseMaoEsquerdaNoTorax::testCondition(skelTracked _sk) {
	vector<ofVec2f> torax;
	torax.push_back(ofVec2f(_sk.curJoints->scrJoints[XN_SKEL_LEFT_SHOULDER].x,_sk.curJoints->scrJoints[XN_SKEL_LEFT_SHOULDER].y));
	torax.push_back(ofVec2f(_sk.curJoints->scrJoints[XN_SKEL_RIGHT_SHOULDER].x,_sk.curJoints->scrJoints[XN_SKEL_RIGHT_SHOULDER].y));
	torax.push_back(ofVec2f(_sk.curJoints->scrJoints[XN_SKEL_RIGHT_HIP].x,_sk.curJoints->scrJoints[XN_SKEL_RIGHT_HIP].y));
	torax.push_back(ofVec2f(_sk.curJoints->scrJoints[XN_SKEL_LEFT_HIP].x,_sk.curJoints->scrJoints[XN_SKEL_LEFT_HIP].y));
	return myPolygonHitTest(_sk.curJoints->scrJoints[XN_SKEL_LEFT_HAND].x, _sk.curJoints->scrJoints[XN_SKEL_LEFT_HAND].y, torax);
}

//**********************************************************************
skelPose::skelPose(string _name, int _id) {
	name = _name;
	id = _id;
}

skelPose::~skelPose() {
    for (vector<skelPoseCondition*>::iterator i = conditions.begin(); i != conditions.end(); i++) {
		delete(*i);
	}
	conditions.clear();
}

void skelPose::addCondition(skelPoseCondition * _pc) {
	_pc->initialize();
	conditions.push_back(_pc);
	string msg = _pc->name + ":";
	printf(msg.c_str());
	printf("\n");
}

bool skelPose::testPose(skelTracked _sk) {
	for(int i = 0; i < conditions.size(); i++) {
		if(!conditions[i]->testCondition(_sk)) {
			return false;
		}
	}
	return (conditions.size() > 0); // se não tiver nenhuma condição retorna falso
}

void skelPose::setName(string _name) {
	name = _name;
}

void skelPose::print(string _msg) {
	string msg = _msg + "==" + name + "==" + ofToString(id) + "==";
	for(int i = 0; i < conditions.size(); i++) {
		msg += conditions[i]->name + "|";
	}
	msg += "\n";
	printf(msg.c_str());
}

//**********************************************************************
skelPoseList::skelPoseList() {
}

skelPoseList::~skelPoseList() {
    for (vector<skelPose*>::iterator i = items.begin(); i != items.end(); i++) {
		delete(*i);
	}
	items.clear();
}

skelPose * skelPoseList::addPose(string _name, int _id) {
	skelPose * result = new skelPose(_name, _id);
	items.push_back(result);
	return result;
}

int skelPoseList::findPoseByName(string _name) {
	int result = 0;
    for (vector<skelPose*>::iterator i = items.begin(); i != items.end(); i++) {
		if((*i)->name == _name) { return result; }
		result += 1;
	}
	return -1;
}

//**********************************************************************
skelPoseActionItem::skelPoseActionItem(skelPose * _pose, int _duration, actionFinishedEnum _expectedEnd) {
	pose = _pose;
	expectedEnd = _expectedEnd;
	duration = _duration;
}

skelPoseActionItem::~skelPoseActionItem() {
}

//**********************************************************************
skelPoseAction::skelPoseAction() {
}

skelPoseAction::~skelPoseAction() {
    for (vector<skelPoseActionItem*>::iterator i = items.begin(); i != items.end(); i++) {
		delete(*i);
	}
	items.clear();
}

skelPoseActionItem * skelPoseAction::addItem(skelPose* _pose, int _duration, actionFinishedEnum _expectedEnd) {
	if(_pose == NULL) return NULL;
	skelPoseActionItem * result = new skelPoseActionItem(_pose, _duration, _expectedEnd);
	items.push_back(result);
	return result;
}

void skelPoseAction::restart() {
	curPoseIndex = 0;
	curPoseObject = NULL;
	startTime = ofGetElapsedTimeMillis();
	poseAtual = -1; poseAnterior = -1;
	status = paNotStarted;
	comecamosContarTempoNaPose = false;
	elapsedTime = 0;
}

poseActionStatusEnum skelPoseAction::testAction(skelTracked _sk) {

	skelPoseActionItem * curPose;
		
	if(curPoseIndex >= items.size() || status == paFailed || status == paFinished) restart(); // preciso reiniciar

	curPose = items[curPoseIndex]; // pego a pose corrente
	curPoseObject = curPose->pose;
		
	if(curPose->pose->testPose(_sk)) { // está na pose esperada?
		poseAnterior = poseAtual;
		poseAtual = curPoseIndex;
		if(curPose->expectedEnd == afStayInPose) { // tem que ficar na pose durante um tempo para passar para a próxima
			if(poseAnterior == poseAtual) { // estamos repetindo a pose então descobre a quanto tempo
				elapsedTime = ofGetElapsedTimeMillis() - startTime;
				if(elapsedTime >= curPose->duration) curPoseIndex += 1; // ficou o tempo necessário então vai para a próxima
				ofDrawBitmapString("ESTAMOS REPETINDO A POSE : " + ofToString(curPoseIndex) + " : " + ofToString(elapsedTime) + ":" + actionStatusNames[status], 500, 650);

				// sinaliza que estamos contando pose repetida mas desejada
				comecamosContarTempoNaPose = true;
			} else { // é a primeira vez nesta pose então prepara para contar o tempo
				startTime = ofGetElapsedTimeMillis();
			}
		} else { // não precisa ficar um tempo então passa para a próxima
			curPoseIndex += 1;
			startTime = ofGetElapsedTimeMillis(); // mudou de pose e ainda não terminou então começa a contar o tempo
		}
		if(curPoseIndex == items.size()) { // atendeu a última pose? então finalizou OK
			status = paFinished;
			return status;
		} else {
			status = paRunning;
		}
		ofDrawBitmapString("ESTA NA POSE DESEJADA : " + ofToString(curPoseIndex) + " : " + ofToString(elapsedTime) + ":" + actionStatusNames[status], 500, 680);
	} else if(comecamosContarTempoNaPose) { // se começamos a contar um tempo e a pose mudou então falhou
		comecamosContarTempoNaPose = false;
		status = paFailed;
		// se estavamos contando um tempo na pose, mas mudamos de pose sem esperar o tempo necessario, então a ação falhou
		return status;
	} else if(curPoseIndex > 0) { // não está na pose esperada, mas já esteve em pelo menos uma
		elapsedTime = ofGetElapsedTimeMillis() - startTime;
		ofDrawBitmapString("NAO ESTA NA POSE ESPERADA : " + ofToString(curPoseIndex) + " : " + ofToString(elapsedTime) + ":" + actionStatusNames[status], 500, 680);
		if(elapsedTime >= curPose->duration) { // estourou o tempo de espera por uma pose
			status =  paFailed;
			return status;
		}
		status = paRunning;
	}

	return status;
}

void skelPoseAction::draw(float _x, float _y) {
	ofDrawBitmapString(name, _x, _y);
	float __y = _y + 20;
    for (vector<skelPoseActionItem*>::iterator i = items.begin(); i != items.end(); i++) {
		ofDrawBitmapString((*i)->pose->name, _x, _y);
		__y += 20;
	}
}

//--------------------------------------------------
//--------------------------------------------------
skelPoseActionList::skelPoseActionList() {
}

skelPoseActionList::~skelPoseActionList() {
    for (vector<skelPoseAction*>::iterator i = actions.begin(); i != actions.end(); i++) {
		delete(*i);
	}
	actions.clear();
}

int skelPoseActionList::findById(int _id) {
	for(int i = 0; i < actions.size(); i++) {
		if(_id == actions[i]->id) return i;
	}
	return -1;
}

int skelPoseActionList::findByName(string _name) {
	for(int i = 0; i < actions.size(); i++) {
		if(_name == actions[i]->name) return i;
	}
	return -1;
}

skelPoseAction * skelPoseActionList::addAction(string _name, int _id) {
	skelPoseAction * result;
	int i = findById(_id);
	if(i > -1) {
		result = actions[i];
	} else {
		i = findByName(_name);
		if(i > -1) {
			result = actions[i];
		} else {
			result = new skelPoseAction();
			result->name = _name;
			result->id = _id;
			actions.push_back(result);
		}
	}
		 
	return result;
}


