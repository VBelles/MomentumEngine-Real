#include "mcv_platform.h"
#include "AirborneLongActionState.h"

AirborneLongActionState::AirborneLongActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, AirborneLong) {
}

void AirborneLongActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;

	VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);

	if (hasInput) {
		//aceleración según sentido de movimiento
		float appliedAcceleration = calculateAccelerationAccordingToDirection(enterFront, desiredDirection,
			enteringPowerStats->acceleration, backwardsMaxDotProduct, sidewaysMaxDotProduct,
			backwardsAirDriftFactorLong, sidewaysAirDriftFactorLong);

		deltaMovement += calculateHorizontalDeltaMovement(delta, VEC3{ velocityVector->x , 0 , velocityVector->z },
			desiredDirection, appliedAcceleration, enteringPowerStats->longJumpVelocityVector.z);

		transferVelocityToDirectionAndAccelerate(delta, false, desiredDirection, appliedAcceleration);
		clampHorizontalVelocity(enteringPowerStats->longJumpVelocityVector.z);
	}
	else {
		deltaMovement.x = velocityVector->x * delta;
		deltaMovement.z = velocityVector->z * delta;
	}
}

void AirborneLongActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	getPlayerModel()->setGravityMultiplier(enteringPowerStats->longGravityMultiplier);
	//dbg("Entrando en airborne long\n");
	getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
}

void AirborneLongActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	//dbg("Saliendo de airborne long\n");
}
