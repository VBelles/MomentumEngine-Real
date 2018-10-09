#pragma once


struct TMsgGetPower;

class TCompGivePower : public TCompBase {
private:
	float maxPowerToGive = 0.f;
	float powerToGive = maxPowerToGive;
	float baseMultiplier = 0.f;
	float stateMultiplier = 1.f;

	CTimer resetTimer;
	float resetTime = 0.f;

	void onGetPower(const TMsgGetPower& msg);

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void debugInMenu();

	void setStateMultiplier(float multiplier);
	float getStateMultiplier();
	void reset();

	float getPowerToGive();
	void setPowerToGive(float amount);
};
