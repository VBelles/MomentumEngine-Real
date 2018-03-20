#include "mcv_platform.h"
#include "AirborneActionState.h"

AirborneActionState::AirborneActionState(CHandle playerModelHandle)
	: IActionState::IActionState(playerModelHandle) {
	accelerationVector = GetPlayerModel()->GetAccelerationVector();
	velocityVector = GetPlayerModel()->GetVelocityVector();
}

void AirborneActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput != VEC2::Zero;

	
	
	VEC3 desiredDirection = GetCamera()->TransformToWorld(movementInput);
	
	if (hasInput){// && enterFront.Dot(desiredDirection) > backwardsMaxDotProduct) {
		VEC3 targetPos = GetPlayerTransform()->getPosition() + desiredDirection;
		RotatePlayerTowards(delta, targetPos, 3.5f);
	}

	if (hasInput) {
		//aceleración según sentido de movimiento
		float appliedAcceleration = CalculateAccelerationAccordingToDirection(enterFront, desiredDirection,
			enteringPowerStats->airAcceleration, backwardsMaxDotProduct, sidewaysMaxDotProduct, backwardsAirDriftFactor,
			sidewaysAirDriftFactor);

		deltaMovement += CalculateHorizontalDeltaMovement(delta, VEC3{ velocityVector->x , 0 , velocityVector->z},
			desiredDirection, appliedAcceleration,
			enteringPowerStats->maxHorizontalSpeed);
		TransferVelocityToDirectionAndAccelerate(delta, false, desiredDirection, appliedAcceleration);
		ClampHorizontalVelocity(enteringPowerStats->maxHorizontalSpeed);
	}
	else {
		deltaMovement.x = velocityVector->x * delta;
		deltaMovement.z = velocityVector->z * delta;
	}

	if(velocityVector->y < 0){
		GetPlayerModel()->SetGravityMultiplier(enteringPowerStats->fallingMultiplier);
	}
}

void AirborneActionState::OnStateEnter(IActionState * lastState) {
	IActionState::OnStateEnter(lastState);
	enteringPowerStats = &*GetPlayerModel()->GetPowerStats();
	GetPlayerModel()->maxVerticalSpeed = enteringPowerStats->maxVelocityVertical;
	GetPlayerModel()->ResetGravity();
	enterFront = GetPlayerTransform()->getFront();
	sidewaysMaxDotProduct = cos(deg2rad(sidewaysdMinAngle));
	backwardsMaxDotProduct = cos(deg2rad(backwardsdMinAngle));
}

void AirborneActionState::OnStateExit(IActionState * nextState) {
	IActionState::OnStateExit(nextState);
}

void AirborneActionState::OnJumpHighButton() {
	//grab high
}

void AirborneActionState::OnJumpLongButton() {
	//grab long
}

void AirborneActionState::OnStrongAttackButton() {
	if (GetPlayerModel()->IsAttackFree()) {
		GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::FallingAttack);
	}
}

void AirborneActionState::OnFastAttackButton() {
}


void AirborneActionState::OnLanding() {
	//Ir a landing action state
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Landing);
}
