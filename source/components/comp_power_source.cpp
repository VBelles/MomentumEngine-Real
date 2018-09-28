#include "mcv_platform.h"
#include "comp_power_source.h"
#include "entity/common_msgs.h"
#include "components/player/comp_power_gauge.h"
#include "modules/system_modules/module_uniques.h"
#include "modules/system_modules/module_gui.h"
#include "gui/widgets/gui_map_marker.h"

DECL_OBJ_MANAGER("power_source", TCompPowerSource);

void TCompPowerSource::debugInMenu() {
	ImGui::DragFloat("Power Per Cycle", &powerPerCycle, 50, 0, 100000);
	ImGui::DragFloat("Cycle Time", &cycleTime, 0.01, 0, 1);
	ImGui::DragFloat("Time To Increase Power", &timeToIncreasePower, 0.01, 0, 5);
	ImGui::Checkbox("Is Player Inside", &isPlayerInside);
}

void TCompPowerSource::registerMsgs() {
	DECL_MSG(TCompPowerSource, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompPowerSource, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompPowerSource, TMsgTriggerExit, onTriggerExit);
}

void TCompPowerSource::update(float delta) {
	if (isPlayerInside) {
		if (timer.elapsed() >= cycleTime) {
			timer.reset();
			getPowerGauge()->increasePowerInTime(powerPerCycle, timeToIncreasePower);
		}
	}
}

void TCompPowerSource::load(const json& j, TEntityParseContext& ctx) {
	powerPerCycle = j.value("power_per_cycle", powerPerCycle);
	cycleTime = j.value("cycle_time", cycleTime);
	timeToIncreasePower = j.value("time_to_increase_power", timeToIncreasePower);
}

void TCompPowerSource::onTriggerEnter(const TMsgTriggerEnter& msg) {
	CEntity* entity = msg.h_other_entity;
	if (!isPlayerInside && entity->getName() == PLAYER_NAME) {
		timer.setTimeStamp(0);
		isPlayerInside = true;
	}
}

void TCompPowerSource::onTriggerExit(const TMsgTriggerExit & msg) {
	CEntity* entity = msg.h_other_entity;
	if (isPlayerInside && entity->getName() == PLAYER_NAME) {
		isPlayerInside = false;
	}
}

void TCompPowerSource::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	CEntity* player = getEntityByName(PLAYER_NAME);
	powerGaugeHandle = player->get<TCompPowerGauge>();
}

TCompPowerGauge * TCompPowerSource::getPowerGauge() {
	return powerGaugeHandle;
}
