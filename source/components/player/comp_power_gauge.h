#pragma once
#include "components/comp_base.h"

class TCompPowerGauge : public TCompBase {
	DECL_SIBLING_ACCESS();



public:
	int powerLevel = 1;	//Ex: 1, 2, 3 (ssj)
	float power = 0.f;	//Ex: 2300
	float maxPower = 0.f;
	float powerPerLevel = 0.f;
	float dropSpeed = 0.f;
	bool isPure = true;

	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void ReleasePower();
	void GainPower();
	void ResetPower();
	void IncreasePower(float increment);

};
