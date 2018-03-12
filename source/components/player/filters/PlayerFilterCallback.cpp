#include "mcv_platform.h"
#include "PlayerFilterCallback.h"

PlayerFilterCallback::PlayerFilterCallback(CHandle playerModelHandle) {
	this->playerModelHandle = playerModelHandle;
}


bool PlayerFilterCallback::filter(const PxController& a, const PxController& b) {
	
	return true;
}