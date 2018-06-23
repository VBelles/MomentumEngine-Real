#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeNew.h"
#include "components/comp_base.h"
#include "entity/common_msgs.h"
#include "components/player/attack_info.h"

class CEntity;
class TCompTransform;
class TCompCollider;
class TCompSkeleton;
class TCompHitboxes;
struct PowerStats;
struct TMsgAttackHit;

class Dreidel : public IBehaviorTreeNew, public TCompBase {
private:
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onAttackHit(const TMsgAttackHit& msg);
	void onRespawn(const TMsgRespawn& msg);
	void onOutOfBounds(const TMsgOutOfBounds& msg);
	void onPerfectDodged(const TMsgPerfectDodged& msg);
	void onHitboxEnter(const TMsgHitboxEnter& msg);

public:
	DECL_SIBLING_ACCESS();

	float maxHealth = 300.f;
	float health = maxHealth;
	float movementSpeed = 2.5f;
	float stepBackSpeed = 3.5f;
	float rotationSpeed = 20.f;

	float recallDistanceSqrd = 784.f;

	float chaseFov;
	float fovChaseDistanceSqrd = 25.f;
	float smallChaseRadiusSqrd = 10.f;

	float attackFov = deg2rad(60);
	float minCombatDistanceSqrd = 1.5f;
	float maxCombatDistanceSqrd = 4.f;
	float attackCooldown = 5.f;
	float attackDamage = 1.f;

	std::map<std::string, AttackInfo> attackInfos;
	std::map<std::string, VEC2> attacksFrameData;

	float gravity = -55.f;
	VEC3 maxVelocity = { 30, 30, 30 };

	float propelDuration = 1.5f;
	float defaultPropelDuration = propelDuration;
	float floatingDuration = 1.5f;
	float grabbedDuration = 0.8f;

	bool hasAttacked = false;
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

	Dreidel();

	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	void update(float delta);

	void updateGravity(float delta);
	float calculateVerticalDeltaMovement(float delta, float acceleration, float maxVelocityVertical);
	void rotateTowards(float delta, VEC3 targetPos, float rotationSpeed);

	CEntity* getPlayerEntity();
	TCompTransform* getPlayerTransform();
	TCompSkeleton* getSkeleton();
	TCompHitboxes* getHitboxes();
	TCompTransform* getTransform();
	TCompCollider* getCollider();
};