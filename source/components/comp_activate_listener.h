#pragma once

#include "comp_base.h"

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

	void onActivate(const TMsgMechanismSystemActivated& msg);
	void onDeactivate(const TMsgMechanismSystemDeactivated& msg);
};
