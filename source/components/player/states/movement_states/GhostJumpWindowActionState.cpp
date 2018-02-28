#include "mcv_platform.h"
#include "GhostJumpWindowActionState.h"

GhostJumpWindowActionState::GhostJumpWindowActionState(CHandle playerHandle)
	: AirborneActionState::AirborneActionState(playerHandle) {
}


void GhostJumpWindowActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	//dbg("Entrando en GhostJumpWindow\n");
	squatTime = frameWindow * (1.f / 60);
	timer.reset();
	enteringVelocity = GetPlayer()->GetVelocityVector()->Length();
}

void GhostJumpWindowActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	//dbg("Saliendo de GhostJumpWindow\n");
}

void GhostJumpWindowActionState::update (float delta) {
	if (timer.elapsed() >= squatTime) {
		//Como estamos ya en el aire, hacemos el cambio nosotros mismos
		GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	AirborneActionState::update(delta);
}

void GhostJumpWindowActionState::SetMovementInput(VEC2 input) {
	AirborneActionState::SetMovementInput(input);
}

void GhostJumpWindowActionState::OnJumpHighButton() {
	PowerStats* currentPowerStats = GetPlayer()->GetPowerStats();
	velocityVector->y = 0.f;
	*velocityVector += currentPowerStats->jumpVelocityVector;
	//Como estamos ya en el aire, hacemos el cambio nosotros mismos
	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::AirborneNormal);
}

void GhostJumpWindowActionState::OnJumpLongButton() {
	PowerStats* currentPowerStats = GetPlayer()->GetPowerStats();
	*velocityVector = GetPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
	velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::AirborneLong);
}

void GhostJumpWindowActionState::OnLanding() {
	//Ir a landing action state
	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::Run);
}
