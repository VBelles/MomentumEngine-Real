#pragma once

#include "components/comp_base.h"
#include "entity/common_msgs.h"
#include "components/player/attack_info.h"

struct TMsgColliderDestroyed;
class Enemy;
class TCompTransform;
class TCompCollider;
class TCompSkeleton;
class TCompHitboxes;
class TCompGivePower;
class TCompPlayerModel;

class TCompEnemy : public TCompBase {
private:
	Enemy* enemy = nullptr;

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onAttackHit(const TMsgAttackHit& msg);
	void onOutOfBounds(const TMsgOutOfBounds& msg);
	void onPerfectDodged(const TMsgPerfectDodged& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed& msg);
	void onHitboxEnter(const TMsgHitboxEnter& msg);

public:
	DECL_SIBLING_ACCESS();

	~TCompEnemy();

	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	void update(float delta);
};