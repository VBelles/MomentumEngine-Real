#pragma once


struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;
struct TMsgTriggerExit;

// Takes trigger messages and forwards them to specified target.
class TCompFwdMsg : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	CHandle target;

public:
	void debugInMenu();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onTriggerExit(const TMsgTriggerExit& msg);
};
