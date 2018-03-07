#pragma once

#include "ai_controller.h"
#include "entity/common_msgs.h"

class CAIFlyingEnemy : public IAIController {
	DECL_SIBLING_ACCESS();

private:
	int health = 5;
	float speed = 25.f;
	float movementSpeed = 2.5f;
	float rotationSpeed = 5.f;

	float powerGiven = 3400;

	float chaseFov = deg2rad(60);
	float fovChaseDistance = 20.f;
	float smallChaseRadius = 10.f;

	float attackFov = deg2rad(60);
	float startAttackingRadius = 2.f;
	float attackCooldown = 1.f;
	float attackDuration = 1.f;

	TCompTransform* transform;
	CEntity* player;
	TCompTransform* playerTransform;
	TCompCollider* collider;

	VEC3 spawnPosition;
	VEC3 propelVelocityVector;

	CTimer recallTimer;
	CTimer waitAttackTimer;
	CTimer attackTimer;

	//Mesage functions
	void OnHit(const TMsgAttackHit& msg);
	void OnGrabbed(const TMsgGrabbed & msg);
	void OnPropelled(const TMsgPropelled & msg);
	void OnGroupCreated(const TMsgEntitiesGroupCreated & msg);

	boolean IsPlayerInAttackRange();
	boolean IsPlayerInFov();

public:
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();

	void InitStates();
	//States
	void IdleState(float delta);
	void DeathState(float delta);
	void GrabbedState(float delta);
	void PropelledState(float delta);
};
