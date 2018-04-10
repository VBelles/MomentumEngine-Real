#include "mcv_platform.h"
#include "AirborneWallJumpActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"

AirborneWallJumpActionState::AirborneWallJumpActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void AirborneWallJumpActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;

	deltaMovement.x = velocityVector->x * delta;
	deltaMovement.z = velocityVector->z * delta;
	if (velocityVector->y < 0) {
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}

	//bool hasInput = movementInput != VEC2::Zero;

	//VEC3 desiredDirection = getCamera()->TransformToWorld(movementInput);
	//if (hasInput && velocityVector->y < 0) {
	//	VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
	//	rotatePlayerTowards(delta, targetPos, rotationSpeed);

	//	//aceleración según sentido de movimiento
	//	float appliedAcceleration = calculateAccelerationAccordingToDirection(enterFront, desiredDirection,
	//		enteringPowerStats->acceleration, backwardsMaxDotProduct, sidewaysMaxDotProduct,
	//		backwardsAirDriftFactorWall, sidewaysAirDriftFactorWall);

	//	deltaMovement += calculateHorizontalDeltaMovement(delta, VEC3{ velocityVector->x , 0 , velocityVector->z },
	//		desiredDirection, appliedAcceleration, enteringPowerStats->wallJumpVelocityVector.z);

	//	transferVelocityToDirectionAndAccelerate(delta, false, desiredDirection, appliedAcceleration);
	//	clampHorizontalVelocity(enteringPowerStats->wallJumpVelocityVector.z);
	//}
	//else {
	//	deltaMovement.x = velocityVector->x * delta;
	//	deltaMovement.z = velocityVector->z * delta;
	//}

}

void AirborneWallJumpActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	setPose();
	getPlayerModel()->maxVerticalSpeed = enteringPowerStats->maxVelocityVertical;
	getPlayerModel()->setGravityMultiplier(1.1f);
	//dbg("Entrando en airborne long\n");
	enterFront = getPlayerTransform()->getFront();
	sidewaysMaxDotProduct = cos(deg2rad(sidewaysdMinAngle));
	backwardsMaxDotProduct = cos(deg2rad(backwardsdMinAngle));
}

void AirborneWallJumpActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	//dbg("Saliendo de airborne long\n");
}

void AirborneWallJumpActionState::setPose() {
	getRender()->setMesh("data/meshes/pose_long_jump.mesh");
}
