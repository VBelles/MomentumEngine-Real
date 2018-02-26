#include "mcv_platform.h"
#include "AirborneLongActionState.h"

AirborneLongActionState::AirborneLongActionState(TCompPlayerModel * player)
	: AirborneActionState::AirborneActionState(player) {
}

void AirborneLongActionState::update (float delta) {
	bool hasInput = movementInput != VEC2::Zero;
	currentCamera = player->GetCamera();
	collider = player->GetCollider();	
	PowerStats* currentPowerStats = player->GetPowerStats();

	VEC3 desiredDirection = player->GetCamera()->TransformToWorld(movementInput);

	if (hasInput) {
		deltaMovement = CalculateHorizontalDeltaMovement(delta, VEC3{ velocityVector->x , 0 , velocityVector->z },
			desiredDirection, currentPowerStats->acceleration,
			currentPowerStats->longJumpVelocityVector.z);
		TransferVelocityToDirectionAndAccelerate(delta, false, desiredDirection, currentPowerStats->acceleration);
		ClampHorizontalVelocity(currentPowerStats->longJumpVelocityVector.z);
	}
	else {
		deltaMovement.x = velocityVector->x * delta;
		deltaMovement.z = velocityVector->z * delta;
	}

	currentPowerStats->currentGravityMultiplier = currentPowerStats->longGravityMultiplier;
	deltaMovement.y = CalculateVerticalDeltaMovement(delta, accelerationVector->y * currentPowerStats->currentGravityMultiplier, currentPowerStats->maxVelocityVertical);

	velocityVector->y += accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta;
	velocityVector->y = clamp(velocityVector->y, -currentPowerStats->maxVelocityVertical, currentPowerStats->maxVelocityVertical);
}

void AirborneLongActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	//dbg("Entrando en airborne long\n");
}

void AirborneLongActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	//dbg("Saliendo de airborne long\n");
}

void AirborneLongActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

void AirborneLongActionState::OnJumpHighButton() {
	//grab
}

void AirborneLongActionState::OnLanding() {
	//Ir a landing action state
	player->SetMovementState(TCompPlayerModel::ActionStates::Run);
}
