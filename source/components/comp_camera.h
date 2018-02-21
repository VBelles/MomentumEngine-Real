#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"
#include "comp_transform.h"
#include "camera/camera.h"

class TCompCamera : public CCamera, public TCompBase {
private:
	DECL_SIBLING_ACCESS();
	//General camera configuration
	float fovInDegrees;
	float zNear;
	float zFar;

	float xCameraSpeed = 2.f;
	float yCameraSpeed = 1.f;

	const float Y_ANGLE_MIN = deg2rad(5.f);
	const float Y_ANGLE_MAX = deg2rad(60.f);
	const float DEFAULT_Y = deg2rad(20.f);

	float maxVerticalOffset = 3.0f;
	float minVerticalOffset = 0.2f;
	VEC3 verticalOffsetVector = VEC3::Zero;
	float pitchAngleRange;

	float distanceToTarget = 0.f;
	VEC3 distanceVector = VEC3::Zero;

	float xIncrement = 0.f;
	float yIncrement = 0.f;

	float speedFactor = 3.f;

	std::string targetName;
	TCompTransform* targetTransform;

	float padDeadZone = 0.1f;
	
	TCompTransform* myTransform;

	VEC3 centeredPosition;
	bool centeringCamera = false;

	void OnGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void CalculateVerticalOffsetVector();
	

public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void UpdateInputCamera(float dt);
	void UpdateCenteringCamera(float dt);
	void CenterCamera();
	
};
