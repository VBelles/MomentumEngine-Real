#include "mcv_platform.h"
#include "timer_frames.h"

CTimerFrames* CTimerFrames::instance = nullptr;

CTimerFrames::CTimerFrames() {
	if (!instance) {
		instance = this;
	}
}

CTimerFrames::~CTimerFrames() {
	safeDelete(instance);
}

CTimerFrames& CTimerFrames::get() {
	if (!instance) {
		instance = new CTimerFrames();
	}
	return *instance;
}

void CTimerFrames::update(float delta) {
	if (!paused) {
		time += delta;
		frames++;
	}
}

float CTimerFrames::getTime() const {
	return time;
}

int CTimerFrames::getFrames() const {
	return frames;
}

void CTimerFrames::reset() {
	time = 0;
	frames = 0;
}

void CTimerFrames::setPaused(bool paused) {
	this->paused = paused;
}