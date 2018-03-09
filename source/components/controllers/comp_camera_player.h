#pragma once

#include "components/comp_base.h"

class TCompCameraPlayer : public TCompBase
{
  DECL_SIBLING_ACCESS();

private:
	CHandle target;

	//General camera configuration
	float fovInDegrees;
	float zNear;
	float zFar;

	VEC2 cameraSpeed = { 2.f, 1.f };

	const float Y_ANGLE_MIN = deg2rad(-60.f);
	const float Y_ANGLE_MAX = deg2rad(-5.f);
	const float DEFAULT_Y = deg2rad(-20.f);

	float maxVerticalOffset = 3.0f;
	float minVerticalOffset = 0.2f;
	VEC3 verticalOffsetVector = VEC3::Zero;
	float pitchAngleRange;

	float distanceToTarget = 0.f;
	VEC3 distanceVector = VEC3::Zero;

	float speedFactor = 3.f;

	std::string targetName;

	float padDeadZone = 0.1f;

	VEC3 centeredPosition;
	bool centeringCamera = false;

	void OnGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void CalculateVerticalOffsetVector();

public:
	static void registerMsgs();
	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	VEC2 GetIncrementFromInput(float delta);
	void UpdateMovement(VEC2 increment, float delta);
	void CenterCamera();
};

