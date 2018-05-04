#include "mcv_platform.h"
#include "basic_controller_behavior.h"
#include "entity/common_msgs.h"
#include "components/platforms/comp_platform_rotative.h"
#include "components/platforms/comp_platform_simple.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/base_states/SlideActionState.h"

PxControllerBehaviorFlags BasicControllerBehavior::getBehaviorFlags(const PxShape & shape, const PxActor & actor) {
	CEntity* player = getEntityByName(PLAYER_NAME);
	TCompPlayerModel* playerModel = player->get<TCompPlayerModel>();
	if (dynamic_cast<SlideActionState*>(playerModel->baseState)) {
		return PxControllerBehaviorFlag::eCCT_SLIDE | PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
	}
	return PxControllerBehaviorFlag::eCCT_CAN_RIDE_ON_OBJECT;
}

PxControllerBehaviorFlags BasicControllerBehavior::getBehaviorFlags(const PxController & controller) {
	return PxControllerBehaviorFlag::eCCT_SLIDE;
}

PxControllerBehaviorFlags BasicControllerBehavior::getBehaviorFlags(const PxObstacle & obstacle) {
	return PxControllerBehaviorFlags();
}
