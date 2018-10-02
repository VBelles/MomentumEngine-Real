#pragma once


struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;
struct TMsgTriggerExit;
class TCompPowerGauge;

class TCompPowerSource : public TCompBase {
private:
	float powerPerCycle = std::numeric_limits<float>::max();
	float cycleTime = 7 / 60.f;
	float timeToIncreasePower = 0.2f;

	CTimer timer;

	bool isPlayerInside = false;
	CHandle powerGaugeHandle;

public:
	DECL_SIBLING_ACCESS();
	void debugInMenu();
	static void registerMsgs();
	void update(float delta);

	void load(const json& j, TEntityParseContext& ctx);
	void onTriggerEnter(const TMsgTriggerEnter & msg);
	void onTriggerExit(const TMsgTriggerExit & msg);
	void onGroupCreated(const TMsgEntitiesGroupCreated & msg);

	TCompPowerGauge* getPowerGauge();
};

