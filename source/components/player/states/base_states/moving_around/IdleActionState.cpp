#include "mcv_platform.h"
#include "IdleActionState.h"
#include "modules/game_modules/game/module_game.h"

IdleActionState::IdleActionState(StateManager * stateManager) :
	GroundedActionState(stateManager, Idle) {
}

void IdleActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();

	//Buscamos un punto en la direcci�n en la que el jugador querr�a ir y, seg�n si queda a izquierda o derecha, rotamos
	VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
	bool isTurnAround = getPlayerModel()->getTransform()->getFront().Dot(desiredDirection) <= backwardsMaxDotProduct;
	if (hasInput && !isTurnAround) {
		VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
		rotatePlayerTowards(delta, targetPos, currentPowerStats->rotationSpeed);
	}

	//Si hay input se traslada toda la velocidad antigua a la nueva direcci�n de front y se le a�ade lo acelerado
	if (hasInput) {
		deltaMovement += calculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			getPlayerTransform()->getFront(), currentPowerStats->acceleration,
			getPlayerModel()->walkingSpeed);

		transferVelocityToDirectionAndAccelerate(delta, true, getPlayerTransform()->getFront(), currentPowerStats->acceleration);
		clampHorizontalVelocity(getPlayerModel()->walkingSpeed);
	}

	if (!stateManager->isChangingBaseState) {
		VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
		if (isTurnAround) {
			stateManager->changeState(IdleTurnAround);
		}
		else if (horizontalVelocity.Length() > 0.f) {
			stateManager->changeState(Walk);
		}
	}

	if (closeCameraTimer.elapsed() >= closeCameraTime) {
		getCameraPlayer()->moveCameraCloser(true);
		EngineGame->showChrysalis();
	}
}

void IdleActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	closeCameraTimer.reset();
	getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
}

void IdleActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	EngineGame->hideChrysalis();
}

void IdleActionState::onSpendCoinsButton() {
	stateManager->changeState(SpendCoins);
}

void IdleActionState::onLevelChange(int powerLevel) {
	animation = animations[powerLevel - 1];
	if (stateManager->getState()->state == state) {
		getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	}
}
