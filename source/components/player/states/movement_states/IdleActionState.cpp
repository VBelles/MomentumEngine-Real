#include "mcv_platform.h"
#include "IdleActionState.h"

IdleActionState::IdleActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void IdleActionState::update (float delta) {
	GroundedActionState::update(delta);
	if (velocityVector->x > 0.f || velocityVector->z > 0.f) {
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Run);
	}
}

void IdleActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	SetPose();
	dbg("Entrando en idle\n");
}

void IdleActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	dbg("Saliendo de idle\n");
}

void IdleActionState::OnJumpHighButton() {
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquat);
}

void IdleActionState::OnJumpLongButton() {
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void IdleActionState::OnLeavingGround() {
	//Set state a alguno por defecto, luego las clases derivadas de esta ya sabrán qué hacer
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}
