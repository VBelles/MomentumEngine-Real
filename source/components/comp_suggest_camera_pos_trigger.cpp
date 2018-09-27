#include "mcv_platform.h"
#include "comp_suggest_camera_pos_trigger.h"
#include "game_constants.h"
#include "components/controllers/comp_camera_player.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("suggestCameraTrigger", TCompSuggestCameraPosTrigger);

void TCompSuggestCameraPosTrigger::debugInMenu() {
}

void TCompSuggestCameraPosTrigger::load(const json& j, TEntityParseContext& ctx) {
	assert(j.count("cameraSuggestion"));
	cameraSuggestion = j.value("cameraSuggestion", "player_camera");
	suggestYaw = j.value("suggestYaw", false);
	hasOppositeYaw = j.value("hasOppositeYaw", false);
	suggestPitch = j.value("suggestPitch", false);
	forceDistance = j.value("forceDistance", false);
	distanceToCamera = j.value("distanceToCamera", 5.5f);
	changeCenteringCamera = j.value("changeCenteringCamera", false);
	forcePositionOnEnter = j.value("forcePositionOnEnter", true);
	if (j.count("centeringCameraSpeed")) {
		centeringCameraSpeed = loadVEC2(j["centeringCameraSpeed"]);
	}
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
	if (PLAYER_NAME == entity->getName()) {
		CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
		TCompCameraPlayer* cameraPlayer = playerCameraEntity->get<TCompCameraPlayer>();
		//Llamar a funciones de player camera para cambiar yaw, pitch, distancia o si fuerza el centrar cámara
		cameraPlayer->suggestYawPitchDistance(yawFromCamera, pitchFromCamera, distanceToCamera, suggestYaw, hasOppositeYaw, suggestPitch, forceDistance, changeCenteringCamera);
		if (forcePositionOnEnter) {
			cameraPlayer->placeCameraOnSuggestedPosition(centeringCameraSpeed);
		}
	}
}

void TCompSuggestCameraPosTrigger::onTriggerExit(const TMsgTriggerExit & msg) {
	CEntity* entity = msg.h_other_entity;
	if (PLAYER_NAME == entity->getName()) {
		CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
		TCompCameraPlayer* cameraPlayer = playerCameraEntity->get<TCompCameraPlayer>();
		//Resetear valores por defecto de cámara player
		cameraPlayer->resetSuggested();
	}
}


