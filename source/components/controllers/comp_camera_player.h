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
	float idleDistanceToTarget = 0.f;
	VEC2 cameraSpeed;
	float zoomOutSpeed = 20.f;
	float zoomInSpeed = 10.f;
	VEC2 centeringCameraSpeed;
	float minPitch = 0.f;
	float maxPitch = 0.f;
	float initialYaw = 0.f;
	float initialPitch = 0.f;
	float zoomInSpeedIdleRun = 0.f;
	float zoomOutSpeedIdleRun = 0.f;
	float minPitchOffset = 0.f;
	float maxPitchOffset = 0.f;
	float pitchOffsetThreshold = 0.f;

	//float pitchOffset = deg2rad(10);
	float pitchOffset = 16.f;
	
	CHandle targetHandle;
	CHandle transformHandle;
	CTransform targetTransform;
	VEC3 prevTargetPosition;

	VEC3* playerVelocityVector;

	VEC2 input;
	
	float offset = 1.f;

	float defaultZoomInSpeed;
	float defaultZoomOutSpeed;
	float currentDistanceToTarget = 0.f;
	float runDistanceToTarget = 0.f;
	VEC2 currentCenteringCameraSpeed;

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
	bool hasOppositeYaw = false;

	float mouseLerpCoef = 35.f;

	//Msgs
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onLockCameraInput(const TMsgLockCameraInput& msg);

	void updateTargetTransform(float delta);
	void updateInput();
	void updateRotation(float delta);
	void updatePosition(float delta);
	void updateCenteringCamera(float delta);
	float calculatePitchOffset(float pitch);
	bool sphereCast();
	bool isCameraInsideGeometry();
	bool raycast(VEC3 origin, VEC3 destination, PxRaycastCallback& callback);
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
	void suggestYawPitchDistance(float yaw, float pitch, float distance, bool suggestYaw, bool hasOppositeYaw, bool suggestPitch, bool forceDistance, bool changeCenteringCamera);
	void placeCameraOnSuggestedPosition(VEC2 centeringSpeed);
	void resetSuggested();
    void lockCameraInput(bool isLocked);
	void moveCameraCloser(bool wantClose);

	void resetCurrentDistance();
	float getInitialPitch() { return initialPitch; }
};

