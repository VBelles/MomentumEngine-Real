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
		Grounded, Airborne
	};
	IActionState* actionState;// { get; private set; }

	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void SetTranslationInput(VEC2 input, float delta);
	void SetRotationInput(float amount, float delta);
	void JumpButtonPressed();

private:
	float speedFactor = 0.f;//cargado desde json
	float rotationSpeed = 0.f;//cargado desde json
	float fallingMultiplier = 1.1f;
	float normalGravityMultiplier = 1.f;
	float currentGravityMultiplier = 1.f;
	VEC3 deltaMovement;
	TCompTransform *myTransform;
	TCompCamera *currentCamera;
	TCompCollider* collider;
	void OnGroupCreated(const TMsgEntitiesGroupCreated& msg);
	VEC3 accelerationVector = {0.f, -9.81f, 0.f};
	VEC3 velocityVector = { 0.f, 0.f, 0.f };
	VEC3 maxVelocity = {50.f, 25.f, 50.f};
	VEC3 jumpVelocity = {0.f, 8.f, 0.f};

	bool grounded = false;

	std::map<ActionStates, IActionState> actionStates;

	DECL_SIBLING_ACCESS();
};