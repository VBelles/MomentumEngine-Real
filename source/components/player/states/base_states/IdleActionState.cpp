#include "mcv_platform.h"
#include "IdleActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


IdleActionState::IdleActionState(StateManager * stateManager) :
	GroundedActionState(stateManager, Idle) {
}

void IdleActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput != VEC2::Zero;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();

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
			getPlayerModel()->walkingSpeed);

		transferVelocityToDirectionAndAccelerate(delta, true, getPlayerTransform()->getFront(), currentPowerStats->acceleration);
		clampHorizontalVelocity(getPlayerModel()->walkingSpeed);
	}

	if (!isChangingBaseState) {
		VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
		if (isTurnAround) {
			stateManager->changeState(IdleTurnAround);
		}
		else if (horizontalVelocity.Length() > 0.f) {
			stateManager->changeState(Walk);
		}
	}
}

void IdleActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	getPlayerModel()->getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
}

void IdleActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
}

