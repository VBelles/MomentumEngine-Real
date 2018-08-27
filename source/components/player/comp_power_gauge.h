#pragma once
#include "components/comp_base.h"

class TCompPowerGauge : public TCompBase {
private:
	int powerLevel = 1;	//Ex: 1, 2, 3 (ssj)
	float power = 0.f;	//Ex: 2300
	float maxPower = 0.f;
	float powerPerLevel[NUMBER_OF_POWER_LEVELS];
	float dropSpeed[NUMBER_OF_POWER_LEVELS];
	bool isPure = true;
	float freezeDropTime = 1.8f;
	CTimer freezeDropTimer;

	float minIncreasePerFrame = 1.f;
	float targetPower;
	float powerIncreaseSpeed;


public:
	DECL_SIBLING_ACCESS();

	void debugInMenu();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void releasePower();
	void gainPower();
	void resetPower();
	void increasePower(float increment);
	void setPower(float power);

	void increasePowerInTime(float power, float time);

	int getPowerLevel();
	float getPower();
	float getMaxPower();

	float getBarPercentage();
	float getPowerLevelPercentage();
};
