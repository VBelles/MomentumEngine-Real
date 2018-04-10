#include "mcv_platform.h"
#include "IdleActionState.h"

IdleActionState::IdleActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void IdleActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput != VEC2::Zero;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();

	//Buscamos un punto en la dirección en la que el jugador querría ir y, según si queda a izquierda o derecha, rotamos
	VEC3 desiredDirection = getCamera()->TransformToWorld(movementInput);
	bool isTurnAround = getPlayerModel()->getTransform()->getFront().Dot(desiredDirection) <= backwardsMaxDotProduct;
	if (hasInput && !isTurnAround) {
		VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
		rotatePlayerTowards(delta, targetPos, currentPowerStats->rotationSpeed);
	}

	//Si hay input se traslada toda la velocidad antigua a la nueva dirección de front y se le añade lo acelerado
	if (hasInput) {
		deltaMovement += calculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			getPlayerTransform()->getFront(), currentPowerStats->acceleration,
			getPlayerModel()->walkingSpeed);

		transferVelocityToDirectionAndAccelerate(delta, true, getPlayerTransform()->getFront(), currentPowerStats->acceleration);
		clampHorizontalVelocity(getPlayerModel()->walkingSpeed);
	}

	VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
	if (isTurnAround) {
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::IdleTurnAround);
	}
	else if (horizontalVelocity.Length() > 0.f) {
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Walk);
	}
}

void IdleActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	setPose();
}

void IdleActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
}
