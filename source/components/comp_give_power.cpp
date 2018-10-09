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
		reset();
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
	CEntity* ownEntity = CHandle(this).getOwner();
	CEntity* attackerEntity = msg.sender;
	attackerEntity->sendMsg(TMsgGainPower{ ownEntity, powerGiven });
	ownEntity->sendMsg(TMsgLosePower{ attackerEntity, powerGiven });
	resetTimer.reset();
}

void TCompGivePower::setStateMultiplier(float multiplier) {
	stateMultiplier = multiplier;
}

float TCompGivePower::getStateMultiplier() {
	return stateMultiplier;
}

void TCompGivePower::reset() {
	powerToGive = maxPowerToGive;
	resetTimer.reset();
}

float TCompGivePower::getPowerToGive() {
	return powerToGive;
}

void TCompGivePower::setPowerToGive(float amount) {
	powerToGive = clamp(amount, 0.f, maxPowerToGive);
}
