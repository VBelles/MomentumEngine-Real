#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"


class TCompCollectable: public TCompBase {
	DECL_SIBLING_ACCESS();

	float collision_radius = 0.3f;

	void OnCollect(const TMsgTriggerEnter& msg);

public:

    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);
    void onCreate(const TMsgEntityCreated& msg);
};
