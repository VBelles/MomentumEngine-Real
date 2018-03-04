#include "mcv_platform.h"
#include "JumpSquatLongActionState.h"

JumpSquatLongActionState::JumpSquatLongActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}


void JumpSquatLongActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	SetPose();
	//dbg("Entrando en JumpSquatLong\n");
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
}

void JumpSquatLongActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	//dbg("Saliendo de JumpSquatLong\n");
}

void JumpSquatLongActionState::update (float delta) {
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	
	if (timer.elapsed() >= squatTime) {
		//saltar
		*velocityVector = GetPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
		//Dejamos que el cambio de estado se haga cuando lo detecte ground sensor
		deltaMovement = *velocityVector * delta;
	}
	else {
		deltaMovement = VEC3::Zero;
		//distancia vertical recorrida
		currentPowerStats->currentGravityMultiplier = currentPowerStats->normalGravityMultiplier;
		deltaMovement.y = CalculateVerticalDeltaMovement(delta, accelerationVector->y * currentPowerStats->currentGravityMultiplier, currentPowerStats->maxVelocityVertical);

		//Nueva velocidad vertical y clampeo
		velocityVector->y += accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta;
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
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::AirborneLong);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpSquatLong);
	}
}
