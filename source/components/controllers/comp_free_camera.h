#pragma once

#include "components/comp_base.h"

class TCompCamera;
class TCompTransform;

class TCompFreeCamera : public TCompBase {
private:
	float movementSpeed = 10.f;
	VEC2 rotationSpeed = VEC2(2.5, 1.25);
	VEC2 pitchRange = VEC2(deg2rad(-80), deg2rad(80));
	float speedMultiplier = 3.f;

	CHandle cameraHandle;
	CHandle transformHandle;

	TCompCamera* getCamera();
	TCompTransform* getTransform();

	void updateRotation(float delta);
	void updatePosition(float delta);

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json & j, TEntityParseContext & ctx);
	void update(float delta);
	void onCreated(const TMsgEntityCreated& msg);
};
