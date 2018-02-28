#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"

class TCompCollectable: public TCompBase {
	DECL_SIBLING_ACCESS();

	bool collected = false;
	std::string type = "";

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

    void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
    void update(float dt);

	void onCollect(const TMsgTriggerEnter& msg);
	void OnColliderDestroyed(const TMsgColliderDestroyed& msg);
};
