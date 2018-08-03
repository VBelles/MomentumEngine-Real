#pragma once

#include "comp_base.h"

class TCompTransform;
struct TMsgEntitiesGroupCreated;
struct TMsgColliderDestroyed;
struct TMsgAttackHit;

//Breakable only by player
class TCompBreakable : public TCompBase {
private:
	//json
	std::string prefabToSpawn;
	float hp = 0.f;
	float lootOffset = 1.f;
	std::vector<std::string> brokenPartsNames;

	std::vector<CHandle> brokenParts;

	CHandle colliderHandle;
	CHandle transformHandle;

	//Messages
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed& msg);
	void onHit(const TMsgAttackHit& msg);

	void onDie();
	void dropLoot();

	TCompCollider* getCollider() { return colliderHandle; }
	TCompTransform* getTransform() { return transformHandle; }

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
};