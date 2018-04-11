#include "mcv_platform.h"
#include "basic_controller_behavior.h"
#include "entity/common_msgs.h"

PxControllerBehaviorFlags BasicControllerBehavior::getBehaviorFlags(const PxShape & shape, const PxActor & actor) {
	return PxControllerBehaviorFlag::eCCT_USER_DEFINED_RIDE;
}

PxControllerBehaviorFlags BasicControllerBehavior::getBehaviorFlags(const PxController & controller) {
	return PxControllerBehaviorFlag::eCCT_SLIDE;
}

PxControllerBehaviorFlags BasicControllerBehavior::getBehaviorFlags(const PxObstacle & obstacle) {
	return PxControllerBehaviorFlags();
}
