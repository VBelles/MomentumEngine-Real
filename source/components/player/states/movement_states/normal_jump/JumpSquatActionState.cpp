#include "mcv_platform.h"
#include "JumpSquatActionState.h"

JumpSquatActionState::JumpSquatActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void JumpSquatActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	if (timer.elapsed() >= squatTime) {
		//saltar
		GetPlayerModel()->isAttachedToPlatform = false;
		velocityVector->y = isShortHop ? currentPowerStats->shortHopVelocity : currentPowerStats->jumpVelocityVector.y;
		//Dejamos que el cambio de estado se haga cuando lo detecte ground sensor
		deltaMovement = *velocityVector * delta;
	}
	else {
		bool hasInput = movementInput != VEC2::Zero;
		if (hasInput) {
			deltaMovement += GetPlayerTransform()->getFront() * enteringVelocity * delta;
		}
	}
}

void JumpSquatActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	SetPose();
	//dbg("Entrando en JumpSquat\n");
	squatTime = squatFrames * (1.f / 60);
	isShortHop = false;
	timer.reset();
	enteringVelocity = GetPlayerModel()->GetVelocityVector()->Length();
}

void JumpSquatActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	//dbg("Saliendo de JumpSquat\n");
}

//ni caso a este input
void JumpSquatActionState::OnJumpHighButton() {}

void JumpSquatActionState::OnJumpHighButtonReleased() {
	isShortHop = true;
}

void JumpSquatActionState::OnStrongAttackButton() {
}

void JumpSquatActionState::OnFastAttackButton() {
}

void JumpSquatActionState::OnLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpSquat);
	}
}
