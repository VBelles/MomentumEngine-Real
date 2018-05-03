#include "mcv_platform.h"
#include "SlideActionState.h"
#include "components/player/comp_player_model.h"
#include "skeleton/comp_skeleton.h"

void SlideActionState::update(float delta) {
	AirborneActionState::update(delta);
}

void SlideActionState::onStateEnter(IActionState* lastState) {
	AirborneActionState::onStateEnter(lastState);
	getPlayerModel()->getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
}

void SlideActionState::onStateExit(IActionState* nextState) {
	AirborneActionState::onStateExit(nextState);
}

void SlideActionState::onMove(HitState& hitState) {
	if (!hitState.isGrounded) { //Not grounded, change to airborne normal
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	else { //Grounded, check slope
		float dot = hitState.hit.worldNormal.dot(PxVec3(0, 1, 0));
		if (dot >= getPlayerModel()->getController()->getSlopeLimit()) {
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Landing);
		}
	}
}

