#pragma once

#include "geometry/transform.h"
#include "geometry/rigid_anim.h"

class CRigidAnimResource;

struct TCompRigidAnim : public TCompBase {
	RigidAnims::CController controller;
	float                   current_time = 0.f;
	bool                    loops = true;
	bool                    is_moving = true;
	float                   speed_factor = 1.0f;
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void debugInMenu();
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	static void registerMsgs();
	DECL_SIBLING_ACCESS();
};
