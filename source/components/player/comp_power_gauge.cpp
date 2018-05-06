#include "mcv_platform.h"
#include "comp_power_gauge.h"
#include "utils/utils.h"
#include "entity/common_msgs.h"
#include "comp_player_model.h"
#include "components/comp_tags.h"
#include "components/comp_collider.h"

DECL_OBJ_MANAGER("power_gauge", TCompPowerGauge);

void TCompPowerGauge::registerMsgs() {
}

void TCompPowerGauge::load(const json & j, TEntityParseContext & ctx) {
	powerLevel = j.value("power_level", 1);
	maxPower = j.value("max_power", 30000.f);
	powerPerLevel = j.value("power_per_level", 10000.f);
	dropSpeed = j.value("drop_speed", 5.f * 60.f);
	freezeDropTime = j.value("freezeDropTime", 1.5f);
}

void TCompPowerGauge::update(float delta) {
	if (freezeDropTimer.elapsed() > freezeDropTime) {
		increasePower(-dropSpeed * delta);
	}
}

void TCompPowerGauge::releasePower() {
	increasePower(-powerPerLevel);
}

void TCompPowerGauge::gainPower() {
	increasePower(powerPerLevel);
}

void TCompPowerGauge::resetPower() {
	increasePower(-maxPower);
}

void TCompPowerGauge::increasePower(float increment) {
	if (increment > 0) {
		freezeDropTimer.reset();
	}
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
		((CEntity*)CHandle(this).getOwner())->sendMsg(msg); //Send message to my entity
		auto& pureEntitiesHandles = CTagsManager::get().getAllEntitiesByTag(getID("pure"));
		for (auto handle : pureEntitiesHandles) {
			CEntity* e = handle;
			if (e) e->sendMsg(msg);
		}
	}
}

void TCompPowerGauge::setPower(float power) {
	increasePower(power - this->power);
}

int TCompPowerGauge::getPowerLevel() { return powerLevel; }

float TCompPowerGauge::getPower() { return power; }

float TCompPowerGauge::getMaxPower() { return maxPower; }


