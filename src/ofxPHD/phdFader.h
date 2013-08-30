#pragma once

#include "ofMain.h"

//--------------------------------------------------------------------------------------------------------------
enum phdFaderActionType { pfaNone, pfaFadeIn, pfaFadeOut };

//--------------------------------------------------------------------------------------------------------------
class phdFader {
protected:
	float value;
	float duration;

	float startTime;
	float endTime;

	bool bRecalculate;

	phdFaderActionType action;

public:

	phdFader();

	inline phdFaderActionType getAction() { return action; }
	inline float getDuration() { return duration; }
	inline void setDuration(float _duration) { duration = _duration; }
	inline void setValue(float _value) { value = _value; }
	inline float getValue() { return value; }

	void fadeIn(bool _restart = true);
	void fadeOut(bool _restart = false);
	void pause();

	void update();
};