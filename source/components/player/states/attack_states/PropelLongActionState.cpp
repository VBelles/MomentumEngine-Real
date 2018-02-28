#include "mcv_platform.h"
#include "PropelLongActionState.h"
#include "components/comp_hitbox.h"

PropelLongActionState::PropelLongActionState(TCompPlayerModel * player)
	: AirborneActionState::AirborneActionState(player) {
}

void PropelLongActionState::update (float delta) {
	PowerStats* currentPowerStats = player->GetPowerStats();
	if (timer.elapsed() >= endingTime) {
		velocityVector->y = currentPowerStats->jumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;
		player->SetAttackState(TCompPlayerModel::ActionStates::Idle);
		player->SetMovementState(TCompPlayerModel::ActionStates::AirborneNormal);
		//pasar mensaje a la otra entidad
		CHandle playerHandle = CHandle(player).getOwner();
		CEntity* targetEntity = propelTarget;
		VEC3 propelVelocity = { 0, -currentPowerStats->jumpVelocityVector.y, 0 };
		targetEntity->sendMsg(TMsgPropelled{ playerHandle, propelVelocity });
	}
	else {
		*velocityVector = VEC3::Zero;
		deltaMovement = movingVelocity * delta;
	}
}

void PropelLongActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	endingTime = endingFrames * (1.f / 60);
	propelTarget = player->grabTarget;
	CEntity* targetEntity = propelTarget;
	TCompTransform* targetTransform = targetEntity->get<TCompTransform>();
	movingVelocity = targetTransform->getPosition() + VEC3::Up * 2.f - playerTransform->getPosition();
	movingVelocity /= endingTime;
	timer.reset();
	player->lockMovementState = true;
	player->lockWalk = true;
}

void PropelLongActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	player->lockMovementState = false;
	player->lockWalk = false;
}

void PropelLongActionState::SetMovementInput(VEC2 input) {
}

void PropelLongActionState::OnJumpHighButton() {
}

void PropelLongActionState::OnJumpLongButton() {
}

void PropelLongActionState::OnLanding() {
	player->SetMovementState(TCompPlayerModel::ActionStates::Run);
	player->SetAttackState(TCompPlayerModel::ActionStates::Idle);
}