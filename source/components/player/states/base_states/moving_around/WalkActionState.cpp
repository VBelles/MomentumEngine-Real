#include "mcv_platform.h"
#include "WalkActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_camera.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


WalkActionState::WalkActionState(StateManager* stateManager) :
	GroundedActionState(stateManager, Walk) {
}

void WalkActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput != VEC2::Zero;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	float desiredVelocity = currentPowerStats->maxHorizontalSpeed;
	bool wantToWalk = false;
	if (movementInput.Length() < 0.8f) {
		desiredVelocity = getPlayerModel()->walkingSpeed;
		wantToWalk = true;
	}

	//Buscamos un punto en la dirección en la que el jugador querría ir y, según si queda a izquierda o derecha, rotamos
	VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
	bool isTurnAround = getPlayerModel()->getTransform()->getFront().Dot(desiredDirection) <= backwardsMaxDotProduct;
	if (hasInput && !isTurnAround) {
		VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
		rotatePlayerTowards(delta, targetPos, currentPowerStats->rotationSpeed);
	}

	//Si hay input se traslada toda la velocidad antigua a la nueva dirección de front y se le añade lo acelerado
	if (hasInput) {
		deltaMovement += calculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			getPlayerTransform()->getFront(), currentPowerStats->acceleration,
			desiredVelocity);

		transferVelocityToDirectionAndAccelerate(delta, true, getPlayerTransform()->getFront(), currentPowerStats->acceleration);
		clampHorizontalVelocity(desiredVelocity);
	}
	else {
		VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
		if (currentPowerStats->deceleration * delta < horizontalVelocity.Length()) {
			deltaMovement = calculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
				-VEC3(velocityVector->x, 0, velocityVector->z), currentPowerStats->deceleration, desiredVelocity);

			transferVelocityToDirectionAndAccelerate(delta, false, -VEC3(velocityVector->x, 0, velocityVector->z), currentPowerStats->deceleration);
		}
		else {
			velocityVector->x = 0.f;
			velocityVector->z = 0.f;
		}
	}

	if (!stateManager->isChangingBaseState) {
		if (isTurnAround) {
			stateManager->changeState(TurnAround);
		}
		else {
			VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };

			if (horizontalVelocity.Length() == 0.f) {
				stateManager->changeState(Idle);
			}
			else if (!wantToWalk && horizontalVelocity.Length() > getPlayerModel()->walkingSpeed) {
				stateManager->changeState(Run);
			}
		}
	}

}

void WalkActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	getPlayerModel()->getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
	getPlayerModel()->getSkeleton()->setTimeFactor(0.5f);
}

void WalkActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	getPlayerModel()->getSkeleton()->setTimeFactor(1);
}

