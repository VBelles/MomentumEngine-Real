#pragma once

#include "comp_base.h"
#include "geometry/transform.h"
#include "entity/common_msgs.h"
#include "comp_transform.h"
#include "comp_camera.h"


class TCompPlayerModel : public TCompBase {
	float   speedFactor = 2.0f;
	float   rotationSpeed = 2.0f;
	TCompTransform *myTransform;
	TCompCamera *currentCamera;
	void OnCreate(const TMsgEntityCreated& msg);

	DECL_SIBLING_ACCESS();

public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void SetTranslationInput(VEC2 input, float delta);
	void SetRotationInput(float amount, float delta);
};