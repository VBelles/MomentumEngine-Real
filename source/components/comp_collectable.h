#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"


class TCompCollectable: public TCompBase {
	DECL_SIBLING_ACCESS();

	float collision_radius = 0.3f;
    CHandle         h_collector;

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

    void onCreate(const TMsgEntityCreated& msg);
    void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
    void onCollect(const TMsgTriggerEnter& msg);
};
