#include "mcv_platform.h"
#include "WallJumpSquatPlummetActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"

WallJumpSquatPlummetActionState::WallJumpSquatPlummetActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, WallJumpSquatPlummet) {
}

void WallJumpSquatPlummetActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	if (!stateManager->isChangingBaseState) {
		if (timer.elapsed() >= endingTime) {
			//saltar
			*velocityVector = getPlayerTransform()->getFront() * frontVelocity;
			velocityVector->y = verticalVelocity;

			deltaMovement = *velocityVector * delta;
			getSkeleton()->executeAction(animation, 0.2f, 0.2f);
			stateManager->changeState(WallJumpPlummet);
		}
	}
}

void WallJumpSquatPlummetActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	getPlayerModel()->setGravityMultiplier(0.f);
	getPlayerModel()->maxVerticalSpeed = abs(verticalVelocity);
	timer.reset();
}

void WallJumpSquatPlummetActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}
