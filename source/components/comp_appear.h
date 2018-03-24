#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"

class TCompAppear: public TCompBase {
	DECL_SIBLING_ACCESS();

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);
    void update(float dt);

	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onActivate(const TMsgMechanismSystemActivated & msg);
};
