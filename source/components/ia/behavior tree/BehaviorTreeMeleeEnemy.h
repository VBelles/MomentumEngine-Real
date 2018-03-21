#pragma once

#include "IBehaviorTree.h"
#include "components/comp_base.h"
#include "entity/common_msgs.h"

class CBehaviorTreeMeleeEnemy : public IBehaviorTree, public TCompBase {
	DECL_SIBLING_ACCESS();
private:

public:
	CBehaviorTreeMeleeEnemy();

	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	void update(float delta);
};