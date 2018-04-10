#pragma once

#include "components/comp_base.h"
#include "entity/common_msgs.h"

struct TMsgTriggerEnter;
struct TMsgAssignRangedAttackOwner;
struct TMsgTriggerEnter;
struct TMsgColliderDestroyed;

class TCompRangedAttack : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	CHandle ownerHandle;
	float speed = 1.0f;
	float lifetime = 60.0f;
	AttackInfo attackInfo;
	CTimer timer;
	bool hit = false;

	void onCreate(const TMsgEntityCreated& msg);
	void onAssignRangedAttackOwner(const TMsgAssignRangedAttackOwner& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed& msg);

public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
};
