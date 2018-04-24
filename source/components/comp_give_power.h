#pragma once

#include "comp_base.h"

struct TMsgGetPower;

class TCompGivePower : public TCompBase {
	DECL_SIBLING_ACCESS();
private:
	float maxPowerToGive = 0.f;
	float powerToGive = maxPowerToGive;
	float baseMultiplier = 0.f;
	float stateMultiplier = 1.f;

	void onGetPower(const TMsgGetPower& msg);

public:
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);

	void setStateMultiplier(float multiplier);
	void reset();
};