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
	bool isClosed = false;
	bool enabled = true;

	//Rolling platform
	CTimer rollTimer;
	float rollSpeed = 5.f;
	float rollWaitDuration = 2.f;
	float targetRoll = 0.f;
	bool doRoll = false;

	bool hasDirector = false;

	CTimer travelWaitTimer;
	float travelWaitTime = 0.f;

	float rotationSpeed;
	VEC3 rotationAxisLocal;
	VEC3 rotationAxisGlobal;
	
	CHandle transformHandle;
	CHandle colliderHandle;

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void onCreated(const TMsgEntityCreated& msg);

	void turnAround();
	void setEnabled(bool enabled);

	TCompTransform* getTransform();
	TCompCollider* getCollider();
	PxRigidDynamic* getRigidDynamic();
};
