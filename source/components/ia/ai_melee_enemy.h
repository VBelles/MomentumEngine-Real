#pragma once

#include "ai_controller.h"
#include "entity/common_msgs.h"

class CAIMeleeEnemy : public IAIController {
	DECL_SIBLING_ACCESS();

private:

	float health = 5.f;
	float speed = 25.f;
	float movementSpeed = 2.5f;
	float rotationSpeed = 5.f;

	float powerGiven = 7000;

	float chaseFov = deg2rad(60);
	float fovChaseDistance = 20.f;
	float smallChaseRadius = 10.f;

	float attackFov = deg2rad(60);
	float attackRadius = 2.f;
	float attackColdown = 1.f;
	float attackDuration = 1.f;

	CHandle player;

	VEC3 spawnPosition;

	VEC3 velocityVector;

	CTimer recallTimer;
	CTimer waitAttackTimer;
	CTimer attackTimer;

	//Mesage functions
	void OnHit(const TMsgAttackHit& msg);
	void OnGroupCreated(const TMsgEntitiesGroupCreated & msg);

	boolean IsPlayerInAttackRange();
	boolean IsPlayerInFov();

public:
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	void update(float delta);

	void InitStates();
	//States
	void IdleState(float delta);
	void ChaseState(float delta);
	void RecallState(float delta);
	void IdleWarState(float delta);
	void AttackState(float delta);
	void DeathState(float delta);
	void VerticalLaunchState(float delta);

	void LaunchVertically();

	float CalculateVerticalDeltaMovement(float delta, float acceleration, float maxVelocityVertical);

	TCompTransform* getTransform() { return get<TCompTransform>(); }
	TCompCollider* getCollider() { return get<TCompCollider>(); }
	CEntity* getPlayerEntity() { return player; }
	TCompTransform* getPlayerTransform() { return getPlayerEntity()->get<TCompTransform>(); }
};


