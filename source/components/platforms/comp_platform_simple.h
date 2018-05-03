#pragma once

#include "components/comp_base.h"
#include "geometry/curve.h"

struct TMsgEntityCreated;
class TCompTransform;
class TCompCollider;

class TCompPlatformSimple : public TCompBase {
private:

	CCurve curve;
	float speed = 0.f;
	float ratio = 0.f;
	bool automove = false;
	bool moveBackwards = false;

	float rotationSpeed;
	VEC3 rotationAxis;
	
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
