#pragma once

#include "components/ia/behavior_tree/IBehaviorTree.h"
#include "components/comp_base.h"
#include "entity/common_msgs.h"

class CEntity;
class TCompTransform;
class TCompCollider;
class TCompSkeleton;
struct PowerStats;

class CBehaviorTreeFlyingRangedEnemy : public IBehaviorTree, public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	float maxHealth = 3.f;
	float health = maxHealth;
	float movementSpeed = 2.5f;
	float rotationSpeed;

	float recallDistance = 28.f;

	float attackFov = deg2rad(60);
	float minCombatDistance = 2.f;
	float maxCombatDistance = 20.f;
	float attackCooldown = 7.f;
	float attackDamage = 1.f;
	VEC3 attackSpawnOffset = VEC3(0, 2, 2);
	VEC3 attackTargetOffset = VEC3(0, 0.76f, 0);
	std::string attackPrefab = "data/prefabs/rangedAttack.prefab";

	float propelDuration = 1.5f;
	float floatingDuration = 1.5f;
	float grabbedDuration = 0.8f;

	bool hasAttacked = false;
	bool isDead = false;
	bool isStunned = false;
	float stunDuration;

	CHandle playerHandle;

	float gravity = -55.f;
	VEC3 initialLaunchPos;
	VEC3 spawnPosition;
	VEC3 velocityVector;

	CTimer timer;
	CTimer idleWarTimer;
	CTimer attackTimer;
	CTimer stunTimer;

	AttackInfo receivedAttack;

	int damageCalc(float delta = 0.f);
	int onDeath(float delta = 0.f);
	int dead(float delta = 0.f);
	int onGrab(float delta = 0.f);
	int grabbed(float delta = 0.f);
	int onPropel(float delta = 0.f);
	int propelled(float delta = 0.f);
	int onHorizontalLaunch(float delta = 0.f);
	int horizontalLaunched(float delta = 0.f);
	int onVerticalLaunch(float delta = 0.f);
	int verticalLaunched(float delta = 0.f);
	int floating(float delta = 0.f);
	int onStun(float delta = 0.f);
	int stunned(float delta = 0.f);
	int respawn(float delta = 0.f);
	int returnToSpawn(float delta = 0.f);
	int onIdleWar(float delta = 0.f);
	int idleWar(float delta = 0.f);
	int onAttack(float delta = 0.f);
	int attack(float delta = 0.f);
	int idle(float delta = 0.f);

	bool deathCondition(float delta = 0.f);
	bool deadCondition(float delta = 0.f);
	bool grabCondition(float delta = 0.f);
	bool propelCondition(float delta = 0.f);
	bool horizontalLaunchCondition(float delta = 0.f);
	bool verticalLaunchCondition(float delta = 0.f);
	bool onStunCondition(float delta = 0.f);
	bool stunCondition(float delta = 0.f);
	bool returnToSpawnCondition(float delta = 0.f);
	bool combatCondition(float delta = 0.f);

	CEntity* getPlayerEntity();
	TCompTransform* getPlayerTransform();
	TCompSkeleton* getSkeleton();

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onAttackHit(const TMsgAttackHit& msg);
	void onRespawn(const TMsgRespawn& msg);
	void onOutOfBounds(const TMsgOutOfBounds& msg);

	void rotateTowards(float delta, VEC3 targetPos, float rotationSpeed);

public:
	CBehaviorTreeFlyingRangedEnemy();

	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	void update(float delta);

	TCompTransform* getTransform();
	TCompCollider* getCollider();

	void setSpawnPosition(VEC3 spawnPosition);

};