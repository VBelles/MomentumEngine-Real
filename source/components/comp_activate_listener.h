#pragma once


struct TMsgMechanismSystemActivated;
struct TMsgMechanismSystemDeactivated;
struct TMsgAllScenesCreated;

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

	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onActivate(const TMsgMechanismSystemActivated& msg);
	void onDeactivate(const TMsgMechanismSystemDeactivated& msg);
};
