#include "mcv_platform.h"
#include "GhostJumpWindowActionState.h"

GhostJumpWindowActionState::GhostJumpWindowActionState(TCompPlayerModel * player)
	: AirborneActionState::AirborneActionState(player) {
}


void GhostJumpWindowActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	//dbg("Entrando en GhostJumpWindow\n");
	squatTime = frameWindow * (1.f / 60);
	timer.reset();
	enteringVelocity = player->GetVelocityVector()->Length();
}

void GhostJumpWindowActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	//dbg("Saliendo de GhostJumpWindow\n");
}

void GhostJumpWindowActionState::update (float delta) {
	if (timer.elapsed() >= squatTime) {
		//Como estamos ya en el aire, hacemos el cambio nosotros mismos
		player->SetMovementState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	AirborneActionState::update(delta);
}

void GhostJumpWindowActionState::SetMovementInput(VEC2 input) {
	AirborneActionState::SetMovementInput(input);
}

void GhostJumpWindowActionState::OnJumpHighButton() {
	PowerStats* currentPowerStats = player->GetPowerStats();
	velocityVector->y = 0.f;
	*velocityVector += currentPowerStats->jumpVelocityVector;
	//Como estamos ya en el aire, hacemos el cambio nosotros mismos
	player->SetMovementState(TCompPlayerModel::ActionStates::AirborneNormal);
}

void GhostJumpWindowActionState::OnJumpLongButton() {
	PowerStats* currentPowerStats = player->GetPowerStats();
	*velocityVector = playerTransform->getFront() * currentPowerStats->longJumpVelocityVector.z;
	velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
	player->SetMovementState(TCompPlayerModel::ActionStates::AirborneLong);
}

void GhostJumpWindowActionState::OnLanding() {
	//Ir a landing action state
	player->SetMovementState(TCompPlayerModel::ActionStates::Run);
}
