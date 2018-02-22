#pragma once

#include "ai_controller.h"
#include "entity/common_msgs.h"

class CAIMeleeEnemy : public IAIController {
	DECL_SIBLING_ACCESS();
private:

	enum States{Idle, Chase, Recall}; //Todo: use this

	float health = 180.f;

	TCompTransform* transform;
	CEntity* player;
	TCompTransform* playerTransform;

	VEC3 spawnPosition;

	void OnHit(const TMsgDamage& msg);

	void OnGroupCreated(const TMsgEntitiesGroupCreated & msg);

public:
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	void InitStates();
	void IdleState(float delta);
	void ChaseState(float delta);
	void RecallState(float delta);
	boolean IsPlayerInFov();
};


