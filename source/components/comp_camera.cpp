#include "mcv_platform.h"
#include "comp_camera.h"
#include "comp_transform.h"

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

void TCompCamera::update(float dt) {

	TCompTransform* c = get<TCompTransform>();
	assert(c);
	setPerspective(deg2rad(fovInDegrees), zNear, zFar);
	this->lookAt(c->getPosition(), targetPosition, c->getUp());

}

