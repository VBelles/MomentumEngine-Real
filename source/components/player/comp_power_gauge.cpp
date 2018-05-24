#include "mcv_platform.h"
#include "comp_power_gauge.h"
#include "utils/utils.h"
#include "entity/common_msgs.h"
#include "comp_player_model.h"
#include "components/comp_tags.h"
#include "components/comp_collider.h"
#include "components/comp_purity.h"

DECL_OBJ_MANAGER("power_gauge", TCompPowerGauge);


void TCompPowerGauge::registerMsgs() {
}

void TCompPowerGauge::load(const json & j, TEntityParseContext & ctx) {
	powerLevel = j.value("power_level", 1);
	//powerPerLevel = j.value("power_per_level", 10000.f);
	maxPower = 0.f;
	if (j.count("power_per_level")) {
		json jPowerPerLevel = j["power_per_level"];
		for (int i = 0; i < NUMBER_OF_POWER_LEVELS; i++) {
			powerPerLevel[i] = jPowerPerLevel[i];
			maxPower += powerPerLevel[i];
		}
	}
	//dropSpeed = j.value("drop_speed", 5.f * 60.f);
	if (j.count("drop_speed")) {
		json jDropSpeed = j["drop_speed"];
		for (int i = 0; i < NUMBER_OF_POWER_LEVELS; i++) {
			dropSpeed[i] = jDropSpeed[i];
		}
	}
	freezeDropTime = j.value("freezeDropTime", 1.5f);
}

void TCompPowerGauge::update(float delta) {
	if (freezeDropTimer.elapsed() > freezeDropTime) {
		increasePower(-dropSpeed[powerLevel - 1] * delta);
	}
}

void TCompPowerGauge::releasePower() {
	float barPercentage = getPowerLevelPercentage();
	float powerToDecrease = barPercentage * powerPerLevel[powerLevel - 1];
	if (powerLevel != 0) {
		powerToDecrease += (1 - barPercentage) * powerPerLevel[powerLevel - 2];
	}
	increasePower(-powerToDecrease);
}

void TCompPowerGauge::gainPower() {
	increasePower(powerPerLevel[powerLevel - 1]);
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

	if (power <= powerPerLevel[0]) {
		powerLevel = 1;
	}
	else if (power <= powerPerLevel[0] + powerPerLevel[1]) {
		powerLevel = 2;
	}
	else {
		powerLevel = 3;
	}

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
		getObjectManager<TCompPurity>()->forEach([&msg](CHandle purity) { //Send message to entities with purity
			((CEntity*)purity.getOwner())->sendMsg(msg);
		});
	}
	EngineGUI.getVariables().getVariant("power_progress")->setFloat(power / maxPower);
}

void TCompPowerGauge::setPower(float power) {
	increasePower(power - this->power);
}

int TCompPowerGauge::getPowerLevel() { return powerLevel; }

float TCompPowerGauge::getPower() { return power; }

float TCompPowerGauge::getMaxPower() { return maxPower; }

float TCompPowerGauge::getBarPercentage() {
	float percentage = getPowerLevelPercentage() / NUMBER_OF_POWER_LEVELS;
	percentage += (float)(powerLevel - 1) / NUMBER_OF_POWER_LEVELS;
	return percentage;
}

float TCompPowerGauge::getPowerLevelPercentage() {
	float percentage = power;
	for (int i = 0; i < powerLevel - 1; i++) {
		percentage -= powerPerLevel[i];
	}
	percentage /= powerPerLevel[powerLevel - 1];

	return percentage;
}

