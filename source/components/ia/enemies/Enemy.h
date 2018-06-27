#pragma once

#include "components/ia/behavior_tree/IBehaviorTree.h"
#include "components/player/attack_info.h"


struct EnemyAttack{
	AttackInfo attackInfo;
	std::string hitboxName;
	float hitboxStart = 0.f;
	float hitboxEnd = 0.f;
};

class Enemy : public IBehaviorTree {
protected:
	float hp = 0.f;
	float maxHp = 0.f;

	VEC3 spawnPosition;
	float maxDistanceFromSpawnSqrd = 0.f;

	VEC3 velocity;
	float movementSpeed;
	float rotationSpeed;
	float gravity;
	float maxVelocity;

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

	float propelDuration = 0.f;
	float floatingDuration = 0.f;
	float grabbedDuration = 0.f;
	float stunDuration = 0.f;

	CTimer timer;
	CTimer animationTimer;

public:
	void load(const json& j, TEntityParseContext & ctx);

};