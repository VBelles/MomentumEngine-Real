#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"

class TCompMechanism: public TCompBase {
	DECL_SIBLING_ACCESS();

	CTimer desactivationTimer;
	float desactivationTime;
	std::vector<std::string> mechanismSystemsNames;
	std::vector<CHandle> mechanismSystems;

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

    void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
    void update(float dt);

	void onHit(const TMsgAttackHit& msg);
};
