#pragma once

#include "comp_base.h"
#include "geometry/transform.h"
#include "entity/common_msgs.h"
#include "comp_transform.h"
#include "comp_camera.h"


class TCompPlayerModel : public TCompBase {
	float   speedFactor = 0.f;//cargado desde json
	float   rotationSpeed = 0.f;//cargado desde json
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

	DECL_SIBLING_ACCESS();

public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void SetTranslationInput(VEC2 input, float delta);
	void SetRotationInput(float amount, float delta);
	void JumpButtonPressed();
};