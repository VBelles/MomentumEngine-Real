#include "mcv_platform.h"
#include "controller_behavior_callback.h"
#include "entity/common_msgs.h"

PxControllerBehaviorFlags GameControllerBehaviorCallback::getBehaviorFlags(const PxShape & shape, const PxActor & actor) {
	return PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
}

PxControllerBehaviorFlags GameControllerBehaviorCallback::getBehaviorFlags(const PxController & controller) {
	return PxControllerBehaviorFlag::eCCT_SLIDE;
}

PxControllerBehaviorFlags GameControllerBehaviorCallback::getBehaviorFlags(const PxObstacle & obstacle) {
	return PxControllerBehaviorFlags();
}
