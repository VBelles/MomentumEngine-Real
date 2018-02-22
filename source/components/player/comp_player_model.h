#pragma once

#include "components/comp_base.h"
#include "geometry/transform.h"
#include "entity/common_msgs.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "states/IActionState.h"
#include "PowerGauge.h"


struct PowerStats {
	float maxHorizontalSpeed = 0.f;
	float rotationSpeed = 0.f;
	float fallingMultiplier = 1.1f;
	float normalGravityMultiplier = 1.f;
	float currentGravityMultiplier = 1.f;
	float maxVelocityVertical = 30.f;
	VEC3 jumpVelocityVector = { 0.f, 8.f, 0.f };
};


class TCompPlayerModel : public TCompBase, PowerGauge::PowerListener {
	DECL_SIBLING_ACCESS();

public:
	int frame = 0;
	enum ActionStates{
		Grounded, Airborne, JumpSquat, GhostJumpSquat,
		GhostJumpWindow, Run, AirborneNormal,
	};
	IActionState* actionState;
	
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void SetActionState(ActionStates newState);
	void update(float dt);
	void SetMovementInput(VEC2 input, float delta);
	void JumpButtonPressed();
	void CenterCameraButtonPressed();
	void ReleasePowerButtonPressed();
	void GainPowerButtonPressed();

	TCompTransform* GetTransform() { return myTransform; }
	TCompCamera* GetCamera() { return currentCamera; }
	TCompCollider* GetCollider() { return collider; }
	VEC3* GetAccelerationVector() { return &accelerationVector; }
	VEC3* GetVelocityVector() { return &velocityVector; }
	float GetAcceleration() { return acceleration; }
	
	PowerStats* GetPowerStats();
	
	bool isGrounded = false;

	void OnLevelChange(int powerLevel);

private:
	VEC3 deltaMovement;
	TCompTransform *myTransform;
	TCompCamera *currentCamera;
	TCompCollider* collider;
	VEC3 accelerationVector = {0.f, 0.f, 0.f};
	VEC3 velocityVector = { 0.f, 0.f, 0.f };
	float acceleration;
	float gravity = 0;


	PowerStats* ssj1;
	PowerStats* ssj2;
	PowerStats* ssj3;
	PowerStats* currentPowerStats;

	void OnGroupCreated(const TMsgEntitiesGroupCreated& msg);

	PowerStats* loadPowerStats(const json& j);

	PowerGauge* powerGauge;

	std::map<ActionStates, IActionState*> actionStates;
};