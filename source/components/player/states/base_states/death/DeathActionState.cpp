#include "mcv_platform.h"
#include "DeathActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/comp_power_gauge.h"
#include "components/player/states/StateManager.h"
#include "components/controllers/comp_camera_player.h"

DeathActionState::DeathActionState(StateManager* stateManager) :
	IActionState(stateManager, Death) {
}

void DeathActionState::update(float delta) {
	if (timer.elapsed() >= deathTime) {
		//Engine.getEntities().setManagerUpdate("skeleton", false);
	}
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
			//Engine.getEntities().setManagerUpdate("skeleton", true);
			respawn();
			frameCounter = 0;
			finish = true;
		}
	}
	if (timer.elapsed() >= startSelfIllumDecreaseTime) {
		TCompRender* render = getPlayerModel()->get<TCompRender>();
		render->selfIllumRatio = clamp(render->selfIllumRatio - selfIllumDecreaseSpeed * delta, selfIllumMinimumRatio, 1.f);
	}
}

void DeathActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState); 
	getSkeleton()->blendCycle(animationLoop, 0.0f, 0.0f);
	getSkeleton()->executeAction(animation, 0.1f, 0.0f);
	getPlayerModel()->setGravity(0);
	timer.reset();
	finish = false;
	EngineScripting.throwEvent(onPlayerKilled, "");

	copyCam();
	CEntity* teleportCameraEntity = getEntityByName(TELEPORT_CAMERA);
	EngineCameras.blendInCamera(teleportCameraEntity, 0.001f, CModuleCameras::EPriority::GAMEPLAY);
	EngineSound.emitEvent(SOUND_DIE);
}

void DeathActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
	CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
	EngineCameras.blendInCamera(playerCameraEntity, 0.00001f, CModuleCameras::EPriority::GAMEPLAY);
	getPlayerModel()->resetGravity();
	TCompRender* render = getPlayerModel()->get<TCompRender>();
	render->selfIllumRatio = 1.f;
	//Engine.getEntities().setManagerUpdate("skeleton", true);
}

void DeathActionState::respawn() {
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

	getPlayerModel()->resetHp();
	getPowerGauge()->resetPower();
}

void DeathActionState::copyCam() {
	//presupone que entramos siempre en player camera
	CEntity* copyCamEntity = getEntityByName(PLAYER_CAMERA);
	CEntity* pasteCamEntity = getEntityByName(TELEPORT_CAMERA);

	TCompTransform* copyCamTransform = copyCamEntity->get<TCompTransform>();
	TCompTransform* pasteCamTransform = pasteCamEntity->get<TCompTransform>();
	pasteCamTransform->setPosition(copyCamTransform->getPosition());
	pasteCamTransform->setRotation(copyCamTransform->getRotation());
}


