#include "mcv_platform.h"
#include "comp_camera.h"

DECL_OBJ_MANAGER("camera", TCompCamera);



void TCompCamera::debugInMenu() {
	ImGui::DragFloat("FOV", &fovInDegrees, 0.1f, 50.f, 180.f);
}

void TCompCamera::load(const json& j, TEntityParseContext& ctx) {

	// ..
	fovInDegrees = j.value("fov", 75.f);
	zNear = j.value("z_near", 0.1f);
	zFar = j.value("z_far", 1000.f);
	setPerspective(deg2rad(fovInDegrees), zNear, zFar);

	distanceToTarget = j.value("distance_to_target", 10.f);
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
}

void TCompCamera::update(float dt) {
	auto& pad = CEngine::get().getInput().host(Input::PLAYER_1).pad();
	VEC2 rightAnalogInput = VEC2::Zero;
	rightAnalogInput.x = pad.button(Input::EPadButton::PAD_RANALOG_X).value;
	rightAnalogInput.y = pad.button(Input::EPadButton::PAD_RANALOG_Y).value;


	//El pad manda
	if (rightAnalogInput.Length() > padDeadZone) {
		xIncrement -= rightAnalogInput.x * dt;
		yIncrement -= rightAnalogInput.y * dt;
	}
	else {//Si no, mouse
		auto& mouse = CEngine::get().getInput().host(Input::PLAYER_1).mouse();
		xIncrement += mouse._position_delta.x * dt;
		yIncrement += mouse._position_delta.y * dt;
	}
	QUAT rotationQuat = QUAT::CreateFromYawPitchRoll(xIncrement, yIncrement, 0);
	VEC3 distanceVector = { 0, 0, -distanceToTarget };

	VEC3 localPosition = VEC3::Transform(distanceVector, rotationQuat);

	myTransform->setPosition(targetTransform->getPosition() + localPosition);

	setPerspective(deg2rad(fovInDegrees), zNear, zFar);
	this->lookAt(myTransform->getPosition(), targetTransform->getPosition(), myTransform->getUp());
}

