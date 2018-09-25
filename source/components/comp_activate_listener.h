#pragma once

#include "comp_base.h"

struct TMsgMechanismSystemActivated;
struct TMsgMechanismSystemDeactivated;
struct TMsgEntityCreated;

class TCompActivateListener : public TCompBase {
	DECL_SIBLING_ACCESS();
	//json
	bool renderEnabledOnActive = true;
	bool colliderEnabledOnActive = true;
	bool platformEnabledOnActive = true;
	bool animationEnabledOnActive = true;

	bool changeRender = true;
	bool changeCollider = true;
	bool changePlatform = true;
	bool changeAnim = true;

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

	void onEntityCreated(const TMsgEntityCreated& msg);
	void onActivate(const TMsgMechanismSystemActivated& msg);
	void onDeactivate(const TMsgMechanismSystemDeactivated& msg);
};
