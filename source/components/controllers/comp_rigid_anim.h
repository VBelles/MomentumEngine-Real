#pragma once

#include "geometry/transform.h"
#include "geometry/rigid_anim.h"

class CRigidAnimResource;

struct TCompRigidAnim : public TCompBase {
private:
	RigidAnims::CController controller;
	float                   current_time = 0.f;
	bool                    loops = true;
	bool                    is_moving = true;
	bool					stopOnNextLoop = false;
	float                   speed_factor = 1.0f;
	CTransform				initialTransform;
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
public:
	DECL_SIBLING_ACCESS();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void updateAnimation();
	void debugInMenu();
	static void registerMsgs();
	void setIsMoving(bool isMoving);
	void setStopOnNextLoop(bool stop);
};
