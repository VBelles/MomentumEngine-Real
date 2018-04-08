#pragma once

#include "components/comp_base.h"

class CEntity;
class TCompTransform;

class TCompFixedCamera : public TCompBase {
	DECL_SIBLING_ACCESS();

private:

	//Json configuration
	float panningSpeed;
	bool returnToPlayerCameraWithInput;
	float timeToMixOut;

	CHandle transformHandle;

	VEC2 input;

	bool isMovementLocked = false;

	//Msgs
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onLockCameraInput(const TMsgLockCameraInput& msg);

	void updateInput();

	TCompTransform* getTransform();

public:
	static void registerMsgs();
	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
};

