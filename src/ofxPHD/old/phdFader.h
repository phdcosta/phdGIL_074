#pragma once

#include "ofMain.h"

//--------------------------------------------------------------------------------------------------------------
enum phdFaderActionType { pfaNone, pfaFadeIn, pfaFadeOut };

//--------------------------------------------------------------------------------------------------------------
class phdFader {
private:
	float value;
	float duration;

	float startTime;
	float endTime;

	bool bRecalculate;

	phdFaderActionType action;

public:

	phdFader();

	float getValue();
	inline float getDuration();
	inline void setDuration(float _duration);
	inline void setValue(float _value);

	void fadeIn(bool _restart = true);
	void fadeOut(bool _restart = false);
	void pause();

	void update();
};