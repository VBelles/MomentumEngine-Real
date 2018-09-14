#pragma once

struct TMsgAllScenesCreated;

class TCompDayNightCycle : public TCompBase {
private:
	CTimer2 cycleTimer;

	//dawn, day, dusk, night
	size_t dayPeriodIndex;
	std::vector<float> durations;

	std::string skyName;
	CHandle skyHandle;

	float lerpTime = 10.f;

	bool enabled = true;

public:
	DECL_SIBLING_ACCESS();
	void onAllScenesCreated(const TMsgAllScenesCreated& msg);

	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void debugInMenu();

	void setEnabled(bool enabled);
};
