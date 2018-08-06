#pragma once

#include "comp_base.h"

struct TMsgAllScenesCreated;
struct TMsgEntitiesGroupCreated;
struct TMsgMechanismSystemActivated;
struct TMsgMechanismSystemDeactivated;

class TCompActivateListener : public TCompBase {
	DECL_SIBLING_ACCESS();
	//json
	bool renderEnabledOnActive = true;
	bool colliderEnabledOnActive = true;
	bool platformEnabledOnActive = true;
	bool animationEnabledOnActive = true;
public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onActivate(const TMsgMechanismSystemActivated& msg);
	void onDeactivate(const TMsgMechanismSystemDeactivated& msg);
};
