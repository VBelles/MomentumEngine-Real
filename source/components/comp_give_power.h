#pragma once


struct TMsgGetPower;

class TCompGivePower : public TCompBase {
private:
	float maxPowerToGive = 0.f;
	float powerToGive = maxPowerToGive;
	float baseMultiplier = 0.f;
	float stateMultiplier = 1.f;

	void onGetPower(const TMsgGetPower& msg);

	std::string particles;
	VEC3 particlesOffset;

	CTimer resetTimer;
	float resetTime = 0.f;

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void debugInMenu();

	void setStateMultiplier(float multiplier);
	void reset();

	float getPowerToGive();
	void setPowerToGive(float amount);
};
