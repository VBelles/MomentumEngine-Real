#include "mcv_platform.h"
#include "comp_camera_player.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("camera_player", TCompCameraPlayer);

void TCompCameraPlayer::debugInMenu() {
	ImGui::DragFloat("distanceToTarget", &distanceToTarget, 0.1f, 1.f, 1000.f);
}

// -------------------------------------------------
void TCompCameraPlayer::renderDebug() {
}

// -------------------------------------------------
void TCompCameraPlayer::load(const json& j, TEntityParseContext& ctx) {
	distanceToTarget = j.value("distance_to_target", 5.f);
	distanceVector.z = -distanceToTarget;

	if (j.count("target")) {
		targetName = j.value("target", "");
	}
}

void TCompCameraPlayer::registerMsgs() {
	DECL_MSG(TCompCameraPlayer, TMsgEntitiesGroupCreated, OnGroupCreated);
}

void TCompCameraPlayer::OnGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	TCompTransform* myTransform = get<TCompTransform>();
	myTransform->setYawPitchRoll(0, DEFAULT_Y, 0);
	pitchAngleRange = Y_ANGLE_MAX - Y_ANGLE_MIN;

	target = getEntityByName(targetName.c_str());
	CEntity *playerEntity = target;
}

void TCompCameraPlayer::update(float delta) {
	VEC2 increment = GetIncrementFromInput(delta);
	UpdateMovement(increment, delta);
	CalculateVerticalOffsetVector();
}

VEC2 TCompCameraPlayer::GetIncrementFromInput(float delta) {
	VEC2 increment = VEC2::Zero;
	VEC2 padInput = {
		EngineInput[Input::EPadButton::PAD_RANALOG_X].value,
		EngineInput[Input::EPadButton::PAD_RANALOG_Y].value
	};
	if (padInput.Length() > padDeadZone) {
		increment.x -= padInput.x * cameraSpeed.x * delta;
		increment.y += padInput.y * cameraSpeed.y * delta;
	}
	else if (!CApp::get().showDebug) {
		auto& mouse = EngineInput.host(Input::PLAYER_1).mouse();
		increment.x -= mouse._position_delta.x * cameraSpeed.x * delta;
		increment.y -= mouse._position_delta.y * cameraSpeed.y * delta;
	}
	return increment;
}

void TCompCameraPlayer::UpdateMovement(VEC2 increment, float delta) {
	float y, p, r;
	TCompTransform* myTransform = get<TCompTransform>();
	myTransform->getYawPitchRoll(&y, &p, &r);

	//Move the camera to the target position
	CEntity *playerEntity = target;
	TCompTransform* targetTransform = playerEntity->get<TCompTransform>();
	myTransform->setPosition(targetTransform->getPosition());

	//Rotate the camera
	y += increment.x;
	p += increment.y;
	p = clamp(p, Y_ANGLE_MIN, Y_ANGLE_MAX);
	myTransform->setYawPitchRoll(y, p, r);

	//Move the camera back
	myTransform->setPosition(myTransform->getPosition() - myTransform->getFront() * distanceToTarget);
}

void TCompCameraPlayer::CalculateVerticalOffsetVector() {
	float y, p, r;
	TCompTransform* myTransform = get<TCompTransform>();
	myTransform->getYawPitchRoll(&y, &p, &r);
	float currentOffset = ((pitchAngleRange - (y - Y_ANGLE_MIN)) / pitchAngleRange) * (maxVerticalOffset - minVerticalOffset) + minVerticalOffset;
	verticalOffsetVector.y = currentOffset;
}

void TCompCameraPlayer::CenterCamera() {
	CEntity *playerEntity = target;
	TCompTransform* targetTransform = playerEntity->get<TCompTransform>();
	centeredPosition = targetTransform->getPosition() - targetTransform->getFront() * distanceToTarget;
	VEC3 velocityVector = targetTransform->getPosition() - centeredPosition;
	velocityVector.Normalize();

	TCompTransform* myTransform = get<TCompTransform>();
	myTransform->setPosition(centeredPosition);
	float y, p, r;
	myTransform->getYawPitchRoll(&y, &p, &r);
	y = atan2(velocityVector.x, velocityVector.z);
	//p = asin(-velocityVector.y); //No nos interesa el pitch
	myTransform->setYawPitchRoll(y, p, r);
}
