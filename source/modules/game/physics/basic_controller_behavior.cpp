#include "mcv_platform.h"
#include "basic_controller_behavior.h"
#include "entity/common_msgs.h"
#include "components/platforms/comp_platform_rotative.h"

PxControllerBehaviorFlags BasicControllerBehavior::getBehaviorFlags(const PxShape & shape, const PxActor & actor) {
	CHandle colliderHandle;
	colliderHandle.fromVoidPtr(actor.userData);
	CEntity* entity = colliderHandle.getOwner();
	TCompPlatformRotative* platformRotative = entity->get<TCompPlatformRotative>();
	if (platformRotative) {
		return PxControllerBehaviorFlag::eCCT_SLIDE;
	}
	return PxControllerBehaviorFlag::eCCT_USER_DEFINED_RIDE;
}

PxControllerBehaviorFlags BasicControllerBehavior::getBehaviorFlags(const PxController & controller) {
	return PxControllerBehaviorFlag::eCCT_SLIDE;
}

PxControllerBehaviorFlags BasicControllerBehavior::getBehaviorFlags(const PxObstacle & obstacle) {
	return PxControllerBehaviorFlags();
}
