#pragma once

#include "components/ia/enemies/Enemy.h"
#include "components/comp_base.h"
#include "entity/common_msgs.h"
#include "components/player/attack_info.h"

struct TMsgColliderDestroyed;

class Kippah : public Enemy, public TCompBase {
private:
	void initBehaviorTree();

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onAttackHit(const TMsgAttackHit& msg);
	void onOutOfBounds(const TMsgOutOfBounds& msg);
	void onPerfectDodged(const TMsgPerfectDodged& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed& msg);
	void onHitboxEnter(const TMsgHitboxEnter& msg);

public:
	DECL_SIBLING_ACCESS();

	int maxHpToGive = 1;
	int hpGiven = 0;
	VEC3 attackSpawnOffset = VEC3(0, 3, 0);
	VEC3 attackTargetOffset = VEC3(0, 0.76f, 0);
	std::string attackPrefab = "data/prefabs/rangedAttack.prefab";

	~Kippah();

	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	void update(float delta);
};