#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"

class TCompMechanism: public TCompBase {
	DECL_SIBLING_ACCESS();

	CTimer deactivationTimer;
	float deactivationTime;
	bool isActivated = false;
	std::vector<std::string> mechanismSystemsNames;
	std::vector<CHandle> mechanismSystems;

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

    void onAllScenesCreated(const TMsgAllScenesCreated& msg);
    void update(float dt);

	void onHit(const TMsgAttackHit& msg);
};
