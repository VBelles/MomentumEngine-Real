#pragma once


struct TMsgAllScenesCreated;
struct TMsgAttackHit;

class TCompMechanism: public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	CTimer deactivationTimer;
	CTimer reactivationTimer;
	float deactivationTime;
	bool isActivated = false;
	bool isResettable = true;
	float reactivationTime = 0.f;
	std::vector<std::string> mechanismSystemsNames;
	std::vector<CHandle> mechanismSystems;
	FMOD::Studio::EventInstance* clockEventInstance = nullptr;

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

    void onAllScenesCreated(const TMsgAllScenesCreated& msg);
    void update(float dt);

	void onHit(const TMsgAttackHit& msg);
	void onActivated(bool isActive, bool sound = true);
};
