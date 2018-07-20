#include "mcv_platform.h"
#include "comp_self_illum_variation.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "components/comp_render.h"

DECL_OBJ_MANAGER("self_illum_variation", TCompSelfIllumVariation);

void TCompSelfIllumVariation::debugInMenu() {
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::DragFloat("Max Ratio", &maxRatio, 0.1f, 0, 20.f);
	ImGui::DragFloat("Half Amplitude", &halfAmplitude, 0.01f, 0, 10.f);
	halfAmplitude = clamp(halfAmplitude, 0.f, maxRatio/2);
	ImGui::DragFloat("Time Factor", &timeFactor, 0.1f, 0, 20.f);
}

void TCompSelfIllumVariation::registerMsgs() {
	DECL_MSG(TCompSelfIllumVariation, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompSelfIllumVariation::load(const json& j, TEntityParseContext& ctx) {
	enabled = j.value("enabled", true);
	maxRatio = j.value("max_ratio", 1.f);
	halfAmplitude = j.value("half_amplitude", 0.15f);
	halfAmplitude = clamp(halfAmplitude, 0.f, maxRatio / 2);
	timeFactor = j.value("time_factor", 3.3f);
}

void TCompSelfIllumVariation::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	timer.reset();
}

void TCompSelfIllumVariation::update(float delta) {
	TCompRender* render = get<TCompRender>();
	if (render) {
		if (enabled) {
			render->selfIllumRatio = (maxRatio - halfAmplitude) + halfAmplitude * sin(timer.elapsed() * timeFactor);
		}
	}
}

void TCompSelfIllumVariation::setEnabled(bool enable, float stopRatio) {
	enabled = false;
	TCompRender* render = get<TCompRender>();
	if (render) {
		if (stopRatio > 0) {
			render->selfIllumRatio = maxRatio * stopRatio;
		}
	}
}
