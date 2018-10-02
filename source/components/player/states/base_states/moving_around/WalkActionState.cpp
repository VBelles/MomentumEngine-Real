#include "mcv_platform.h"
#include "WalkActionState.h"
#include "modules/system_modules/particles/comp_particles.h"


WalkActionState::WalkActionState(StateManager* stateManager) :
	GroundedActionState(stateManager, Walk) {
}

void WalkActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	float desiredVelocity = currentPowerStats->maxHorizontalSpeed;
	bool wantToWalk = false;
	if (movementInput.Length() < PAD_RUN_THRESHOLD || autoWalk) {
		desiredVelocity = getPlayerModel()->walkingSpeed;
		wantToWalk = true;
	}

	//Buscamos un punto en la dirección en la que el jugador querría ir y, según si queda a izquierda o derecha, rotamos
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

	//Si hay input se traslada toda la velocidad antigua a la nueva dirección de front y se le añade lo acelerado
	if (hasInput || autoWalk) {
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
	getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	//dustParticlesHandle = EngineParticles.launchSystem("data/particles/dust.particles", getEntity(), "", VEC3(0, 0.3, 0));
}

void WalkActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	//EngineParticles.kill(dustParticlesHandle, 0.5f);
	autoWalk = false;
}

void WalkActionState::onSpendCoinsButton() {
	stateManager->changeState(SpendCoins);
}

void WalkActionState::onLevelChange(int powerLevel) {
	animation = animations[powerLevel - 1];
	if (stateManager->getState()->state == state) {
		getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	}
}

