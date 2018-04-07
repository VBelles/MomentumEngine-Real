#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"



class TCompBreakable : public TCompBase {
	DECL_SIBLING_ACCESS();

public:

private:
	
public:

	void debugInMenu();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void update(float dt);

	void onColliderDestroyed(const TMsgColliderDestroyed& msg);

};