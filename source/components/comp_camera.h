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

	std::string targetName;
	float distanceToTarget = 0.f;

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