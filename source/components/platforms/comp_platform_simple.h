#pragma once

#include "geometry/curve.h"

struct TMsgEntityCreated;

class TCompPlatformSimple : public TCompBase {
private:
	CCurve curve;
	float speed = 0.f;
	float ratio = 0.f;
	bool automove = false;
	bool moveBackwards = false;
	bool isClosed = false;

	bool enabled = true;
	float elapsedAtPauseRoll;
	float elapsedAtPauseTravelWait;

	//Rolling platform
	CTimer2 rollTimer;
	float rollSpeed = 0.f;
	float rollWaitDuration = 2.f;
	float targetRoll = 0.f;
	bool doRoll = false;


	CTimer2 travelWaitTimer;
	float travelWaitTime = 0.f;

	float rotationSpeed;
	VEC3 rotationAxisLocal;
	VEC3 rotationAxisGlobal;
	
	CHandle transformHandle;
	CHandle colliderHandle;

public:
	DECL_SIBLING_ACCESS();

	bool hasDirector = false;


	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	float angleInBounds(float angle, float lowerBound, float upperBound);

	void onCreated(const TMsgEntityCreated& msg);

	void turnAround();
	void setEnabled(bool enabled);
	bool getEnabled() { return enabled; }

	bool isRolling();

	TCompTransform* getTransform();
	TCompCollider* getCollider();
	PxRigidDynamic* getRigidDynamic();
};
