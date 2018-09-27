#pragma once

#include "components/ia/behavior_tree/IBehaviorTree.h"
#include "components/player/attack_info.h"

class TCompHitboxes;
class TCompGivePower;
class TCompPlayerModel;

enum EnemyAttackType {
	Melee, Ranged
};

struct EnemyAttack {
	AttackInfo attackInfo;
	std::string hitboxName;
	float hitboxStart = 0.f;
	float hitboxEnd = 0.f;
	float attackEnd = 0.f;
	EnemyAttackType type = Melee;
	VEC3 attackSpawnOffset = VEC3::Zero;
	VEC3 attackTargetOffset = VEC3::Zero;
	std::string attackPrefab = "";
};

class Enemy : public IBehaviorTree {
public:
	virtual ~Enemy();

	std::string behaviorTreeFile = "";

	bool onHit = false;
	bool onOutOfBounds = false;
	bool onSpawn = false;

	std::string navMeshId;
	CNavMeshQuery* navMeshQuery = nullptr;
	std::vector<VEC3> smoothPath;
	int currentPathPoint = 0;

	//HP
	float hp = 0.f;
	float maxHp = 0.f;

	//On perfect dodged
	int maxHpToGive = 0;
	int hpGiven = 0;

	//Spawn
	VEC3 spawnPosition;
	float maxDistanceFromSpawnSqrd = 0.f;

	//Movement
	VEC3 deltaMovement;
	VEC3 velocity;
	float movementSpeed = 0.f;
	float rotationSpeed = 0.f;
	float gravity = 0.f;
	float gravityMultiplier = 1.f;
	float maxVerticalVelocity = 0.f;
	bool grounded = false;
	bool airborne = false;

	float strollRadius = 0.f;

	//Combat
	AttackInfo receivedAttack;
	float chaseFov = 0.f;
	float fovChaseDistanceSqrd = 0.f;
	float smallChaseRadiusSqrd = 0.f;

	float attackFov = 0.f;
	float shortCombatDistanceSqrd = 0.f;
	float mediumCombatDistanceSqrd = 0.f;
	float longCombatDistanceSqrd = 0.f;

	float stepBackDistanceMoved = 0.f;
	float stepBackDistance = 0.f;

	float attackCoolDown = 0.f;

	float blockTime = 0.f;

	float attackSlots = 1.f;
	std::string attackTarget = "";

	std::map<std::string, EnemyAttack> attacks;
	std::string currentAttack = "";

	float propelDuration = 0.f;
	float grabbedDuration = 0.f;
	float suspensionDuration = 0.f;
	float stunDuration = 0.f;
	VEC3 initialLaunchPos;

	CTimer2 superArmorTimer;
	float superArmorTime = 0.f;
	int superArmorAmount = 0;
	bool isBlocking = false;
	bool blockingBroken = false;

	//Timers
	CTimer2 timer;
	CTimer2 animationTimer;
	CTimer2 stunTimer;
	CTimer2 blockTimer;
	CTimer2 attackTimer;

	//Handles
	CHandle playerHandle;
	CHandle playerTransformHandle;
	CHandle playerModelHandle;
	CHandle transformHandle;
	CHandle colliderHandle;
	CHandle skeletonHandle;
	CHandle hitboxesHandle;
	CHandle powerHandle;

	//Sound
	FMOD_3D_ATTRIBUTES soundAttributes;

	void load(const json& j) override;
	void debugInMenu() override;
	void updateGravity(float delta);
	void updateSoundAttributes();
	void rotateTowards(float delta, VEC3 targetPos, float rotationSpeed);
	void move(float delta);

	bool hasSuperArmor();

	CHandle getEntityHandle();
	TCompTransform* getPlayerTransform();
	TCompPlayerModel* getPlayerModel();
	TCompTransform* getTransform();
	TCompCollider* getCollider();
	TCompSkeleton* getSkeleton();
	TCompHitboxes* getHitboxes();
	TCompGivePower* getPower();

	void resetCurrent();
	void setCurrent(std::string node);
};
