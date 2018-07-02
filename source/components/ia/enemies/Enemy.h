#pragma once

#include "components/ia/behavior_tree/IBehaviorTreeNew.h"
#include "components/player/attack_info.h"

class TCompTransform;
class TCompCollider;
class TCompSkeleton;
class TCompHitboxes;

struct EnemyAttack {
	AttackInfo attackInfo;
	std::string hitboxName;
	float hitboxStart = 0.f;
	float hitboxEnd = 0.f;
};

class Enemy : public IBehaviorTreeNew {
public:
	virtual ~Enemy();

	//HP
	float hp = 0.f;
	float maxHp = 0.f;

	//Spawn
	VEC3 spawnPosition;
	float maxDistanceFromSpawnSqrd = 0.f;

	//Movement
	VEC3 deltaMovement;
	VEC3 velocity;
	float movementSpeed = 0.f;
	float rotationSpeed = 0.f;
	float gravity = 0.f;
	float maxVerticalVelocity = 0.f;
	bool grounded = false;
	bool airborne = false;

	float strollRadius = 0.f;
	VEC3 nextWaypoint;

	//Combat
	AttackInfo receivedAttack;
	float chaseFov = 0.f;
	float fovChaseDistanceSqrd = 0.f;
	float smallChaseRadiusSqrd = 0.f;

	float attackFov = 0.f;
	float shortCombatDistanceSqrd = 0.f;
	float mediumCombatDistanceSqrd = 0.f;
	float longCombatDistanceSqrd = 0.f;
	float stepBackDistanceSqrd = 0.f;

	//<AttackInfo, hitboxName, hitboxStart, hitboxEnd>
	std::map<std::string, EnemyAttack> attacks;
	std::string currentAttack = "";

	float propelDuration = 0.f;
	float grabbedDuration = 0.f;
	float stunDuration = 0.f;
	VEC3 initialLaunchPos;

	CTimer superArmorTimer;
	float superArmorTime = 0.f;
	int superArmorAmount = 0;
	bool isBlocking = false;
	bool blockingBroken = false;

	//Timers
	CTimer timer;
	CTimer animationTimer;
	CTimer stunTimer;

	//Handles
	CHandle playerHandle;
	CHandle transformHandle;
	CHandle colliderHandle;
	CHandle skeletonHandle;
	CHandle hitboxesHandle;

	void load(const json& j, TEntityParseContext & ctx);

	void updateGravity(float delta);
	void rotateTowards(float delta, VEC3 targetPos, float rotationSpeed);
	void move(float delta);

	bool hasSuperArmor();

	CEntity* getPlayer();
	TCompTransform* getPlayerTransform();
	TCompTransform* getTransform();
	TCompCollider* getCollider();
	TCompSkeleton* getSkeleton();
	TCompHitboxes* getHitboxes();

};