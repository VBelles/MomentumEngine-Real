#include "mcv_platform.h"
#include "RunActionState.h"

RunActionState::RunActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void RunActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput != VEC2::Zero;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	bool wantToWalk = false;
	if (movementInput.Length() < 0.8f) {
		wantToWalk = true;
	}

	//Buscamos un punto en la dirección en la que el jugador querría ir y, según si queda a izquierda o derecha, rotamos
	VEC3 desiredDirection = getCamera()->TransformToWorld(movementInput);
	bool isTurnAround = getPlayerModel()->getTransform()->getFront().Dot(desiredDirection) <= backwardsMaxDotProduct;
	if (hasInput && !isTurnAround) {
		VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
		rotatePlayerTowards(delta, targetPos, currentPowerStats->rotationSpeed);
	}

	//Si hay input se traslada toda la velocidad antigua a la nueva dirección de front y se le añade lo acelerado
	if (hasInput && !wantToWalk) {
		deltaMovement += calculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			getPlayerTransform()->getFront(), currentPowerStats->acceleration,
			currentPowerStats->maxHorizontalSpeed);

		transferVelocityToDirectionAndAccelerate(delta, true, getPlayerTransform()->getFront(), currentPowerStats->acceleration);
		clampHorizontalVelocity(currentPowerStats->maxHorizontalSpeed);
	}
	else {
		VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
		if (currentPowerStats->deceleration * delta < horizontalVelocity.Length()) {
			deltaMovement = calculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
				-VEC3(velocityVector->x, 0, velocityVector->z), currentPowerStats->deceleration, currentPowerStats->maxHorizontalSpeed);

			transferVelocityToDirectionAndAccelerate(delta, false, -VEC3(velocityVector->x, 0, velocityVector->z), currentPowerStats->deceleration);
		}
		else {
			velocityVector->x = 0.f;
			velocityVector->z = 0.f;
		}
	}

	if (isTurnAround) {
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::TurnAround);
	}
	else {
		VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };

		if (horizontalVelocity.Length() == 0.f) {
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Idle);
		}
		else if (horizontalVelocity.Length() <= getPlayerModel()->walkingSpeed) {
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Walk);
		}
	}

}

void RunActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	setPose();
}

void RunActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
}
