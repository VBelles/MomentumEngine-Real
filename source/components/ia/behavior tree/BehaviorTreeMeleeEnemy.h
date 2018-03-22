#pragma once

#include "IBehaviorTree.h"
#include "components/comp_base.h"
#include "entity/common_msgs.h"

class CEntity;
class TCompTransform;
class TCompCollider;

class CBehaviorTreeMeleeEnemy : public IBehaviorTree, public TCompBase {
	DECL_SIBLING_ACCESS();
private:
	const float maxHealth = 5.f;
	float health = maxHealth;
	float speed = 25.f;
	float movementSpeed = 2.5f;
	float rotationSpeed = 5.f;

	const float maxPowerToGive = 5000.f;
	float powerToGive = maxPowerToGive;

	float recallDistance = 28.f;

	float chaseFov = deg2rad(60);
	float fovChaseDistance = 20.f;
	float smallChaseRadius = 10.f;

	float attackFov = deg2rad(60);
	float startAttackingRadius = 2.f;
	float rangeRadius = 4.f;
	float attackCooldown = 1.8f;
	float attackDuration = 0.8f;

	float propelDuration;

	float floatingDuration = 1.5f;
	float grabbedDuration = 0.5f;

	bool isDead = false;

	bool isStunned = false;

	CHandle playerHandle;

	VEC3 spawnPosition;
	VEC3 propelVelocityVector;

	VEC3 velocityVector;

	CTimer recallTimer;
	CTimer waitAttackTimer;
	CTimer attackTimer;
	CTimer propelTimer;
	CTimer launchedFloatingTimer;
	CTimer grabbedTimer;

	AttackInfo receivedAttack;

	int damageCalc();
	int onDeath();
	int dead();
	int onGrab();
	int grabbed();
	int onPropel();
	int propelled();
	int onHorizontalLaunch();
	int horizontalLaunched();
	int onVerticalLaunch();
	int verticalLaunched();
	int onStun();
	int stunned();
	int respawn();
	int returnToSpawn();
	int chase();
	int idleWar();
	int attack();
	int idle();

	bool deathCondition();
	bool deadCondition();
	bool grabCondition();
	bool propelCondition();
	bool horizontalLaunchCondition();
	bool verticalLaunchCondition();
	bool stunCondition();
	bool returnToSpawnCondition();
	bool chaseCondition();
	bool combatCondition();

	CEntity* getPlayerEntity();
	TCompTransform* getPlayerTransform();

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onAttackHit(const TMsgAttackHit& msg);
	void onRespawn(const TMsgRespawn& msg);

public:
	CBehaviorTreeMeleeEnemy();

	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	void update(float delta);

	TCompTransform* getTransform();
	TCompCollider* getCollider();
};