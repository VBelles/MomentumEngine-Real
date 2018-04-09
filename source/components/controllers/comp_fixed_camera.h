#pragma once

#include "components/comp_base.h"

class CEntity;
class TCompTransform;

using namespace Interpolator;

class TCompFixedCamera : public TCompBase {
	DECL_SIBLING_ACCESS();

private:

	//Json configuration
	float panningSpeed;
	bool returnToPlayerCameraWithInput;
	float timeToMixOut;
	bool modifyPlayerCameraRotation = true;
	float positionRange = 1.f;

	CHandle transformHandle;
	VEC3 originalPosition;
	VEC3 leftPosition;
	VEC3 rightPosition;
	VEC3 upperPosition;
	VEC3 lowerPosition;

	CTimer repositionTimer;
	float repositionTime = 0.04f;
	bool countingForReposition = false;
	bool isRepositioning = true;

	VEC2 input;

	bool isMovementLocked = false;

	//Msgs
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onLockCameraInput(const TMsgLockCameraInput& msg);

	void updateInput();

	TCompTransform* getTransform();

	TCubicOutInterpolator* cubicOutInterpolator;

	void CopyRotationFromMixedCameraToPlayerCamera();
public:
	static void registerMsgs();
	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
};

