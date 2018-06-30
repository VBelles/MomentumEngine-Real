#include "mcv_platform.h"
#include "DeathActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/comp_power_gauge.h"
#include "components/player/states/StateManager.h"
#include "components/comp_transform.h"
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
}

void DeathActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState); 
	getSkeleton()->blendCycle(animationLoop, 0.0f, 0.0f);
	getSkeleton()->executeAction(animation, 0.1f, 0.0f);
	timer.reset();
	finish = false;
	EngineScripting.throwEvent(onPlayerKilled, "");

	copyCam();
	CEntity* teleportCameraEntity = getEntityByName(TELEPORT_CAMERA);
	EngineCameras.blendInCamera(teleportCameraEntity, 0.001f, CModuleCameras::EPriority::GAMEPLAY);
}

void DeathActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
	CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
	EngineCameras.blendInCamera(playerCameraEntity, 0.00001f, CModuleCameras::EPriority::GAMEPLAY);
	//Engine.getEntities().setManagerUpdate("skeleton", true);
}

void DeathActionState::respawn() {
	*velocityVector = VEC3();
	assert(getPlayerModel());
	VEC3 respawnPosition = getPlayerModel()->getRespawnPosition();
	getCollider()->controller->setFootPosition(PxExtendedVec3(respawnPosition.x, respawnPosition.y, respawnPosition.z));
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


