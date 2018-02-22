#pragma once

#include "ai_controller.h"
#include "entity/common_msgs.h"

class CAIMeleeEnemy : public IAIController {
	DECL_SIBLING_ACCESS();

private:

	float health = 180.f;
	float speed = 25.f;
	float movementSpeed = 2.5f;
	float rotationSpeed = 5.f;

	float chaseFov = deg2rad(60);
	float fovChaseDistance = 20.f;
	float smallChaseRadius = 10.f;

	float atackFov = deg2rad(60);
	float atackRadius = 2.f;
	float atackColdown = 1.f;
	float atackDuration = 1.f;

	TCompTransform* transform;
	CEntity* player;
	TCompTransform* playerTransform;
	TCompCollider* collider;

	VEC3 spawnPosition;

	CTimer recallTimer;
	CTimer waitAtackTimer;
	CTimer atackTimer;

	//Mesage functions
	void OnHit(const TMsgDamage& msg);
	void OnGroupCreated(const TMsgEntitiesGroupCreated & msg);

	boolean IsPlayerInAtackRange();
	boolean IsPlayerInFov();

public:
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();

	void InitStates();
	//States
	void IdleState(float delta);
	void ChaseState(float delta);
	void RecallState(float delta);
	void IdleWarState(float delta);
	void AtackState(float delta);

};


