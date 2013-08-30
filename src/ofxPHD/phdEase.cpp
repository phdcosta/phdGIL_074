#include "phdEase.h"

//-----------------------------------------------------------------------------------------------------
double getValueInCurve(int _curve, double _pos) {

	double r = _pos;

	switch ( _curve ) {
		case EASE_IN_EASE_OUT:
			r = 0.5f - 0.5f * cosf( PI * _pos ); break;
		case EASE_IN:
			r = 1.0f + sinf( HALF_PI * _pos - HALF_PI); break;
		case EASE_OUT:	
			r = sinf( HALF_PI * _pos ); break;
		case LINEAR: break;
		case LATE_LINEAR:
			r = ( _pos < 0.5 ) ? 0.0f : 2.0f * _pos - 1; break;
		case VERY_LATE_LINEAR:
			r = ( _pos < 0.75 ) ? 0.0f : 4.0f * _pos - 3; break;
		case TANH:
			r = 0.5f + 0.5f * tanh( 2.0f * PI * _pos - PI ) * 1.00374187319732; break;
		case SINH:
			r = 0.5f + 0.23482f * sinh( 3.0f * _pos - 1.5f ); break;
		case SQUARE:
			r = (_pos < 0.5f) ? 0.0f : 1.0f; break;
		case LATE_SQUARE:
			r = (_pos < 0.75f) ? 0.0f : 1.0f; break;
		case EARLY_SQUARE:
			r = (_pos < 0.25f) ? 0.0f : 1.0f; break;
		case LATE_EASE_IN_EASE_OUT:
			r = (_pos < 0.5f) ? 0.0f : 0.5f + 0.5f * cosf( 2.0f * PI * _pos); break;
		case VERY_LATE_EASE_IN_EASE_OUT:
			r = (_pos < 0.75f) ? 0.0f : 0.5f + 0.5f * cosf( 4.0f * PI * _pos); break;
		case QUADRATIC_EASE_IN:
			r = _pos * _pos; break;
		case QUADRATIC_EASE_OUT:
			r = 1.0f - (_pos - 1.0f) * (_pos - 1.0f); break;
		case BOUNCY:{
			float k = 0.5;
			r = 0.5f - 0.51f * cosf( PI * _pos + k * _pos - k * 0.5f ); break;
		}
	}

	return r;
}

phdCurveMode getCurveModeByName(string _name) {
	for(int i = 0; i < NUM_ANIM_CURVES-1; i++) {
		if(phdCurveModeNames[i] == _name) return (phdCurveMode)i;
	}
	return LINEAR;
}

phdLoopMode getLoopModeByName(string _name) {
	if(_name == "LOOP_PINGPONG") return LOOP_PINGPONG;
	return LOOP_NORMAL;
}

phdStartMode getStartModeByName(string _name) {
	if(_name == "START_BACKWARD") return START_BACKWARD;
	return START_FORWARD;
}

// protected
//-----------------------------------------------------------------------------------------------------
void phdEase::drawCurve(float x, float y, float w, float h, int exec, double frac) {

	bool _movingPing = (startMode == START_FORWARD);

	if((int)exec % 2 == 1 && loopMode == LOOP_PINGPONG) {
		_movingPing = !(startMode == START_FORWARD);
	}

	glBegin(GL_LINES);
		int i = 0;
		while(i < 9 && frac > (i+1)*0.1) {
			if(!_movingPing) {
				glVertex2f(x + (i*0.1) * w, y + h - getValueInCurve(curveMode, 1.0-i*0.1) * h);
				glVertex2f(x + ((i+1)*0.1) * w, y + h - getValueInCurve(curveMode, 1.0-(i+1)*0.1) * h);
			} else {
				glVertex2f(x + (i*0.1) * w, y + h - getValueInCurve(curveMode, i*0.1) * h);
				glVertex2f(x + ((i+1)*0.1) * w, y + h - getValueInCurve(curveMode, (i+1)*0.1) * h);
			}
			i += 1;
		}
		if(!_movingPing) {
			glVertex2f(x + (i*0.1) * w, y + h - getValueInCurve(curveMode, 1.0-i*0.1) * h);
			glVertex2f(x + frac * w, y + h - getValueInCurve(curveMode, 1.0-frac) * h);
		} else {
			glVertex2f(x + (i*0.1) * w, y + h - getValueInCurve(curveMode, i*0.1) * h);
			glVertex2f(x + frac * w, y + h - getValueInCurve(curveMode, frac) * h);
		}
	glEnd();
}

// public
//-----------------------------------------------------------------------------------------------------
phdEase::phdEase() {
	curExec = 0.0;
	startMode = START_FORWARD;
	active = false;
}

phdEase::~phdEase() {
}

void phdEase::setup(double _startTime, double _duration, double _executions, double _velocity, phdCurveMode _curveMode, phdLoopMode _loopMode, phdStartMode _startMode) {
	setStartTime(_startTime);
	setDuration(_duration);
	setExecutions(_executions);
	setVelocity(_velocity);
	setCurveMode(_curveMode);
	setLoopMode(_loopMode);
	setStartMode(_startMode);
	moveTo(0.0);
}

void phdEase::setStartTime(double _startTime) {
	startTime = _startTime;
}

double phdEase::getStartTime() {
	return startTime;
}

void phdEase::setDuration(double _duration) {
	duration = _duration;
	if(duration < 0.001) duration = 0.001;
} // minimum one ms

double phdEase::getDuration() {
	return duration;
}

void phdEase::setExecutions(double _executions) {
	executions = fabs(_executions);
}

double phdEase::getExecutions() {
	return executions;
}

void phdEase::setVelocity(double _velocity) {
	velocity = fabs(_velocity);
}

double phdEase::getVelocity() {
	return velocity;
}

void phdEase::setLoopMode(phdLoopMode _loopMode) {
	loopMode = _loopMode;
}

phdLoopMode phdEase::getLoopMode() {
	return loopMode;
}

void phdEase::setCurveMode(phdCurveMode _curveMode) {
	curveMode = _curveMode;
};

phdCurveMode phdEase::getCurveMode() {
	return curveMode;
}

void phdEase::setStartMode(phdStartMode _startMode) {
	startMode = _startMode;
}

phdStartMode phdEase::getStartMode() {
	return startMode;
}

double phdEase::getValue() {
	return curValue;
}

double phdEase::getCurTime() {
	return curTime;
}

double phdEase::getCurExec() {
	return curExec;
}

bool phdEase::getActive() {
	return active;
}

void phdEase::internalMoveTo(double _time) {

	if(curTime == _time) return; // nothing to be done

	curTime = _time;

	// uses executions == 0.0 for infinity loop of an animation
	double _executions = executions;
	if(executions == 0.0) _executions = 1.0;

	if(curTime < startTime) {
		active = false;
		curExec = 0.0;
	} else if(curTime > startTime + duration * _executions) {
		if(executions == 0.0) {
			active = true;
			curExec = (curTime - startTime) / duration;
		} else {
			active = false;
			curExec = _executions;
		}
	} else {
		active = true;
		curExec = (curTime - startTime) / duration;
	}

	bool _movingPing = (startMode == START_FORWARD);

	if(loopMode == LOOP_PINGPONG) {
		if((int)curExec % 2 == 1) {
			_movingPing = !(startMode == START_FORWARD);
		}
	}

	double _aux = curExec - (int)curExec;

	if(_aux == 0.0 && curTime > (startTime + duration * curExec)) {
		_aux = executions - (int)executions; // over end keep frac part of executions
		if(_aux == 0.0) _aux = 1.0;
	}

	if(_movingPing) {
		curValue = getValueInCurve(curveMode, _aux);
	} else {
		curValue = getValueInCurve(curveMode, 1.0 - _aux);
	}
}

void phdEase::moveTo(double _time) {
	internalMoveTo(_time);
}

void phdEase::moveBy(double _delta) {
	internalMoveTo(curTime + _delta * velocity);
}

void phdEase::draw(float x, float y, float w, float h) {
	ofFill();
	if(getActive()) {
		ofSetColor(128,192,64,92);
	} else {
		ofSetColor(220,192,64,92);
	}
	if(executions != 0.0) {
		for(int i = 0; i < (int)executions; i++) { ofRect(x + i * w, y, w, h); }
		ofRect(x + w * (int)executions, y, w * (executions - (int)executions), h);
		ofNoFill();
		ofSetColor(255,255,255,255);
		for(int i = 0; i < (int)executions; i++) { ofRect(x + i * w, y, w, h); drawCurve(x + i * w, y, w, h, i, 1.0); }
		ofRect(x + w * (int)executions, y, w * (executions-(int)executions), h);
		drawCurve(x + w * (int)executions, y, w, h, (int)executions, (executions-(int)executions));
		ofFill();
		ofCircle(x + getCurExec() * w, y + h - h * curValue, 3);
	} else {
		ofRect(x, y, w, h);
		ofNoFill();
		ofSetColor(255,255,255,255);
		ofRect(x, y, w, h);
		bool _movingPing = (startMode == START_FORWARD);
		drawCurve(x, y, w, h, _movingPing ? 0 : 1, 1.0);
		if((int)curExec % 2 == 1 && loopMode == LOOP_PINGPONG) { _movingPing = !(startMode == START_FORWARD); }
		double _aux = (curExec-(int)curExec);
		if(!_movingPing) {_aux = 1.0 - _aux; }
		ofFill();
		ofCircle(x + _aux * w, y + h - h * curValue, 3);
	}
	ofSetColor(255,255,255,255);
	ofDrawBitmapString(ofToString(getValue(),3), x, y+10);
}

string phdEase::getClassName() {
	return "PHDANIMVALUE";
}
