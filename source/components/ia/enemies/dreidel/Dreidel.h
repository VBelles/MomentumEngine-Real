#pragma once

#include "components/ia/enemies/Enemy.h"
#include "components/comp_base.h"
#include "entity/common_msgs.h"
#include "components/player/attack_info.h"

struct TMsgColliderDestroyed;

class Dreidel : public Enemy, public TCompBase {
private:
	void initBehaviorTree();

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onAttackHit(const TMsgAttackHit& msg);
	void onOutOfBounds(const TMsgOutOfBounds& msg);
	void onPerfectDodged(const TMsgPerfectDodged& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed& msg);
	//void onHitboxEnter(const TMsgHitboxEnter& msg);

public:
	DECL_SIBLING_ACCESS();

	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	void update(float delta);
};