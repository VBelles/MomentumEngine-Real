#include "mcv_platform.h"
#include "comp_suggest_camera_pos_trigger.h"
#include "game_constants.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "components/controllers/comp_camera_player.h"
#include "components/comp_collider.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("suggestCameraTrigger", TCompSuggestCameraPosTrigger);

void TCompSuggestCameraPosTrigger::debugInMenu() {
}

void TCompSuggestCameraPosTrigger::load(const json& j, TEntityParseContext& ctx) {
	assert(j.count("cameraSuggestion"));
	cameraSuggestion = j.value("cameraSuggestion", "player_camera");
	suggestYaw = j.value("suggestYaw", false);
	suggestPitch = j.value("suggestPitch", false);
	forceDistance = j.value("forceDistance", false);
	distanceToCamera = j.value("distanceToCamera", 5.5f);
	changeCenteringCamera = j.value("changeCenteringCamera", false);
}

void TCompSuggestCameraPosTrigger::registerMsgs() {
	DECL_MSG(TCompSuggestCameraPosTrigger, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompSuggestCameraPosTrigger, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompSuggestCameraPosTrigger, TMsgTriggerExit, onTriggerExit);
}

void TCompSuggestCameraPosTrigger::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	CEntity* entity = getEntityByName(cameraSuggestion);
	TCompTransform* camera = entity->get<TCompTransform>();
	float roll;
	camera->getYawPitchRoll(&yawFromCamera, &pitchFromCamera, &roll);
}

void TCompSuggestCameraPosTrigger::onTriggerEnter(const TMsgTriggerEnter & msg) {
	CEntity* entity = msg.h_other_entity;	
	if (strcmp(PLAYER_NAME, entity->getName()) == 0) {
		CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
		TCompCameraPlayer* cameraPlayer = playerCameraEntity->get<TCompCameraPlayer>();
		//Llamar a funciones de player camera para cambiar yaw, pitch, distancia o si fuerza el centrar cámara
		cameraPlayer->suggestYawPitchDistance(yawFromCamera, pitchFromCamera, distanceToCamera, suggestYaw, suggestPitch, forceDistance, changeCenteringCamera);
		cameraPlayer->placeCameraOnSuggestedPosition();
	}
}

void TCompSuggestCameraPosTrigger::onTriggerExit(const TMsgTriggerExit & msg) {
	CEntity* entity = msg.h_other_entity;
	if (strcmp(PLAYER_NAME, entity->getName()) == 0) {
		CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
		TCompCameraPlayer* cameraPlayer = playerCameraEntity->get<TCompCameraPlayer>();
		//Resetear valores por defecto de cámara player
		cameraPlayer->resetSuggested();
	}
}


