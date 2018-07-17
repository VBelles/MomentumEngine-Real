#pragma once

#include "components/comp_base.h"
#include "entity/common_msgs.h"
#include "components/player/attack_info.h"

struct TMsgTriggerEnter;
struct TMsgTriggerEnter;
struct TMsgColliderDestroyed;

class TCompRangedAttack : public TCompBase {
private:
	CHandle ownerHandle;
	float initialSpeed = 10.f;
	float speed = initialSpeed;
	float lifetime = 30.0f;
	float grabTime = frames2sec(30);
	float warmUpTime = frames2sec(120);
	float scaleSpeed = 0.f;
	AttackInfo attackInfo;
	CTimer timer;
	bool hit = false;
	CTimer grabTimer;
	CTimer warmUpTimer;

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onAssignRangedAttackOwner(const TMsgAssignRangedAttackOwner& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed& msg);
	void onAttackHit(const TMsgAttackHit& msg);

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
};
