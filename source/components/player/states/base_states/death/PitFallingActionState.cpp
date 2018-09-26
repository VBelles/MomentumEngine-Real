#include "mcv_platform.h"
#include "PitFallingActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/comp_power_gauge.h"
#include "components/player/states/StateManager.h"
#include "components/controllers/comp_camera_player.h"

PitFallingActionState::PitFallingActionState(StateManager* stateManager) :
	IActionState(stateManager, PitFalling) {
}

void PitFallingActionState::update(float delta) {
	if (timer.elapsed() >= respawnTime) {
		if (finish) {
			if (frameCounter < 1) {
				if (frameCounter == 0) {
					//poner la cámara del player donde toca
					getSkeleton()->blendCycle(animationIdle, 0.0f, 0.0f);
					CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
					TCompCameraPlayer* playerCamera = playerCameraEntity->get<TCompCameraPlayer>();
					playerCamera->resetCurrentDistance();
				}
				frameCounter++;
			}
			else {
				stateManager->changeConcurrentState(Free);
				stateManager->changeState(AirborneNormal);
			}
		}
		else {
			respawn();
			frameCounter = 0;
			finish = true;
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
	EngineSound.emitEvent(SOUND_DIE);
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
		getPlayerModel()->resetHp();
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
