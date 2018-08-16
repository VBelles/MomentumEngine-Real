#include "mcv_platform.h"
#include "basic_controller_behavior.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/StateManager.h"
#include "components/player/states/IActionState.h"
#include "components/platforms/comp_platform_simple.h"


BasicControllerBehavior::BasicControllerBehavior(CHandle entityHandle) : entityHandle(entityHandle) {
}

PxControllerBehaviorFlags BasicControllerBehavior::getBehaviorFlags(const PxShape& shape, const PxActor& actor) {
	CEntity* collidedEntity = getEntity(&actor);
	CEntity* entity = entityHandle;
	CHandle playerModelHandle = entity->get<TCompPlayerModel>();
	if (playerModelHandle.isValid()) { // Is a player
		TCompPlayerModel* playerModel = playerModelHandle;
		if (playerModel->getStateManager()->getState()->state == Slide) {
			return PxControllerBehaviorFlag::eCCT_SLIDE;
		}
		CHandle platformHandle = collidedEntity->get<TCompPlatformSimple>();
		if (platformHandle.isValid()) { // Collides against a TCompPlatformSimple
			TCompPlatformSimple* platform = platformHandle;
			if (platform->isRolling()) {
				playerModel->disableClimbing();
				return PxControllerBehaviorFlag::eCCT_SLIDE;
			}
		}

	}


	return PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
}

PxControllerBehaviorFlags BasicControllerBehavior::getBehaviorFlags(const PxController& controller) {
	return PxControllerBehaviorFlag::eCCT_SLIDE;
}

PxControllerBehaviorFlags BasicControllerBehavior::getBehaviorFlags(const PxObstacle& obstacle) {
	return PxControllerBehaviorFlags();
}

