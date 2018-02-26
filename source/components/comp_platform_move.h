#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"

class TCompPlatformMove: public TCompBase {
	DECL_SIBLING_ACCESS();

    CEntity*    player;
    std::string type = "";

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

    void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
    void update(float dt);

    //void onCollect(const TMsgTriggerEnter& msg);
};
