#include "mcv_platform.h"
#include "comp_sky.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("sky", TCompSky);

void TCompSky::debugInMenu() {
	//bool fixed = false;
	//ImGui::Checkbox("Fix sky", &fixed);
	//if (fixed) {
	//	setSkybox(REGULAR, 5.f);
	//}
	int skyboxToChange = static_cast<int>(skyboxIndex);
	static const char* skyboxToChange_str =
		"REGULAR\0"
		"INVERTED\0"
		"PINK\0"
		"RED\0"
		"TURQUESA\0"
		"\0";
	ImGui::Combo("Skybox", &skyboxToChange, skyboxToChange_str);
	if (skyboxToChange != skyboxIndex) {
		setSkybox(static_cast<SkyboxType>(skyboxToChange), 5.f);
	}

	bool sequential = false;
	ImGui::Checkbox("Sequential sky", &sequential);
	if (sequential) {
		setToSequential();
	}
	bool random = false;
	ImGui::Checkbox("Random sky", &random);
	if (random) {
		setToRandom();
	}
}

void TCompSky::load(const json& j, TEntityParseContext& ctx) {
	for (auto& skyBox : j["skyboxes"]) {
		std::string texture = skyBox.value("texture", "");
		float time = skyBox.value("time", 100.f);
		dbg("texture: \n", texture.c_str());
		skyboxes.push_back(Skybox{ Resources.get(texture)->as<CTexture>(), time });
	}
	fixedLerpTime = j.value("lerp_time", 10.f);
	currentLerpTime = fixedLerpTime;

	numSkyboxes = skyboxes.size();
	if (numSkyboxes == 0) {
		skyboxes.push_back(Skybox{ Resources.get("data/textures/cubemaps/momentum/OutputCube_256.dds")->as<CTexture>(), 20.f });
	}
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
	if (numSkyboxes < 2) return;
	switch (currentMode) {
	case SEQUENTIAL:
		if (waitingToEnter) {
			if (cb_globals.global_skybox_ratio == 1.f) {
				waitingToEnter = false;
				changeTimer.reset();
			}
		}
		else if (changeTimer.elapsed() >= skyboxes[skyboxIndex].duration) {
			changeTimer.reset();
			lerpTimer.reset();
			currentLerpTime = fixedLerpTime;
			//slot 0 -> textura actual, slot 1 -> textura siguiente
			skyboxes[skyboxIndex].texture->activate(TS_ENVIRONMENT_MAP);
			skyboxIndex = (skyboxIndex + 1) % numSkyboxes;
			skyboxes[skyboxIndex].texture->activate(TS_ENVIRONMENT_MAP_1);
			cb_globals.global_skybox_ratio = 0.f;
		}
		break;
	case FIXED:
		if (waitingToEnter && cb_globals.global_skybox_ratio == 1.f) {
			waitingToEnter = false;
			lerpTimer.reset();
			cb_globals.global_skybox_ratio = 0.f;
			currentLerpTime = nextLerpTime;
			skyboxes[skyboxIndex].texture->activate(TS_ENVIRONMENT_MAP);
			skyboxIndex = nextSkybox;
			skyboxes[skyboxIndex].texture->activate(TS_ENVIRONMENT_MAP_1);
		}
		break;
	case RANDOM:
		if (waitingToEnter) {
			if (cb_globals.global_skybox_ratio == 1.f) {
				waitingToEnter = false;
				changeTimer.reset();
			}
		}
		else if (changeTimer.elapsed() >= skyboxes[skyboxIndex].duration) {
			changeTimer.reset();
			lerpTimer.reset();
			currentLerpTime = fixedLerpTime;
			//slot 0 -> textura actual, slot 1 -> textura siguiente
			skyboxes[skyboxIndex].texture->activate(TS_ENVIRONMENT_MAP);
			std::vector<int> possibleIndexes;
			for (int i = 0; i < numSkyboxes; i++) {
				if (i != skyboxIndex) {
					possibleIndexes.push_back(i);
				}
			}
			skyboxIndex = possibleIndexes[std::rand() % (possibleIndexes.size())];
			skyboxes[skyboxIndex].texture->activate(TS_ENVIRONMENT_MAP_1);
			cb_globals.global_skybox_ratio = 0.f;
		}
		break;
	}
	if (lerpTimer.elapsed() <= currentLerpTime) {
		cb_globals.global_skybox_ratio = lerpTimer.elapsed() / currentLerpTime;
	}
	else {
		cb_globals.global_skybox_ratio = 1.f;
	}
}

void TCompSky::setSkybox(SkyboxType type, float lerpTime) {
	currentMode = FIXED;
	nextSkybox = type;
	nextLerpTime = lerpTime;
	waitingToEnter = true;
}

void TCompSky::setToSequential() {
	currentMode = SEQUENTIAL;
	waitingToEnter = true;
}

void TCompSky::setToRandom() {
	currentMode = RANDOM;
	waitingToEnter = true;
}
