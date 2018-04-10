#pragma once

#include "comp_base.h"

class TCompAppear: public TCompBase {
	DECL_SIBLING_ACCESS();
	CTimer cutreTimer;
	float cutreTime = 3.5f;
public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onActivate(const TMsgMechanismSystemActivated& msg);
};
