#include "mcv_platform.h"
#include "comp_day_night_cycle.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "components/comp_sky.h"

DECL_OBJ_MANAGER("day_night_cycle", TCompDayNightCycle);

void TCompDayNightCycle::debugInMenu() {
	if (ImGui::Checkbox("Enabled", &enabled)) {
		cycleTimer.setPaused(!enabled);
	}
}

void TCompDayNightCycle::setEnabled(bool enabled) {
	this->enabled = enabled;
	cycleTimer.setPaused(!enabled);
}

void TCompDayNightCycle::load(const json& j, TEntityParseContext& ctx) {
	dayPeriodIndex = j.value("starting_period", 1);
	for (float duration : j["durations"]) {
		durations.push_back(duration);
	}
	lerpTime = j.value("lerp_time", lerpTime);
	skyName = j.value("sky_name", "");
}

void TCompDayNightCycle::registerMsgs() {
	DECL_MSG(TCompDayNightCycle, TMsgAllScenesCreated, onAllScenesCreated);
}


void TCompDayNightCycle::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	CEntity* skyEntity = getEntityByName(skyName);
	skyHandle = skyEntity->get<TCompSky>();
	cycleTimer.reset();
}


void TCompDayNightCycle::update(float dt) {
	if (enabled) {
		if (cycleTimer.elapsed() >= durations[dayPeriodIndex]) {
			TCompSky* sky = skyHandle;
			cycleTimer.reset();
			dayPeriodIndex = (dayPeriodIndex + 1) % durations.size();

			switch (dayPeriodIndex) {
			case 0:
				sky->setSkybox(SkyboxType::PINK, lerpTime);
				break;
			case 1:
				sky->setSkybox(SkyboxType::REGULAR, lerpTime);
				break;
			case 2:
				sky->setSkybox(SkyboxType::PINK, lerpTime);
				break;
			case 3:
				sky->setSkybox(SkyboxType::INVERTED, lerpTime);
				break;
			default:
				sky->setSkybox(SkyboxType::REGULAR, lerpTime);
				break;
			}
		}
	}
}