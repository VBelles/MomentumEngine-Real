#pragma once

#include "utils/timer_frames.h"

class CTimer2 {
	float time_stamp = 0;
	int frame_stamp = 0;
public:
	CTimer2() {
		reset();
	}

	float getTimeStamp() const {
		float now;
		now = CTimerFrames::get().getTime();
		return now;
	}

	int getFrameStamp() const {
		int now;
		now = CTimerFrames::get().getFrames();
		return now;
	}

	float elapsed() {
		float now = getTimeStamp();
		float delta = now - time_stamp;
		return delta;
	}

	void setElapsed(float time) {
		time_stamp = getTimeStamp() - time;
	}

	int elapsedFrames() {
		int now = getFrameStamp();
		int delta = now - frame_stamp;
		return delta;
	}

	void reset() {
		time_stamp = getTimeStamp();
		frame_stamp = getFrameStamp();
	}

	float elapsedAndReset() {
		float delta = elapsed();
		reset();
		return delta;
	}

	int elapsedFramesAndReset() {
		int delta = elapsedFrames();
		reset();
		return delta;
	}
	
	void setTimeStamp(float value) {
		time_stamp = value;
	}

	void setFrameStamp(int value) {
		frame_stamp = value;
	}
};
