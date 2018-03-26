#include "mcv_platform.h"
#include "AirborneWallJumpActionState.h"

AirborneWallJumpActionState::AirborneWallJumpActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void AirborneWallJumpActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput != VEC2::Zero;

	VEC3 desiredDirection = GetCamera()->TransformToWorld(movementInput);

	if (hasInput && velocityVector->y < 0) {

	/*	VEC3 targetPos = GetPlayerTransform()->getPosition() + desiredDirection;
		RotatePlayerTowards(delta, targetPos, rotationSpeed);*/

		//aceleración según sentido de movimiento
		float appliedAcceleration = CalculateAccelerationAccordingToDirection(enterFront, desiredDirection,
			enteringPowerStats->acceleration, backwardsMaxDotProduct, sidewaysMaxDotProduct,
			backwardsAirDriftFactorWall, sidewaysAirDriftFactorWall);

		deltaMovement += CalculateHorizontalDeltaMovement(delta, VEC3{ velocityVector->x , 0 , velocityVector->z },
			desiredDirection, appliedAcceleration, enteringPowerStats->wallJumpVelocityVector.z);

		TransferVelocityToDirectionAndAccelerate(delta, false, desiredDirection, appliedAcceleration);
		ClampHorizontalVelocity(enteringPowerStats->wallJumpVelocityVector.z);
	}
	else {
		deltaMovement.x = velocityVector->x * delta;
		deltaMovement.z = velocityVector->z * delta;
	}
}

void AirborneWallJumpActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	SetPose();
	GetPlayerModel()->maxVerticalSpeed = enteringPowerStats->maxVelocityVertical;
	GetPlayerModel()->SetGravityMultiplier(1.1f);
	//dbg("Entrando en airborne long\n");
	enterFront = GetPlayerTransform()->getFront();
	sidewaysMaxDotProduct = cos(deg2rad(sidewaysdMinAngle));
	backwardsMaxDotProduct = cos(deg2rad(backwardsdMinAngle));
}

void AirborneWallJumpActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	//dbg("Saliendo de airborne long\n");
}