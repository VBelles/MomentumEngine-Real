#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"



class TCompCollectable : public TCompBase {
	DECL_SIBLING_ACCESS();

public:
	enum Type {
		UNDEFINED, CHRYSALIS, COIN
	};

	std::map<std::string, Type> typeByName = {
		{ "undefined", UNDEFINED },
	    { "chrysalis", CHRYSALIS },
	    { "coin", COIN }
	};


private:
	bool collected = false;
	Type type;

public:

	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void OnColliderDestroyed(const TMsgColliderDestroyed& msg);

	void collect();
	Type getTypeByName(std::string name);
};

struct TMsgCollect {
	CHandle collectableHandle;
	TCompCollectable::Type type;
	DECL_MSG_ID();
};