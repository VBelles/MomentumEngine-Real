#pragma once

#include "comp_base.h"
#include "geometry/transform.h"
#include "entity/common_msgs.h"
#include "comp_transform.h"
#include "comp_camera.h"
#include "player/IActionState.h"


class TCompPlayerModel : public TCompBase {

public:
	enum ActionStates{
		Grounded, Airborne, JumpSquat
	};
	IActionState* actionState;// { get; private set; }

	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void SetActionState(ActionStates newState);
	void update(float dt);
	void SetMovementInput(VEC2 input, float delta);
	void JumpButtonPressed();

	TCompTransform* GetTransform() { return myTransform; }
	TCompCamera* GetCamera() { return currentCamera; }
	TCompCollider* GetCollider() { return collider; }
	VEC3* GetAccelerationVector() { return &accelerationVector; }
	VEC3* GetVelocityVector() { return &velocityVector; }

	float speedFactor = 0.f;//cargado desde json
	float rotationSpeed = 0.f;//cargado desde json
	float fallingMultiplier = 1.1f;
	float normalGravityMultiplier = 1.f;
	float currentGravityMultiplier = 1.f;
	float maxVelocityVertical = 30.f;
	VEC3 jumpVelocityVector = {0.f, 8.f, 0.f};
	bool isGrounded = false;

private:
	VEC3 deltaMovement;
	TCompTransform *myTransform;
	TCompCamera *currentCamera;
	TCompCollider* collider;
	VEC3 accelerationVector = {0.f, -9.81f, 0.f};
	VEC3 velocityVector = { 0.f, 0.f, 0.f };
	void OnGroupCreated(const TMsgEntitiesGroupCreated& msg);


	std::map<ActionStates, IActionState*> actionStates;

	DECL_SIBLING_ACCESS();
};