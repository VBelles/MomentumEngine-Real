#include "mcv_platform.h"
#include "comp_camera.h"

DECL_OBJ_MANAGER("camera", TCompCamera);



void TCompCamera::debugInMenu() {
	ImGui::DragFloat("FOV", &fovInDegrees, 0.1f, 50.f, 180.f);
	ImGui::DragFloat("distanceToTarget", &distanceToTarget, 0.1f, 1.f, 1000.f);
}

void TCompCamera::load(const json& j, TEntityParseContext& ctx) {

	// ..
	fovInDegrees = j.value("fov", 75.f);
	zNear = j.value("z_near", 0.1f);
	zFar = j.value("z_far", 1000.f);
	setPerspective(deg2rad(fovInDegrees), zNear, zFar);

	distanceToTarget = j.value("distance_to_target", 10.f);
	distanceVector.z = -distanceToTarget;

	if (j.count("target")) {
		targetName = j.value("target", "");
	}

}

void TCompCamera::registerMsgs() {
	DECL_MSG(TCompCamera, TMsgEntitiesGroupCreated, OnGroupCreated);
}

void TCompCamera::OnGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	myTransform = get<TCompTransform>();

	CEntity *playerEntity = (CEntity *)getEntityByName(targetName.c_str());
	targetTransform = playerEntity->get<TCompTransform>();

	yIncrement = DEFAULT_Y;
	pitchAngleRange = Y_ANGLE_MAX - Y_ANGLE_MIN;
}

void TCompCamera::update(float delta) {
	VEC2 increment = GetIncrementFromInput(delta);
	UpdateMovement(increment, delta);
	CalculateVerticalOffsetVector();
	setPerspective(deg2rad(fovInDegrees), zNear, zFar);
	this->lookAt(myTransform->getPosition(), targetTransform->getPosition() + verticalOffsetVector, myTransform->getUp());
}

VEC2 TCompCamera::GetIncrementFromInput(float delta) {
	VEC2 increment = VEC2::Zero;
	VEC2 padInput = {
		EngineInput[Input::EPadButton::PAD_LANALOG_X].value,
		EngineInput[Input::EPadButton::PAD_LANALOG_Y].value
	};
	if (padInput.Length() > padDeadZone) {
		increment.x -= padInput.x * cameraSpeed.x * delta;
		increment.y -= padInput.y * cameraSpeed.y * delta;
	}
	else {
		auto& mouse = EngineInput.host(Input::PLAYER_1).mouse();
		increment.x -= mouse._position_delta.x * cameraSpeed.x * delta;
		increment.y -= mouse._position_delta.y * cameraSpeed.y * delta;
	}
	return increment;
}

void TCompCamera::UpdateMovement(VEC2 increment, float delta) {
	float y, p, r;
	myTransform->getYawPitchRoll(&y, &p, &r);

	//Move the camera to the target position
	myTransform->setPosition(targetTransform->getPosition());

	//Rotate the camera
	y += increment.x;
	p += increment.y;
	p = clamp(p, -Y_ANGLE_MAX, -Y_ANGLE_MIN);
	myTransform->setYawPitchRoll(y, p, r);

	//Move the camera back
	myTransform->setPosition(myTransform->getPosition() - myTransform->getFront() * distanceToTarget);
}

void TCompCamera::CalculateVerticalOffsetVector() {
	float currentOffset = ((pitchAngleRange - (yIncrement - Y_ANGLE_MIN)) / pitchAngleRange) * (maxVerticalOffset - minVerticalOffset) + minVerticalOffset;
	verticalOffsetVector.y = currentOffset;
}

void TCompCamera::CenterCamera() {
	centeredPosition = targetTransform->getPosition() - targetTransform->getFront() * distanceToTarget;
	VEC3 direction = targetTransform->getPosition() - centeredPosition;
	direction.Normalize();
	myTransform->setPosition(centeredPosition);
	float y, p, r;
	myTransform->getYawPitchRoll(&y, &p, &r);
	y = atan2(direction.x, direction.z);
	//p = asin(-direction.y); //No nos interesa el pitch
	myTransform->setYawPitchRoll(y, p, r);

}
