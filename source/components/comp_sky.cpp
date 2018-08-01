#include "mcv_platform.h"
#include "comp_sky.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("sky", TCompSky);

void TCompSky::debugInMenu() {
}

void TCompSky::load(const json& j, TEntityParseContext& ctx) {
	for (auto& skyBox : j["skyboxes"]) {
		std::string texture = skyBox.value("texture","");
		float time = skyBox.value("time", 100);
		dbg("texture: \n", texture.c_str());
		skyboxes.push_back(Skybox{ Resources.get(texture)->as<CTexture>(), time });
	}
	fixedLerpTime = j.value("lerp_time", 10);
	currentLerpTime = fixedLerpTime;

	numSkyboxes = skyboxes.size();
}

void TCompSky::registerMsgs() {
	DECL_MSG(TCompSky, TMsgAllScenesCreated, onAllScenesCreated);
}


void TCompSky::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	//textura 0 en ambos slots
	skyboxes[skyboxIndex].texture->activate(TS_ENVIRONMENT_MAP);
	skyboxes[skyboxIndex].texture->activate(TS_ENVIRONMENT_MAP_1);
	cb_globals.global_skybox_ratio = 0.f;
	changeTimer.reset();
	lerpTimer.reset();
}


void TCompSky::update(float dt) {
	if (changeTimer.elapsed() >= skyboxes[skyboxIndex].duration) {
		changeTimer.reset();
		lerpTimer.reset();
		//slot 0 -> textura actual, slot 1 -> textura siguiente
		skyboxes[skyboxIndex].texture->activate(TS_ENVIRONMENT_MAP);
		skyboxIndex = (skyboxIndex + 1) % numSkyboxes;
		skyboxes[skyboxIndex].texture->activate(TS_ENVIRONMENT_MAP_1);
		cb_globals.global_skybox_ratio = 0.f;
	}
	if (lerpTimer.elapsed() <= currentLerpTime) {
		cb_globals.global_skybox_ratio = lerpTimer.elapsed() / currentLerpTime;
	}
	else {
		cb_globals.global_skybox_ratio = 1.f;
	}
}
