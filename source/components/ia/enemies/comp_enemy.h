#pragma once

#include "entity/common_msgs.h"
#include "components/player/attack_info.h"

struct TMsgColliderDestroyed;
class Enemy;

class TCompEnemy : public TCompBase {
private:
	Enemy* enemy = nullptr;

	void onEntityCreated(const TMsgEntityCreated& msg);
	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onAttackHit(const TMsgAttackHit& msg);
	void onOutOfBounds(const TMsgOutOfBounds& msg);
	void onPerfectDodged(const TMsgPerfectDodged& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed& msg);
	void onHitboxEnter(const TMsgHitboxEnter& msg);
	void onLosePower(const TMsgLosePower& msg);

public:
	DECL_SIBLING_ACCESS();

	~TCompEnemy();

	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	void update(float delta);
};
