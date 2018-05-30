#pragma once

#include "components/comp_base.h"

class TCompCamera;
class TCompTransform;

class TCompFreeCamera : public TCompBase {
private:
	float movementSpeed = 3.f;
	VEC2 rotationSpeed = VEC2(2, 1);
	VEC2 pitchRange = VEC2(deg2rad(-70), deg2rad(60));

	CHandle cameraHandle;
	CHandle transformHandle;

	TCompCamera* getCamera();
	TCompTransform* getTransform();

	void updateRotation(float delta);
	void updatePosition(float delta);

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void load(const json & j, TEntityParseContext & ctx);
	void update(float delta);
	void onCreated(const TMsgEntityCreated& msg);
};
