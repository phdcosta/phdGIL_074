#pragma once

#include "ofMain.h"

//-----------------------------------------------------------------------------------------------------
enum phdCurveMode {
	EASE_IN_EASE_OUT = 0,
	EASE_IN, 
	EASE_OUT, 
	LINEAR,	
	LATE_LINEAR, 
	VERY_LATE_LINEAR, 
	BOUNCY,	// this needs work
	TANH, 
	SINH, 	
	EARLY_SQUARE, 
	SQUARE, 
	LATE_SQUARE, 		
	LATE_EASE_IN_EASE_OUT, 
	VERY_LATE_EASE_IN_EASE_OUT,
	QUADRATIC_EASE_IN,
	QUADRATIC_EASE_OUT,
	NUM_ANIM_CURVES //leave that on the last to see how many we have
};

static char * phdCurveModeNames[NUM_ANIM_CURVES] = {
	"EASE_IN_EASE_OUT", "EASE_IN", "EASE_OUT", "LINEAR", "LATE_LINEAR", "VERY_LATE_LINEAR", "BOUNCY", "TANH", "SINH", "EARLY_SQUARE",
	"SQUARE", "LATE_SQUARE", "LATE_EASE_IN_EASE_OUT", "VERY_LATE_EASE_IN_EASE_OUT", "QUADRATIC_EASE_IN", "QUADRATIC_EASE_OUT"
};

enum phdLoopMode { LOOP_NORMAL, LOOP_PINGPONG };
enum phdStartMode { START_FORWARD, START_BACKWARD };

//-----------------------------------------------------------------------------------------------------
double getValueInCurve(int _curve, double _pos);
phdCurveMode getCurveModeByName(string _name);
phdLoopMode getLoopModeByName(string _name);
phdStartMode getStartModeByName(string _name);

//-----------------------------------------------------------------------------------------------------
class phdEase {
private:
	double startTime;
	double duration;
	double executions;
	double velocity;

	double curTime;
	double curValue;
	double curExec;

	bool active;

	phdCurveMode curveMode;
	phdLoopMode loopMode;
	phdStartMode startMode;

	void drawCurve(float x, float y, float w, float h, int exec, double frac);

public:

	phdEase();
	~phdEase();

	void setup(double _startTime, double _duration, double _executions, double _velocity, phdCurveMode _curveMode, phdLoopMode _loopMode, phdStartMode _startMode);

	void setStartTime(double _startTime);
	double getStartTime();

	void setDuration(double _duration);
	double getDuration();

	void setExecutions(double _executions);
	double getExecutions();

	void setVelocity(double _velocity);
	double getVelocity();

	void setLoopMode(phdLoopMode _loopMode);
	phdLoopMode getLoopMode();

	void setCurveMode(phdCurveMode _curveMode);
	phdCurveMode getCurveMode();

	void setStartMode(phdStartMode _startMode);
	phdStartMode getStartMode();

	double getValue();
	double getCurTime();
	double getCurExec();

	bool getActive();

	void internalMoveTo(double _time);

	virtual void moveTo(double _time);
	virtual void moveBy(double _delta);
	virtual void draw(float x, float y, float w, float h);
	virtual string getClassName() { return "PHDANIMVALUE"; }
};
