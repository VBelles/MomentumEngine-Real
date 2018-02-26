#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"

class TCompCollectable: public TCompBase {
	DECL_SIBLING_ACCESS();

    CEntity* player;
	float    collision_radius = 0.6f;
    //CHandle h_collector;

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

    void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
    void update(float dt);

    //void onCollect(const TMsgTriggerEnter& msg);
};
