#pragma once

#include "components/comp_base.h"

struct TMsgEntityCreated;
class TCompTransform;
class TCompCollider;

class TCompPlatformSimple : public TCompBase {
private:
	std::vector<VEC3> waypoints;
	size_t currentWaypoint = 0;
	float speed = 0.f;
	VEC3 rotationSpeed;

	CHandle transformHandle;
	CHandle colliderHandle;

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void onCreated(const TMsgEntityCreated& msg);

	TCompTransform* getTransform();
	TCompCollider* getCollider();
	PxRigidDynamic* getRigidDynamic();

};
