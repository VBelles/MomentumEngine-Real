#include "mcv_platform.h"
#include "comp_light_flicker.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "components/comp_light_point.h"

DECL_OBJ_MANAGER("light_flicker", TCompLightFlicker);

void TCompLightFlicker::debugInMenu() {
	ImGui::DragFloat("intensityVariation", &intensityVariation, 0.001f, 0.f, 1.f);
	ImGui::DragFloat("variationSpeed", &variationSpeed, 0.01f, 0.f, 100.f);
}

void TCompLightFlicker::registerMsgs() {
	DECL_MSG(TCompLightFlicker, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompLightFlicker::load(const json& j, TEntityParseContext& ctx) {
	intensityVariation = j.value("intensityVariation", 1.0f);
	variationSpeed = j.value("variationSpeed", 1.0f);
}

void TCompLightFlicker::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	lightHandle = get<TCompLightPoint>();
	TCompLightPoint* light = lightHandle;
	originalIntensity = light->getIntensity();
	previousIntensity = originalIntensity - originalIntensity * intensityVariation;
	nextIntensity = originalIntensity + originalIntensity * intensityVariation;
	ratio = 0.0f;
}

void TCompLightFlicker::update(float dt) {
	TCompLightPoint* light = lightHandle;
	light->setIntensity(lerp(previousIntensity, nextIntensity, ratio));
	ratio += variationSpeed * dt * ((double)rand() / (RAND_MAX)) * 2;
	if (ratio >= 1.0) {
		ratio = 0;
		float aux = previousIntensity;
		previousIntensity = nextIntensity;
		if (aux < nextIntensity) {
			nextIntensity = originalIntensity - originalIntensity * intensityVariation;
		}
		else {
			nextIntensity = originalIntensity + originalIntensity * intensityVariation;
		}
	}
}
