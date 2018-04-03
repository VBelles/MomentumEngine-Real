#include "mcv_platform.h"
#include "comp_power_gauge.h"
#include "utils/utils.h"
#include "entity/common_msgs.h"
#include "comp_player_model.h"
#include "components/comp_tags.h"

DECL_OBJ_MANAGER("power_gauge", TCompPowerGauge);

void TCompPowerGauge::registerMsgs() {
}

void TCompPowerGauge::load(const json & j, TEntityParseContext & ctx) {
	powerLevel = j.value("power_level", 1);
	maxPower = j.value("max_power", 30000.f);
	powerPerLevel = j.value("power_per_level", 10000.f);
	dropSpeed = j.value("drop_speed", 5.f * 60.f);
}

void TCompPowerGauge::update(float delta) {
	IncreasePower(-dropSpeed * delta);
}

void TCompPowerGauge::ReleasePower() {
	IncreasePower(-powerPerLevel);
}

void TCompPowerGauge::GainPower() {
	IncreasePower(powerPerLevel);
}

void TCompPowerGauge::ResetPower() {
	IncreasePower(-maxPower);
}

void TCompPowerGauge::IncreasePower(float increment) {
	//Increase power
	float lastPower = power;
	power += increment;
	power = clamp(power, 0.f, maxPower);

	//Calculate power level
	int lastPowerLevel = powerLevel;
	powerLevel = power < powerPerLevel ? 1 : power < powerPerLevel * 2 ? 2 : 3;

	//Notify player_comp when power level changed
	if (lastPowerLevel != powerLevel) {
		CHandle(this).getOwner().sendMsg(TMsgPowerLvlChange{ this, powerLevel });
		// Inform 'pure' entities of the change.		
	}

	bool lastIsPure = isPure;
	isPure = power == 0.f;
	//Notify pure entities when purity changed
	if (lastIsPure != isPure) {
		TMsgPurityChange msg = TMsgPurityChange{ this, isPure };
		auto& pureEntitiesHandles = CTagsManager::get().getAllEntitiesByTag(getID("pure"));
		for (auto handle : pureEntitiesHandles) {
			CEntity* e = handle;
			if (e) e->sendMsg(msg);
		}
	}

	
	
}


