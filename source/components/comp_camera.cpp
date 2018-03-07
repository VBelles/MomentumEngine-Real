#include "mcv_platform.h"
#include "comp_camera.h"
#include "comp_transform.h"
#include "render/render_objects.h"

DECL_OBJ_MANAGER("camera", TCompCamera);

void TCompCamera::debugInMenu() {
  float fov_deg = rad2deg(getFov());
  float new_znear = getZNear();
  float new_zfar = getZFar();
  bool changed = ImGui::DragFloat("FOV", &fovInDegrees, 0.1f, 30.f, 175.f);
  ImGui::DragFloat("distanceToTarget", &distanceToTarget, 0.1f, 1.f, 1000.f);
  changed |= ImGui::DragFloat("Z Near", &new_znear, 0.001f, 0.01f, 1.0f);
  changed |= ImGui::DragFloat("Z Far", &new_zfar, 1.0f, 2.0f, 3000.0f);
  if (changed)
    setPerspective(deg2rad(fov_deg), new_znear, new_zfar);
  ImGui::LabelText("AspectRatio", "%f", getAspectRatio());
}

// -------------------------------------------------
void TCompCamera::renderDebug() {
  MAT44 inv_view_proj = getViewProjection().Invert();
  
  auto mesh = Resources.get("unit_frustum.mesh")->as<CRenderMesh>();
  mesh->activate();

  // Sample several times to 'view' the z distribution along the 3d space
  const int nsamples = 10;
  for (int i = 1; i < nsamples; ++i) {
    float f = (float)i / (float)(nsamples-1);
    MAT44 world = MAT44::CreateScale(1.f, 1.f, f) * inv_view_proj;
    setWorldTransform(world, VEC4(1, 1, 1, 1));
    mesh->render();
  }
}

// -------------------------------------------------
void TCompCamera::load(const json& j, TEntityParseContext& ctx) {
	fovInDegrees = j.value("fov", rad2deg(getFov()));
	zNear = j.value("z_near", 0.1f /*getZNear()*/);
	zFar = j.value("z_far", 1000.f /*getZFar()*/);
	setPerspective(deg2rad(fovInDegrees), zNear, zFar);

	distanceToTarget = j.value("distance_to_target", 5.f);
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

	myTransform->setYawPitchRoll(0, DEFAULT_Y, 0);
	pitchAngleRange = Y_ANGLE_MAX - Y_ANGLE_MIN;
}

void TCompCamera::update(float delta) {
	VEC2 increment = GetIncrementFromInput(delta);
	UpdateMovement(increment, delta);
	CalculateVerticalOffsetVector();
	setPerspective(deg2rad(fovInDegrees), zNear, zFar);
	this->lookAt(myTransform->getPosition(), targetTransform->getPosition() + VEC3::Up * 1.5f /*+ verticalOffsetVector*/ , myTransform->getUp());
}

VEC2 TCompCamera::GetIncrementFromInput(float delta) {
	VEC2 increment = VEC2::Zero;
	VEC2 padInput = {
		EngineInput[Input::EPadButton::PAD_RANALOG_X].value,
		EngineInput[Input::EPadButton::PAD_RANALOG_Y].value
	};
	if (padInput.Length() > padDeadZone) {
		increment.x -= padInput.x * cameraSpeed.x * delta;
		increment.y += padInput.y * cameraSpeed.y * delta;
	}
	else if(!CApp::get().showDebug){
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
	p = clamp(p, Y_ANGLE_MIN, Y_ANGLE_MAX);
	myTransform->setYawPitchRoll(y, p, r);

	//Move the camera back
	myTransform->setPosition(myTransform->getPosition() - myTransform->getFront() * distanceToTarget);
}

void TCompCamera::CalculateVerticalOffsetVector() {
	float y, p, r;
	myTransform->getYawPitchRoll(&y, &p, &r);
	float currentOffset = ((pitchAngleRange - (y - Y_ANGLE_MIN)) / pitchAngleRange) * (maxVerticalOffset - minVerticalOffset) + minVerticalOffset;
	verticalOffsetVector.y = currentOffset;
}

void TCompCamera::CenterCamera() {
	centeredPosition = targetTransform->getPosition() - targetTransform->getFront() * distanceToTarget;
	VEC3 velocityVector = targetTransform->getPosition() - centeredPosition;
	velocityVector.Normalize();
	myTransform->setPosition(centeredPosition);
	float y, p, r;
	myTransform->getYawPitchRoll(&y, &p, &r);
	y = atan2(velocityVector.x, velocityVector.z);
	//p = asin(-velocityVector.y); //No nos interesa el pitch
	myTransform->setYawPitchRoll(y, p, r);
}
