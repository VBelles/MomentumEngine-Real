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
	if (j.count("target_position")) {
		targetPosition = loadVEC3(j["target_position"]);
	}

}

void TCompCamera::registerMsgs(){
	DECL_MSG(TCompCamera, TMsgEntityCreated, OnCreate);
}

void TCompCamera::OnCreate(const TMsgEntityCreated & msg){
	myTransform = get<TCompTransform>();
	assert(myTransform);
}

void TCompCamera::update(float dt) {
	auto& pad = CEngine::get().getInput().host(Input::PLAYER_1).pad();
	VEC2 rightAnalogInput = VEC2::Zero;
	rightAnalogInput.x = pad.button(Input::EPadButton::PAD_RANALOG_X).value;
	rightAnalogInput.y = pad.button(Input::EPadButton::PAD_RANALOG_Y).value;

	if (rightAnalogInput.Length() > padDeadZone) {
		VEC3 myNewPos = myTransform->getPosition();
		

		myTransform->setPosition(myNewPos);
	}
	else {
		auto& mouse = CEngine::get().getInput().host(Input::PLAYER_1).mouse();
		dbg("%f, %f   Delta = %f\n", mouse._position_delta.x, mouse._position_delta.y);
	}

	setPerspective(deg2rad(fovInDegrees), zNear, zFar);
	this->lookAt(myTransform->getPosition(), targetPosition, myTransform->getUp());

	

	
}

