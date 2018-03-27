#pragma once

#include "mcv_platform.h"
#include "components/comp_base.h"
#include "entity/common_msgs.h"

struct TCompRangedAttack : public TCompBase {
	DECL_SIBLING_ACCESS();
private:
	CHandle ownerHandle;
	float speed = 1.0f;
	AttackInfo attackInfo;

	bool hit = false;

	void onCreate(const TMsgEntityCreated& msg);
	void onAssignRangedAttackOwner(const TMsgAssignRangedAttackOwner& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void OnColliderDestroyed(const TMsgColliderDestroyed& msg);

public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
};
