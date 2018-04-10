#pragma once

#include "comp_base.h"

class TCompTransform;
struct TMsgEntitiesGroupCreated;
struct TMsgColliderDestroyed;
struct TMsgAttackHit;

class TCompBreakable : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	CHandle colliderHandle;
	CHandle transformHandle;
	float hp = 0.f;

	//Messages
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed& msg);
	void onHit(const TMsgAttackHit& msg);

	void onDie();
	void dropLoot();

	TCompCollider* getCollider() { return colliderHandle; }
	TCompTransform* getTransform() { return transformHandle; }

public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
};