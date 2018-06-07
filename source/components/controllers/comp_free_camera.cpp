#include "mcv_platform.h"
#include "comp_free_camera.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "entity/common_msgs.h"


DECL_OBJ_MANAGER("free_camera", TCompFreeCamera);

void TCompFreeCamera::registerMsgs() {
	DECL_MSG(TCompFreeCamera, TMsgEntityCreated, onCreated);
}

void TCompFreeCamera::debugInMenu() {
	ImGui::DragFloat("Movement speed", &movementSpeed, 1.f, 0.f, 100.f);
	ImGui::DragFloat2("Rotation speed", &rotationSpeed.x, 0.01f, 0.f, 5.f);
	ImGui::DragFloat("Speed multiplier", &rotationSpeed.x, 0.5f, 0.f, 20.f);
}

void TCompFreeCamera::load(const json& j, TEntityParseContext& ctx) {
	movementSpeed = j.value("movement_speed", movementSpeed);
	speedMultiplier = j.value("speed_multiplier", speedMultiplier);
	rotationSpeed = j.count("rotation_speed") ? loadVEC2(j["rotation_speed"]) : rotationSpeed;
	pitchRange = j.count("pitch_range") ? loadVEC2(j["pitch_range"]) *  ((float)M_PI / 180.f) : pitchRange;
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
		input = VEC2(-rPadInput.x, rPadInput.y) * rotationSpeed;
	}
	else {
		auto& mouse = EngineInput[Input::PLAYER_1].mouse();
		input = -mouse.position_delta * rotationSpeed;
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
	VEC3 input = VEC2::Zero;
	VEC2 lPadInput = VEC2(EngineInput[Input::PAD_LANALOG_X].value, EngineInput[Input::PAD_LANALOG_Y].value);
	if (lPadInput.Length() > PAD_DEAD_ZONE) {
		input.x = -lPadInput.x;
		input.z = lPadInput.y;
	}
	else { //Detecto el teclado
		if (EngineInput["left"].isPressed()) {
			input.x += 1.f;
		}
		if (EngineInput["right"].isPressed()) {
			input.x -= 1.f;
		}
		if (EngineInput["backwards"].isPressed()) {
			input.z -= 1.f;
		}
		if (EngineInput["forward"].isPressed()) {
			input.z += 1.f;
		}
	}
	
	if (EngineInput["jump"].isPressed()) {
		input.y += 1.f;
	}
	if (EngineInput["down"].isPressed()) {
		input.y -= 1.f;
	}

	float speedIncrement = movementSpeed * delta;

	if (EngineInput["turbo"].isPressed()) {
		speedIncrement *= speedMultiplier;
	}
	
	input.Normalize();

	TCompTransform* transform = getTransform();

	VEC3 deltaMovement;
	deltaMovement += transform->getFront() * input.z * speedIncrement;
	deltaMovement += transform->getLeft() * input.x * speedIncrement;
	deltaMovement += VEC3::UnitY * input.y * speedIncrement;

	transform->setPosition(transform->getPosition() + deltaMovement);
}

TCompCamera* TCompFreeCamera::getCamera() {
	return cameraHandle;
}

TCompTransform* TCompFreeCamera::getTransform() {
	return transformHandle;
}

