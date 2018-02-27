#include "mcv_platform.h"
#include "PropelHighActionState.h"
#include "components/comp_hitbox.h"

PropelHighActionState::PropelHighActionState(TCompPlayerModel * player)
	: AirborneActionState::AirborneActionState(player) {
}

void PropelHighActionState::update (float delta) {
	PowerStats* currentPowerStats = player->GetPowerStats();
	if (timer.elapsed() >= endingTime) {
		velocityVector->y = currentPowerStats->jumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;
		player->SetAttackState(TCompPlayerModel::ActionStates::Idle);
		player->SetMovementState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	else {
		*velocityVector = VEC3::Zero;
		deltaMovement = movingVelocity * delta;
	}
}

void PropelHighActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	endingTime = endingFrames * (1.f / 60);
	movingVelocity = player->grabTarget - playerTransform->getPosition();
	movingVelocity /= endingTime;
	timer.reset();
	player->lockMovementState = true;
	player->lockWalk = true;
}

void PropelHighActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	player->lockMovementState = false;
	player->lockWalk = false;
}

void PropelHighActionState::SetMovementInput(VEC2 input) {
}

void PropelHighActionState::OnJumpHighButton() {
}

void PropelHighActionState::OnJumpLongButton() {
}

void PropelHighActionState::OnLanding() {
	player->SetMovementState(TCompPlayerModel::ActionStates::Run);
	player->SetAttackState(TCompPlayerModel::ActionStates::Idle);
}