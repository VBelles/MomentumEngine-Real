#include "mcv_platform.h"
#include "TeleportActionState.h"
#include "components/postfx/comp_screen_transition.h"

TeleportActionState::TeleportActionState(StateManager* stateManager) :
	IActionState(stateManager, Teleport) {
}

void TeleportActionState::update(float delta) {
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

void TeleportActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);
	getSkeleton()->blendCycle(animation, 0.2, 0.2);
	timer.reset();
	finish = false;

	//getSound()->play("pit_falling");

	screenTransitionStarted = false;
}

void TeleportActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
	CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
	EngineCameras.blendInCamera(playerCameraEntity, 0.00001f, CModuleCameras::EPriority::GAMEPLAY);
}

void TeleportActionState::respawn() {
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
}

TCompScreenTransition* TeleportActionState::getScreenTransition() {
	CEntity* playerCameraEntity = getEntityByName(GAME_CAMERA);
	return playerCameraEntity->get<TCompScreenTransition>();
}