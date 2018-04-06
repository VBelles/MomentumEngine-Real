#include "mcv_platform.h"
#include "comp_mix_camera_trigger.h"
#include "game_constants.h"
#include "comp_transform.h"

DECL_OBJ_MANAGER("mixCameraTrigger", TCompMixCameraTrigger);

void TCompMixCameraTrigger::debugInMenu() {
}

void TCompMixCameraTrigger::load(const json& j, TEntityParseContext& ctx) {
	assert(j.count("cameraToMix"));
	cameraToMix = j.value("cameraToMix", "player_camera");
	timeToMixIn = j.value("timeToMixIn", 1.f);
	timeToMixOut = j.value("timeToMixOut", 1.f);
	modifyPlayerCameraRotation = j.value("modifyPlayerCameraRotation", true);
}

void TCompMixCameraTrigger::registerMsgs() {
	DECL_MSG(TCompMixCameraTrigger, TMsgEntityCreated, onCreate);
	DECL_MSG(TCompMixCameraTrigger, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompMixCameraTrigger, TMsgTriggerExit, onTriggerExit);
}

void TCompMixCameraTrigger::onCreate(const TMsgEntityCreated& msg) {

}

void TCompMixCameraTrigger::onTriggerEnter(const TMsgTriggerEnter & msg) {
	CEntity* entity = msg.h_other_entity;	
	if (strcmp(PLAYER_NAME, entity->getName()) == 0) {
		CHandle h_camera = getEntityByName(cameraToMix);
		CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
		playerCameraEntity->sendMsg(TMsgLockCameraInput{ true });
		Engine.getCameras().blendInCamera(h_camera, timeToMixIn, CModuleCameras::EPriority::GAMEPLAY);
	}
}

void TCompMixCameraTrigger::onTriggerExit(const TMsgTriggerExit & msg) {
	CEntity* entity = msg.h_other_entity;
	std::string playerName = PLAYER_NAME;
	if (strcmp(PLAYER_NAME, entity->getName()) == 0) {
		if (modifyPlayerCameraRotation) {
			CopyRotationFromMixedCameraToPlayerCamera();
		}
		CHandle playerCameraHandle = getEntityByName(PLAYER_CAMERA);
		Engine.getCameras().blendInCamera(playerCameraHandle, timeToMixOut, CModuleCameras::EPriority::GAMEPLAY);
	}
}

void TCompMixCameraTrigger::CopyRotationFromMixedCameraToPlayerCamera() {
	CHandle leavingCameraHandle = getEntityByName(cameraToMix);
	CEntity* leavingCameraEntity = leavingCameraHandle;
	TCompTransform* leavingCameraTransform = leavingCameraEntity->get<TCompTransform>();
	CHandle playerCameraHandle = getEntityByName(PLAYER_CAMERA);
	CEntity* playerCameraEntity = playerCameraHandle;
	TCompTransform* playerCameraTransform = playerCameraEntity->get<TCompTransform>();
	//playerCameraTransform->setPosition(leavingCameraTransform->getPosition());
	float yPLayer, pPlayer, rPlayer;
	playerCameraTransform->getYawPitchRoll(&yPLayer, &pPlayer, &rPlayer);
	float yLeavingCamera, pLeavingCamera, rLeavingCamera;
	leavingCameraTransform->getYawPitchRoll(&yLeavingCamera, &pLeavingCamera, &rLeavingCamera);
	playerCameraTransform->setYawPitchRoll(yLeavingCamera, pLeavingCamera, rPlayer);
}


