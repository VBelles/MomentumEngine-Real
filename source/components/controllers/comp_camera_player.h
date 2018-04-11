#pragma once

#include "components/comp_base.h"

class CEntity;
class TCompTransform;

struct TMsgEntitiesGroupCreated;
struct TMsgLockCameraInput;

class TCompCameraPlayer : public TCompBase {
	DECL_SIBLING_ACCESS();

private:

	//Json configuration
	std::string targetName;
	float defaultDistanceToTarget = 0.f;
	VEC2 cameraSpeed;
	VEC2 centeringCameraSpeed;
	float minPitch = 0.f;
	float maxPitch = 0.f;
	float initialYaw = 0.f;
	float initialPitch = 0.f;
	
	CHandle targetHandle;
	CHandle transformHandle;
	CTransform targetTransform;

	VEC2 input;
	
	float currentDistanceToTarget = 0.f;

	bool isMovementLocked = false;

	VEC2 desiredYawPitch;
	bool centeringCamera = false;

	float sphereCastRadius = 0.0f;

	float suggestedYaw;
	float suggestedPitch;
	float forcedDistance;
	
	bool isYawSuggested = false;
	bool isPitchSuggested = false;
	bool isDistanceForced = false;
	bool isCenteringCameraForced = false;

	//Msgs
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onLockCameraInput(const TMsgLockCameraInput& msg);

	void updateTargetTransform();
	void updateInput();
	void updateMovement(float delta);
	void updateCenteringCamera(float delta);
	bool sphereCast();
	void sweepBack();

	CEntity* getTarget() { return targetHandle; }
	TCompTransform* getTransform() { return transformHandle; }

public:
	static void registerMsgs();
	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void centerCamera();
	void suggestYawPitchDistance(float yaw, float pitch, float distance, bool suggestYaw, bool suggestPitch, bool forceDistance, bool changeCenteringCamera);
	void placeCameraOnSuggestedPosition();
	void resetSuggested();
};

