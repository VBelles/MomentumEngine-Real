#include "mcv_platform.h"
#include "GhostJumpWindowActionState.h"

GhostJumpWindowActionState::GhostJumpWindowActionState(TCompPlayerModel * player)
	: AirborneActionState::AirborneActionState(player) {
}


void GhostJumpWindowActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	dbg("Entrando en GhostJumpWindow\n");
	squatTime = frameWindow * (1.f / 60);
	timer.reset();
	enteringVelocity = player->GetVelocityVector()->Length();
}

void GhostJumpWindowActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	dbg("Saliendo de GhostJumpWindow\n");
}

void GhostJumpWindowActionState::update (float delta) {
	if (timer.elapsed() >= squatTime) {
		//Como estamos ya en el aire, hacemos el cambio nosotros mismos
		player->SetActionState(TCompPlayerModel::ActionStates::Airborne);
	}
}

void GhostJumpWindowActionState::SetMovementInput(VEC2 input, float delta) {
	AirborneActionState::SetMovementInput(input, delta);
}

void GhostJumpWindowActionState::OnJumpHighButton() {
	//saltar
	PowerStats* currentPowerStats = player->GetPowerStats();
	VEC3* velocityVector = player->GetVelocityVector();
	velocityVector->y = 0.f;
	*velocityVector += currentPowerStats->jumpVelocityVector;
	//Como estamos ya en el aire, hacemos el cambio nosotros mismos
	player->SetActionState(TCompPlayerModel::ActionStates::Airborne);
}

void GhostJumpWindowActionState::OnLanding() {
	//Ir a landing action state
	player->SetActionState(TCompPlayerModel::ActionStates::Grounded);
}
