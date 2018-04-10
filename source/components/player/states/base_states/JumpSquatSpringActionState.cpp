#include "mcv_platform.h"
#include "JumpSquatSpringActionState.h"

JumpSquatSpringActionState::JumpSquatSpringActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void JumpSquatSpringActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	if (timer.elapsed() >= squatTime) {
		//saltar
		getPlayerModel()->isAttachedToPlatform = false;
		velocityVector->y = currentPowerStats->springJumpVelocity;
		//Dejamos que el cambio de estado se haga cuando lo detecte ground sensor
		deltaMovement = *velocityVector * delta;
	}
}

void JumpSquatSpringActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	setPose();
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
	getPlayerModel()->maxVerticalSpeed = getPlayerModel()->maxVelocityUpwards;
}

void JumpSquatSpringActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
}

void JumpSquatSpringActionState::onLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpSquat);
	}
}
