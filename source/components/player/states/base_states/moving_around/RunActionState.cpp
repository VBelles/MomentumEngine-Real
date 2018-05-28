#include "mcv_platform.h"
#include "RunActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"
#include "components/comp_camera.h"
#include "components/controllers/comp_camera_player.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


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

	//Buscamos un punto en la dirección en la que el jugador querría ir y, según si queda a izquierda o derecha, rotamos
	VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
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
	getSkeleton()->blendCycle(animation, 0.2f, 0.2f);

	CEntity* cameraEntity = getEntityByName(PLAYER_CAMERA);
	TCompCameraPlayer* camera = cameraEntity->get<TCompCameraPlayer>();
	camera->moveCameraCloser(false);
}

void RunActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
}
