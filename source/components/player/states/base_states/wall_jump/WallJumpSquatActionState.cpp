#include "mcv_platform.h"
#include "WallJumpSquatActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"

WallJumpSquatActionState::WallJumpSquatActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void WallJumpSquatActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	//deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	
	if (timer.elapsed() >= endingTime) {
		//saltar
		*velocityVector = getPlayerTransform()->getFront() * currentPowerStats->wallJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->wallJumpVelocityVector.y;
		
		deltaMovement = *velocityVector * delta;
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneWallJump);
	}
}

void WallJumpSquatActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	setPose();
	endingTime = endingFrames * (1.f / 60);
	timer.reset();
}

void WallJumpSquatActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}

void WallJumpSquatActionState::setPose() {
	getRender()->setMesh("data/meshes/pose_propel.mesh");
}
