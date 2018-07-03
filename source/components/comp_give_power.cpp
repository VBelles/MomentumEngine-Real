#include "mcv_platform.h"
#include "comp_give_power.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("givePower", TCompGivePower);

void TCompGivePower::registerMsgs() {
	DECL_MSG(TCompGivePower, TMsgGetPower, onGetPower);
}

void TCompGivePower::load(const json& j, TEntityParseContext& ctx) {
	maxPowerToGive = j.value("maxPowerToGive", 5000.0f);
	powerToGive = maxPowerToGive;
	baseMultiplier = j.value("baseMultiplier", 1.0f);
	resetTime = j.value("resetTime", 120.0f);
}

void TCompGivePower::update(float dt) {
	if (resetTimer.elapsed() >= resetTime) {
		powerToGive = maxPowerToGive;
		resetTimer.reset();
	}
}

void TCompGivePower::debugInMenu() {

}

void TCompGivePower::onGetPower(const TMsgGetPower& msg) {
	float powerGiven = stateMultiplier * baseMultiplier * msg.power;
	if (powerGiven > powerToGive) {
		powerGiven = powerToGive;
	}
	powerToGive -= powerGiven;
	CEntity *entity = msg.sender;
	entity->sendMsg(TMsgGainPower{ CHandle(this).getOwner(), powerGiven });
}

void TCompGivePower::setStateMultiplier(float multiplier) {
	stateMultiplier = multiplier;
}

void TCompGivePower::reset() {
	powerToGive = maxPowerToGive;
}

float TCompGivePower::getPowerToGive() {
	return powerToGive;
}
