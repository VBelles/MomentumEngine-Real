#include "mcv_platform.h"
#include "AirborneActionState.h"

AirborneActionState::AirborneActionState(TCompPlayerModel * player) 
	: IActionState::IActionState(player) {
	accelerationVector = player->GetAccelerationVector();
	velocityVector = player->GetVelocityVector();
	playerTransform = player->GetTransform();
}

void AirborneActionState::update (float delta) {
	bool hasInput = movementInput != VEC2::Zero;
	currentCamera = player->GetCamera();
	collider = player->GetCollider();
	PowerStats* currentPowerStats = player->GetPowerStats();
	
	VEC3 desiredDirection = player->GetCamera()->TransformToWorld(movementInput);
	
	if (hasInput){// && enterFront.Dot(desiredDirection) > backwardsMaxDotProduct) {
		VEC3 targetPos = playerTransform->getPosition() + desiredDirection;
		RotatePlayerTowards(delta, targetPos, 3.5f);
	}

	if (hasInput) {
		//aceleración según sentido de movimiento
		float appliedAcceleration = CalculateAccelerationAccordingToDirection(enterFront, desiredDirection,
			currentPowerStats->airAcceleration, backwardsMaxDotProduct, sidewaysMaxDotProduct, backwardsAirDriftFactor,
			sidewaysAirDriftFactor);

		deltaMovement = CalculateHorizontalDeltaMovement(delta, VEC3{ velocityVector->x , 0 , velocityVector->z},
			desiredDirection, appliedAcceleration,
			currentPowerStats->maxHorizontalSpeed);
		TransferVelocityToDirectionAndAccelerate(delta, false, desiredDirection, appliedAcceleration);
		ClampHorizontalVelocity(currentPowerStats->maxHorizontalSpeed);
	}
	else {
		deltaMovement.x = velocityVector->x * delta;
		deltaMovement.z = velocityVector->z * delta;
	}

	//distancia vertical recorrida
	currentPowerStats->currentGravityMultiplier = velocityVector->y < 0 ? currentPowerStats->fallingMultiplier : currentPowerStats->normalGravityMultiplier;
	deltaMovement.y = CalculateVerticalDeltaMovement(delta, accelerationVector->y * currentPowerStats->currentGravityMultiplier, currentPowerStats->maxVelocityVertical);

	//Nueva velocidad vertical y clampeo
	velocityVector->y += accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta;
	velocityVector->y = clamp(velocityVector->y, -currentPowerStats->maxVelocityVertical, currentPowerStats->maxVelocityVertical);
}

void AirborneActionState::OnStateEnter(IActionState * lastState) {
	IActionState::OnStateEnter(lastState);
	enterFront = playerTransform->getFront();
	sidewaysMaxDotProduct = cos(deg2rad(sidewaysdMinAngle));
	backwardsMaxDotProduct = cos(deg2rad(backwardsdMinAngle));
}

void AirborneActionState::OnStateExit(IActionState * nextState) {
	IActionState::OnStateExit(nextState);
}

void AirborneActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

void AirborneActionState::OnJumpHighButton() {
	//grab high
}

void AirborneActionState::OnJumpLongButton() {
	//grab long
}

void AirborneActionState::OnStrongAttackButton() {
	if (player->IsAttackFree()) {
		player->SetAttackState(TCompPlayerModel::ActionStates::FallingAttack);
	}
}

void AirborneActionState::OnFastAttackButton() {
}


void AirborneActionState::OnLanding() {
	//Ir a landing action state
	player->SetMovementState(TCompPlayerModel::ActionStates::Run);
}
