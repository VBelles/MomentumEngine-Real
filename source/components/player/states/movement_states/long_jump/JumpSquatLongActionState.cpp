#include "mcv_platform.h"
#include "JumpSquatLongActionState.h"

JumpSquatLongActionState::JumpSquatLongActionState(TCompPlayerModel * player)
	: GroundedActionState::GroundedActionState(player) {
}


void JumpSquatLongActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	//dbg("Entrando en JumpSquatLong\n");
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
}

void JumpSquatLongActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	//dbg("Saliendo de JumpSquatLong\n");
}

void JumpSquatLongActionState::update (float delta) {
	PowerStats* currentPowerStats = player->GetPowerStats();
	collider = player->GetCollider();

	if (timer.elapsed() >= squatTime) {
		//saltar
		*velocityVector = playerTransform->getFront() * currentPowerStats->longJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
		//Dejamos que el cambio de estado se haga cuando lo detecte ground sensor
		deltaMovement = *velocityVector * delta;
	}
	else {
		deltaMovement = VEC3::Zero;
		currentPowerStats->currentGravityMultiplier = currentPowerStats->normalGravityMultiplier;
		float verticalVelocityIncrement = accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta;
		deltaMovement.y = velocityVector->y * delta + 0.5f * verticalVelocityIncrement * delta;
		velocityVector->y += verticalVelocityIncrement;
	}

	velocityVector->y = clamp(velocityVector->y, -currentPowerStats->maxVelocityVertical, currentPowerStats->maxVelocityVertical);
}

void JumpSquatLongActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

//ni caso a este input
void JumpSquatLongActionState::OnJumpHighButton() {}

void JumpSquatLongActionState::OnJumpLongButton() {}

void JumpSquatLongActionState::OnLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		player->SetMovementState(TCompPlayerModel::ActionStates::AirborneLong);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		player->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpSquatLong);
	}
}
