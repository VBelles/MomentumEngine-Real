#pragma once

#include "components/ia/behavior_tree/IBehaviorTree.h"
#include "components/comp_base.h"
#include "entity/common_msgs.h"
#include "components/player/attack_info.h"

class CEntity;
class TCompTransform;
class TCompCollider;
class TCompSkeleton;
struct PowerStats;
struct TMsgAttackHit;

class CBehaviorTreeBallEnemy : public IBehaviorTree, public TCompBase {
	DECL_SIBLING_ACCESS();
private:
	float maxHealth = 5.f;
	float health = maxHealth;
	float movementSpeed = 2.0f;
	float stepBackSpeed = 2.5f;
	float rotationSpeed;

	float recallDistanceSqrd = 28.f;

	float chaseFov = deg2rad(60);
	float fovChaseDistanceSqrd = 25.f;
	float smallChaseRadiusSqrd = 10.f;

	float attackFov = deg2rad(60);
	float minCombatDistanceSqrd = 1.5f;
	float maxCombatDistanceSqrd = 4.f;
	float attackCooldown = 5.f;
	float attackDamage = 1.f;

	float gravity = -55.f;
	VEC3 maxVelocity = { 30, 30, 30 };

	float propelDuration = 1.5f;
	float floatingDuration = 1.5f;
	float grabbedDuration = 0.8f;

	bool isDead = false;
	bool isStunned = false;
	float stunDuration;

	CHandle playerHandle;

	VEC3 initialLaunchPos;
	VEC3 spawnPosition;
	VEC3 velocityVector;
	bool grounded = true;

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
	int airborne(float delta = 0.f);
	int respawn(float delta = 0.f);
	int returnToSpawn(float delta = 0.f);
	int chase(float delta = 0.f);
	int stepBack(float delta = 0.f);
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
	bool airborneCondition(float delta = 0.f);
	bool returnToSpawnCondition(float delta = 0.f);
	bool chaseCondition(float delta = 0.f);
	bool combatCondition(float delta = 0.f);
	bool stepBackCondition(float delta = 0.f);

	CEntity* getPlayerEntity();
	TCompTransform* getPlayerTransform();
	TCompSkeleton* getSkeleton();

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onAttackHit(const TMsgAttackHit& msg);
	void onRespawn(const TMsgRespawn& msg);
	void onOutOfBounds(const TMsgOutOfBounds& msg);
	void onPerfectDodged(const TMsgPerfectDodged& msg);

	void updateGravity(float delta);
	float calculateVerticalDeltaMovement(float delta, float acceleration, float maxVelocityVertical);
	void rotateTowards(float delta, VEC3 targetPos, float rotationSpeed);

public:
	CBehaviorTreeBallEnemy();

	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	void update(float delta);

	TCompTransform* getTransform();
	TCompCollider* getCollider();
};