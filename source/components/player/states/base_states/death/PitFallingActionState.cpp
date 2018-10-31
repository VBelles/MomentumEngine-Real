#include "mcv_platform.h"
#include "PitFallingActionState.h"
#include "components/postfx/comp_screen_transition.h"

PitFallingActionState::PitFallingActionState(StateManager* stateManager) :
	IActionState(stateManager, PitFalling) {
}

void PitFallingActionState::update(float delta) {
	if (timer.elapsed() >= respawnTime) {
		if (!screenTransitionStarted) {
			getScreenTransition()->startTransition(0.f, 1.f);
			screenTransitionStarted = true;
		}
		else {
			if (!getScreenTransition()->isTransitioning()) {
				if (finish) {
					stateManager->changeConcurrentState(Free);
					stateManager->changeState(AirborneNormal);
				}
				else {
					respawn();
					frameCounter = 0;
					finish = true;
					getSkeleton()->blendCycle(animationIdle, 0.0f, 0.0f);
					CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
					EngineCameras.blendInCamera(playerCameraEntity, 0.00001f, CModuleCameras::EPriority::GAMEPLAY);
					getScreenTransition()->startTransition(1.f, 0.f, 0.25f);
				}
			}
		}
	}
	else {
		deltaMovement = VEC3::Zero;
		deltaMovement.y = velocityVector->y * delta;
	}
}

void PitFallingActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);
	//getSkeleton()->blendCycle(animation);
	timer.reset();
	finish = false;

	copyCam();
	CEntity* teleportCameraEntity = getEntityByName(TELEPORT_CAMERA);
	EngineCameras.blendInCamera(teleportCameraEntity, 0.001f, CModuleCameras::EPriority::GAMEPLAY);
	getSound()->play("pit_falling");

	screenTransitionStarted = false;
}

void PitFallingActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
	CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
	EngineCameras.blendInCamera(playerCameraEntity, 0.00001f, CModuleCameras::EPriority::GAMEPLAY);
}

void PitFallingActionState::respawn() {
	*velocityVector = VEC3();
	assert(getPlayerModel());
	VEC3 respawnPosition = getPlayerModel()->getRespawnPosition();
	TCompTransform* transform = getPlayerTransform();
	float yaw, pitch;
	transform->getYawPitchRoll(&yaw, &pitch);
	transform->setYawPitchRoll(getPlayerModel()->getRespawnYaw(), pitch);
	getCollider()->controller->setFootPosition(PxExtendedVec3(respawnPosition.x, respawnPosition.y, respawnPosition.z));

	CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
	TCompTransform* cameraTransform = playerCameraEntity->get<TCompTransform>();
	TCompCameraPlayer* playerCamera = playerCameraEntity->get<TCompCameraPlayer>();
	cameraTransform->setYawPitchRoll(getPlayerModel()->getRespawnYaw(), playerCamera->getInitialPitch());

	getPlayerModel()->damage(fallingDamage);
	if (getPlayerModel()->getHp() < 1) {
		getPlayerModel()->numberOfDeaths++;
		getPlayerModel()->refillHp();
		getPowerGauge()->resetPower();
	}
}

void PitFallingActionState::copyCam() {
	//presupone que entramos siempre en player camera
	CEntity* copyCamEntity = getEntityByName(PLAYER_CAMERA);
	CEntity* pasteCamEntity = getEntityByName(TELEPORT_CAMERA);

	TCompTransform* copyCamTransform = copyCamEntity->get<TCompTransform>();
	TCompTransform* pasteCamTransform = pasteCamEntity->get<TCompTransform>();
	pasteCamTransform->setPosition(copyCamTransform->getPosition());
	pasteCamTransform->setRotation(copyCamTransform->getRotation());
}

TCompScreenTransition* PitFallingActionState::getScreenTransition() {
	CEntity* playerCameraEntity = getEntityByName(GAME_CAMERA);
	return playerCameraEntity->get<TCompScreenTransition>();
}