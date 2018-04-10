#pragma once

#include "ai_controller.h"
#include "entity/common_msgs.h"

struct PowerStats;

class CAIMeleeEnemy : public IAIController {
	DECL_SIBLING_ACCESS();

private:
    const float maxHealth = 5.f;
	float health = maxHealth;
	float speed = 25.f;
	float movementSpeed = 2.5f;
	float rotationSpeed = 5.f;

	float powerGiven = 3400;

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
	std::string originalMeshPath = "data/meshes/enemyMelee.mesh";
    std::string originalShadowMesh;
    std::string originalMaterialPath = "data/materials/piedras1.material";

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
	CTimer grabbedTimer;

	PowerStats* launchPowerStats;

	//Mesage functions
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void OnRespawn(const TMsgRespawn & msg);
	void OnHit(const TMsgAttackHit& msg);
	void OnGrabbed(float duration);
	void OnPropelled(VEC3 velocity);
	void OnLaunchedVertically(float suspensionTime, VEC3 velocity);
	void OnLaunchedHorizontally(float suspensionTime, VEC3 velocity);

	boolean IsPlayerInAttackRange(float range);
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
