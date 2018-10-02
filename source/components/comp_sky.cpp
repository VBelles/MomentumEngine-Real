#include "mcv_platform.h"
#include "comp_sky.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "components/comp_light_dir.h"

DECL_OBJ_MANAGER("sky", TCompSky);

void TCompSky::debugInMenu() {
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
		setSkybox(static_cast<SkyboxType>(skyboxToChange), 10.f);
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

	if (ImGui::Checkbox("Enabled", &enabled)) {
		if (enabled) {
			changeTimer.setElapsed(elapsedAtPauseChange);
			lerpTimer.setElapsed(elapsedAtPauseLerp);
		}
		else {
			elapsedAtPauseChange = changeTimer.elapsed();
			elapsedAtPauseLerp = lerpTimer.elapsed();
		}
	}
}

void TCompSky::setEnabled(bool enabled) {
	if (this->enabled != enabled) {
		this->enabled = enabled;
		if (enabled) {
			changeTimer.setElapsed(elapsedAtPauseChange);
			lerpTimer.setElapsed(elapsedAtPauseLerp);
		}
		else {
			elapsedAtPauseChange = changeTimer.elapsed();
			elapsedAtPauseLerp = lerpTimer.elapsed();
		}
	}
}

void TCompSky::load(const json& j, TEntityParseContext& ctx) {
	for (auto& skybox : j["skyboxes"]) {
		std::string texture = skybox.value("texture", "");
		float time = skybox.value("time", 100.f);
		VEC4 color = loadVEC4(skybox.value("light_color", "1 1 1 1"));
		float intensity = skybox.value("light_intensity", 10.f);
		float emissive = skybox.value("emissive_ratio", 0.f);
		float exposure = skybox.value("exposure", 1.f);
		dbg("texture: \n", texture.c_str());
		skyboxes.push_back(Skybox{ Resources.get(texture)->as<CTexture>(), time, color, intensity, emissive, exposure });
	}
	fixedLerpTime = j.value("lerp_time", 10.f);
	currentLerpTime = fixedLerpTime;
	lightSourceName = j.value("light_source", "");
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
	cb_globals.day_night_cycle_emissive_ratio = skyboxes[skyboxIndex].emissiveRatio;
	changeTimer.reset();
	lerpTimer.reset();
	//pillar directional light
	CHandle lightSourceEntityHandle = getEntityByName(lightSourceName);
	if (!lightSourceEntityHandle.isValid()) {
		dbg("-----------------Falta un light source para el skybox\n");
	}
	else {
		CEntity* entity = lightSourceEntityHandle;
		lightSource = entity->get<TCompLightDir>();
	}
}


void TCompSky::update(float dt) {
	if (enabled) {
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
				previousSkyboxIndex = skyboxIndex;
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
				previousSkyboxIndex = skyboxIndex;
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
				previousSkyboxIndex = skyboxIndex;
				skyboxIndex = possibleIndexes[std::rand() % (possibleIndexes.size())];
				skyboxes[skyboxIndex].texture->activate(TS_ENVIRONMENT_MAP_1);
				cb_globals.global_skybox_ratio = 0.f;
			}
			break;
		}

		TCompLightDir* light = lightSource;
		if (lerpTimer.elapsed() <= currentLerpTime) {
			cb_globals.global_skybox_ratio = lerpTimer.elapsed() / currentLerpTime;
		}
		else {
			cb_globals.global_skybox_ratio = 1.f;
		}
		//intensidad de la luz y color de la luz respecto este ratio
		light->setColor(VEC4::Lerp(skyboxes[previousSkyboxIndex].lightColor, skyboxes[skyboxIndex].lightColor, cb_globals.global_skybox_ratio));
		light->setIntensity(lerp(skyboxes[previousSkyboxIndex].lightIntensity, skyboxes[skyboxIndex].lightIntensity, cb_globals.global_skybox_ratio));
		cb_globals.day_night_cycle_emissive_ratio = lerp(skyboxes[previousSkyboxIndex].emissiveRatio, skyboxes[skyboxIndex].emissiveRatio, cb_globals.global_skybox_ratio);
		cb_globals.global_exposure_adjustment = lerp(skyboxes[previousSkyboxIndex].exposure, skyboxes[skyboxIndex].exposure, cb_globals.global_skybox_ratio);
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
