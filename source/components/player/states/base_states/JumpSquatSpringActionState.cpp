#include "mcv_platform.h"
#include "JumpSquatSpringActionState.h"

JumpSquatSpringActionState::JumpSquatSpringActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void JumpSquatSpringActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = GetPlayerModel()->getPowerStats();
	if (timer.elapsed() >= squatTime) {
		//saltar
		GetPlayerModel()->isAttachedToPlatform = false;
		velocityVector->y = currentPowerStats->springJumpVelocity;
		//Dejamos que el cambio de estado se haga cuando lo detecte ground sensor
		deltaMovement = *velocityVector * delta;
	}
}

void JumpSquatSpringActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	SetPose();
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
	GetPlayerModel()->maxVerticalSpeed = GetPlayerModel()->maxVelocityUpwards;
}

void JumpSquatSpringActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
}

void JumpSquatSpringActionState::OnLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpSquat);
	}
}
