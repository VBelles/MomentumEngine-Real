#include "mcv_platform.h"
#include "JumpSquatActionState.h"

JumpSquatActionState::JumpSquatActionState(TCompPlayerModel * player)
	: GroundedActionState::GroundedActionState(player) {
}


void JumpSquatActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	//dbg("Entrando en JumpSquat\n");
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
	enteringVelocity = player->GetVelocityVector()->Length();
}

void JumpSquatActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	//dbg("Saliendo de JumpSquat\n");
}

void JumpSquatActionState::update (float delta) {
	PowerStats* currentPowerStats = player->GetPowerStats();
	collider = player->GetCollider();

	if (timer.elapsed() >= squatTime) {
		//saltar
		velocityVector->y = currentPowerStats->jumpVelocityVector.y;
		//Dejamos que el cambio de estado se haga cuando lo detecte ground sensor
		deltaMovement = *velocityVector * delta;
	}
	else {
		bool hasInput = movementInput != VEC2::Zero;
		deltaMovement = VEC3::Zero;
		if (hasInput) {
			deltaMovement = playerTransform->getFront() * enteringVelocity * delta;
		}
		//distancia vertical recorrida
		currentPowerStats->currentGravityMultiplier = velocityVector->y < 0 ? currentPowerStats->fallingMultiplier : currentPowerStats->normalGravityMultiplier;
		deltaMovement.y = CalculateVerticalDeltaMovement(delta, accelerationVector->y * currentPowerStats->currentGravityMultiplier, currentPowerStats->maxVelocityVertical);

		//Nueva velocidad vertical y clampeo
		velocityVector->y += accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta;
	}

	velocityVector->y = clamp(velocityVector->y, -currentPowerStats->maxVelocityVertical, currentPowerStats->maxVelocityVertical);
}

void JumpSquatActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

//ni caso a este input
void JumpSquatActionState::OnJumpHighButton() {}

void JumpSquatActionState::OnLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		player->SetMovementState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		player->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpSquat);
	}
}
