#pragma once


struct TMsgAllScenesCreated;
struct TMsgEntitiesGroupCreated;
struct TMsgMechanismSystemActivated;

class TCompAppear: public TCompBase {
	DECL_SIBLING_ACCESS();
public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onActivate(const TMsgMechanismSystemActivated& msg);
};
