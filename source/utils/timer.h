#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include <Windows.h>

class CTimer {
	uint64_t time_stamp;
public:
	CTimer() {
		reset();
	}

	// Ticks!
	uint64_t getTimeStamp() const {
		uint64_t now;
		QueryPerformanceCounter((LARGE_INTEGER*)&now);
		return now;
	}

	float elapsed() {
		uint64_t now = getTimeStamp();
		uint64_t delta_ticks = now - time_stamp;

		LARGE_INTEGER freq;
		if (QueryPerformanceFrequency(&freq)) {
			float delta_secs = (float)(delta_ticks) / (float)freq.QuadPart;
			return delta_secs;
		}
		fatal("QueryPerformanceFrequency returned false!!!\n");
		return 0.f;
	}

	void setElapsed(float time) {
		LARGE_INTEGER freq;
		uint64_t delta_ticks;
		if (QueryPerformanceFrequency(&freq)) {
			delta_ticks = time * (float)freq.QuadPart;
		}
		else {
			fatal("QueryPerformanceFrequency returned false!!!\n");
		}
		time_stamp = getTimeStamp() - delta_ticks;
	}

	// Reset counter to current timestamp
	void reset() {
		time_stamp = getTimeStamp();
	}

	float elapsedAndReset() {
		float delta = elapsed();
		reset();
		return delta;
	}
	
	void setTimeStamp(uint64_t value) {
		time_stamp = value;
	}
};

#endif
