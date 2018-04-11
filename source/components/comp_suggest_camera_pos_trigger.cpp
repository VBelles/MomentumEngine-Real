#include "mcv_platform.h"
#include "comp_suggest_camera_pos_trigger.h"
#include "game_constants.h"
#include "comp_transform.h"
#include "comp_camera.h"
#include "components/comp_collider.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("suggestCameraTrigger", TCompSuggestCameraPosTrigger);

void TCompSuggestCameraPosTrigger::debugInMenu() {
}

void TCompSuggestCameraPosTrigger::load(const json& j, TEntityParseContext& ctx) {
	assert(j.count("cameraToMix"));
	cameraToMix = j.value("cameraToMix", "player_camera");
	timeToMixIn = j.value("timeToMixIn", 1.f);
	timeToMixOut = j.value("timeToMixOut", 1.f);
	modifyPlayerCameraRotation = j.value("modifyPlayerCameraRotation", true);
}

void TCompSuggestCameraPosTrigger::registerMsgs() {
	DECL_MSG(TCompSuggestCameraPosTrigger, TMsgEntityCreated, onCreate);
	DECL_MSG(TCompSuggestCameraPosTrigger, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompSuggestCameraPosTrigger, TMsgTriggerExit, onTriggerExit);
}

void TCompSuggestCameraPosTrigger::onCreate(const TMsgEntityCreated& msg) {

}

void TCompSuggestCameraPosTrigger::onTriggerEnter(const TMsgTriggerEnter & msg) {
	CEntity* entity = msg.h_other_entity;	
	if (strcmp(PLAYER_NAME, entity->getName()) == 0) {
		CHandle h_camera = getEntityByName(cameraToMix);
		CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
		playerCameraEntity->sendMsg(TMsgLockCameraInput{ true });
	}
}

void TCompSuggestCameraPosTrigger::onTriggerExit(const TMsgTriggerExit & msg) {
	CEntity* entity = msg.h_other_entity;
	std::string playerName = PLAYER_NAME;
	if (strcmp(PLAYER_NAME, entity->getName()) == 0) {
		CHandle playerCameraHandle = getEntityByName(PLAYER_CAMERA);
		if (!Engine.getCameras().IsCameraAloneInMix(playerCameraHandle)) {
			if (modifyPlayerCameraRotation) {
				CEntity* cameraToMixEntity = getEntityByName(cameraToMix);
				cameraToMixEntity->sendMsg(TMsgLockCameraInput{ true }); 
			}
		}
	}
}


