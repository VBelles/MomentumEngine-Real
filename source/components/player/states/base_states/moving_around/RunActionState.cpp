#include "mcv_platform.h"
#include "RunActionState.h"
#include "modules/system_modules/slash/comp_slash.h"
#include "modules/system_modules/particles/comp_particles.h"


RunActionState::RunActionState(StateManager* stateManager) :
	GroundedActionState(stateManager, Run) {
}

void RunActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	bool wantToWalk = false;
	if (movementInput.Length() < PAD_RUN_THRESHOLD) {
		wantToWalk = true;
	}

	//Buscamos un punto en la direcci�n en la que el jugador querr�a ir y, seg�n si queda a izquierda o derecha, rotamos
	VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
	float dotProduct = getPlayerModel()->getTransform()->getFront().Dot(desiredDirection);
	bool isTurnAround = dotProduct <= backwardsMaxDotProduct;
	if (hasInput && !isTurnAround) {
		//de 1 a backwardsMaxDotProduct, lerp de rotationSpeed entre min y currentPowerStats->rotationSpeed
		float minRotationSpeed = 1.f;
		dotProduct = scale(dotProduct, backwardsMaxDotProduct, 1, 0, 1);
		float rotationSpeed = lerp(minRotationSpeed, currentPowerStats->rotationSpeed, 1 - dotProduct);
		VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
		rotatePlayerTowards(delta, targetPos, rotationSpeed);
	}

	//Si hay input se traslada toda la velocidad antigua a la nueva direcci�n de front y se le a�ade lo acelerado
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

	if (!stateManager->isChangingBaseState) {
		if (isTurnAround) {
			stateManager->changeState(TurnAround);
		}
		else {
			VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };

			if (horizontalVelocity.Length() == 0.f) {
				stateManager->changeState(Idle);
			}
			else if (horizontalVelocity.Length() <= getPlayerModel()->walkingSpeed) {
				stateManager->changeState(Walk);
			}
		}
	}
}

void RunActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	getSkeleton()->blendCycle(animation, 0.3f, 0.3f);
	getCameraPlayer()->moveCameraCloser(false);
	EngineParticles.launchSystem("data/particles/dust.particles", Particles::LaunchConfig{ getPlayerEntity(), "", VEC3(0, 0.15, 0) });

	if (hasSlash) {
		getTrailSlash(SlashType::LEFT_TENTACLE)->setEnable(true);
		getTrailSlash(SlashType::RIGHT_TENTACLE)->setEnable(true);
	}
}

void RunActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	getTrailSlash(SlashType::LEFT_TENTACLE)->stopEmitting();
	getTrailSlash(SlashType::RIGHT_TENTACLE)->stopEmitting();
}

void RunActionState::onSpendCoinsButton() {
	stateManager->changeState(SpendCoins);
}

void RunActionState::onLevelChange(int powerLevel) {
	animation = animations[powerLevel - 1];
	if (stateManager->getState()->state == state) {
		getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	}

	/*if (powerLevel == 3) {
		hasSlash = true;
		if (stateManager->getState()->state == Run) {
			getTrailSlash(SlashType::LEFT_TENTACLE)->setEnable(true);
			getTrailSlash(SlashType::RIGHT_TENTACLE)->setEnable(true);
		}
	}
	else {
		hasSlash = false;
		if (stateManager->getState()->state == Run) {
			getTrailSlash(SlashType::LEFT_TENTACLE)->stopEmitting();
			getTrailSlash(SlashType::RIGHT_TENTACLE)->stopEmitting();
		}
	}*/

}
