#include "mcv_platform.h"
#include "comp_free_camera.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("free_camera", TCompFreeCamera);


void TCompFreeCamera::registerMsgs() {
	DECL_MSG(TCompFreeCamera, TMsgEntityCreated, onCreated);
}

void TCompFreeCamera::load(const json& j, TEntityParseContext& ctx) {
	movementSpeed = j.value("movement_speed", movementSpeed);
	rotationSpeed = j.count("rotation_speed") ? loadVEC2(j["rotation_speed"]) : rotationSpeed;
	pitchRange = j.count("pitch_range") ? loadVEC2(j["pitch_range"]) *  (M_PI / 180.f) : pitchRange;
}

void TCompFreeCamera::onCreated(const TMsgEntityCreated& msg) {
	transformHandle = get<TCompTransform>();
	assert(transformHandle.isValid());
	cameraHandle = get<TCompCamera>();
	assert(cameraHandle.isValid());

}

void TCompFreeCamera::update(float delta) {
	if (cameraHandle.getOwner() != EngineRender.getCurrentCameraHandle()) return;
	updateRotation(delta);
	updatePosition(delta);
}


void TCompFreeCamera::updateRotation(float delta) {
	VEC2 input = VEC2::Zero;
	VEC2 rPadInput = VEC2(EngineInput[Input::PAD_RANALOG_X].value, EngineInput[Input::PAD_RANALOG_Y].value);
	if (rPadInput.Length() > PAD_DEAD_ZONE) {
		input.x -= rPadInput.x * rotationSpeed.x;
		input.y += rPadInput.y * rotationSpeed.y;
		//input = VEC2(-rPadInput.x, rPadInput.y) * rotationSpeed;
	}
	else {
		auto& mouse = EngineInput[Input::PLAYER_1].mouse();
		input.x -= mouse.position_delta.x * rotationSpeed.x;
		input.y -= mouse.position_delta.y * rotationSpeed.y;
		//input = VEC2(-rPadInput.x, -rPadInput.y) * rotationSpeed;
	}

	TCompTransform* transform = getTransform();
	float y, p;
	transform->getYawPitchRoll(&y, &p);
	y += input.x * delta;
	p += input.y * delta;
	p = clamp(p, pitchRange.x, pitchRange.y);
	transform->setYawPitchRoll(y, p);
}

void TCompFreeCamera::updatePosition(float delta) {
	VEC2 input = VEC2::Zero;
	VEC2 lPadInput = VEC2(EngineInput[Input::PAD_LANALOG_X].value, EngineInput[Input::PAD_LANALOG_Y].value);
	if (lPadInput.Length() > PAD_DEAD_ZONE) {
		input = lPadInput;
	}
	else { //Detecto el teclado
		if (EngineInput["left"].isPressed()) {
			input.x -= 1.f;
		}
		if (EngineInput["right"].isPressed()) {
			input.x += 1.f;
		}
		if (EngineInput["backwards"].isPressed()) {
			input.y -= 1.f;
		}
		if (EngineInput["forward"].isPressed()) {
			input.y += 1.f;
		}
	}
	input.Normalize();

	float increment = movementSpeed * delta;

	float yDelta = 0.f;
	if (EngineInput["jump"].isPressed()) {
		yDelta += increment;
	}
	if (EngineInput["down"].isPressed()) {
		yDelta -= increment;
	}

	VEC3 worldInput = getCamera()->getCamera()->TransformToWorld(input);
	VEC3 deltaMovement = worldInput * increment;
	deltaMovement.y += yDelta;
	TCompTransform* transform = getTransform();
	transform->setPosition(transform->getPosition() +  deltaMovement);
}

TCompCamera* TCompFreeCamera::getCamera() {
	return cameraHandle;
}

TCompTransform* TCompFreeCamera::getTransform() {
	return transformHandle;
}

