#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"
#include "comp_transform.h"
#include "camera/camera.h"

class TCompCamera : public CCamera, public TCompBase {
	//General camera configuration
	VEC3 targetPosition;
	float fovInDegrees;
	float zNear;
	float zFar;

	float padDeadZone = 0.1f;
	float speedFactor = 3.f;
	
	float xIncrement = 0.f;
	float yIncrement = 0.f;
	float distanceToTarget = 0.f;

	TCompTransform* myTransform;

	void OnGroupCreated(const TMsgEntitiesGroupCreated& msg);

public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	
	DECL_SIBLING_ACCESS();
};