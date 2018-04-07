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

	void debugInMenu();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void update(float dt);

	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void collect();
	void OnColliderDestroyed(const TMsgColliderDestroyed& msg);
	Type getTypeByName(std::string name);
};

struct TMsgCollect {
	CHandle collectableHandle;
	TCompCollectable::Type type;
	DECL_MSG_ID();
};