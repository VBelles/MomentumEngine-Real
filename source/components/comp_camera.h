#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"
#include "comp_transform.h"
#include "camera/camera.h"

class TCompCamera : public CCamera, public TCompBase {
	//General camera configuration
	float fovInDegrees;
	float zNear;
	float zFar;
	const float Y_ANGLE_MIN = deg2rad(5.f);
	const float Y_ANGLE_MAX = deg2rad(60.f);
	const float DEFAULT_Y = deg2rad(20.f);

	std::string targetName;
	float distanceToTarget = 0.f;
	VEC3 distanceVector = VEC3::Zero;

	float padDeadZone = 0.1f;
	float speedFactor = 3.f;
	
	float xIncrement = 0.f;
	float yIncrement = 0.f;
	
	TCompTransform* targetTransform;
	TCompTransform* myTransform;

	void OnGroupCreated(const TMsgEntitiesGroupCreated& msg);

public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	
	DECL_SIBLING_ACCESS();
};