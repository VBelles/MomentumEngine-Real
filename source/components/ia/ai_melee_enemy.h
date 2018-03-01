#pragma once

#include "ai_controller.h"
#include "entity/common_msgs.h"

class PowerStats;

class CAIMeleeEnemy : public IAIController {
	DECL_SIBLING_ACCESS();

private:

	float health = 5.f;
	float speed = 25.f;
	float movementSpeed = 2.5f;
	float rotationSpeed = 5.f;

	float powerGiven = 3400;

	float recallDistance = 28.f;

	float chaseFov = deg2rad(60);
	float fovChaseDistance = 20.f;
	float smallChaseRadius = 10.f;

	float attackFov = deg2rad(60);
	float attackRadius = 2.f;
	float attackCooldown = 1.f;
	float attackDuration = 1.f;

	float propelDuration;

	float floatingDuration = 1.5f;

	//WARNING: GUARRADA MÁXIMA!!!
	float initialHorizontalLaunchY = 0.f;
	CTimer horizontalLaunchTimer;
	float minHorizontalLaunchDuration = 0.15f;

	CHandle player;

	VEC3 spawnPosition;
	VEC3 propelVelocityVector;

	VEC3 velocityVector;

	CTimer recallTimer;
	CTimer waitAttackTimer;
	CTimer attackTimer;
	CTimer propelTimer;
	CTimer launchedFloatingTimer;

	PowerStats* launchPowerStats;

	//Mesage functions
	void OnHit(const TMsgAttackHit& msg);
	void OnGrabbed(const TMsgGrabbed& msg);
	void OnPropelled(const TMsgPropelled& msg);
	void OnGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void OnLaunchedVertically(const TMsgLaunchedVertically& msg);

	void OnLaunchedHorizontally(const TMsgLaunchedHorizontally & msg);

	boolean IsPlayerInAttackRange();
	boolean IsPlayerInFov();

	void UpdateGravity(float delta);

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
	void HorizontalLaunchState(float delta);
	void GrabbedState(float delta);
	void PropelledState(float delta);

	void FloatingState(float delta);

	float CalculateVerticalDeltaMovement(float delta, float acceleration, float maxVelocityVertical);

	TCompTransform* getTransform() { return get<TCompTransform>(); }
	TCompCollider* getCollider() { return get<TCompCollider>(); }
	CEntity* getPlayerEntity() { return player; }
	TCompTransform* getPlayerTransform() { return getPlayerEntity()->get<TCompTransform>(); }


};


