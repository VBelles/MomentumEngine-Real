#include "mcv_platform.h"
#include "WallJumpSquatPlummetActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"

WallJumpSquatPlummetActionState::WallJumpSquatPlummetActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
	animation = "walk";
}

void WallJumpSquatPlummetActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	if (!isChangingBaseState) {
		if (timer.elapsed() >= endingTime) {
			//saltar
			*velocityVector = getPlayerTransform()->getFront() * frontVelocity;
			velocityVector->y = verticalVelocity;
		
			deltaMovement = *velocityVector * delta;
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::WallJumpPlummet);
		}
	}
}

void WallJumpSquatPlummetActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	getPlayerModel()->setGravityMultiplier(0.f);
	getPlayerModel()->maxVerticalSpeed = abs(verticalVelocity);
	setPose();
	endingTime = endingFrames * (1.f / 60);
	timer.reset();
	getPlayerModel()->getSkeleton()->executeAction(animation);
}

void WallJumpSquatPlummetActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}

void WallJumpSquatPlummetActionState::setPose() {
	getRender()->setMesh("data/meshes/pose_propel.mesh");
}
