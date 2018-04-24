#include "mcv_platform.h"
#include "comp_camera.h"
#include "comp_transform.h"
#include "render/render_objects.h"

DECL_OBJ_MANAGER("camera", TCompCamera);

void TCompCamera::debugInMenu() {
	float fov_deg = rad2deg(getFov());
	float new_znear = getZNear();
	float new_zfar = getZFar();
	bool isOrtographic = isCameraOrtographic();
	float width = getOrtographicWidth();
	float height = getOrtographicHeight();
	bool changed = ImGui::DragFloat("Fov", &fov_deg, 0.1f, 30.f, 175.f);
	changed |= ImGui::DragFloat("Z Near", &new_znear, 0.001f, 0.01f, 5.0f);
	changed |= ImGui::DragFloat("Z Far", &new_zfar, 0.001f, 0.01f, 300.0f);
	changed |= ImGui::DragFloat("ortographicWidth", &width, 1.0f, 1.0f, 3000.0f);
	changed |= ImGui::DragFloat("ortographicHeight", &height, 1.0f, 1.0f, 3000.0f);
	if (changed)
		setPerspective(deg2rad(fov_deg), new_znear, new_zfar, isOrtographic, width, height);
	ImGui::LabelText("AspectRatio", "%f", getAspectRatio());
}

void TCompCamera::renderDebug() {
	MAT44 inv_view_proj = getViewProjection().Invert();

	auto mesh = Resources.get("unit_frustum.mesh")->as<CRenderMesh>();
	mesh->activate();

	// Sample several times to 'view' the z distribution along the 3d space
	const int nsamples = 10;
	for (int i = 1; i < nsamples; ++i) {
		float f = (float)i / (float)(nsamples - 1);
		MAT44 world = MAT44::CreateScale(1.f, 1.f, f) * inv_view_proj;
		setWorldTransform(world, VEC4(1, 1, 1, 1));
		mesh->render();
	}
}

void TCompCamera::load(const json& j, TEntityParseContext& ctx) {
	bool isOrtographic = j.value("isOrtographic", false);
	float fov_deg = j.value("fov", rad2deg(getFov()));
	float z_near = j.value("z_near", 1.f /*getZNear()*/);
	float z_far = j.value("z_far", 1000.f /*getZFar()*/);
	float ortographicWidth = j.value("ortographicWidth", 50.f);
	float ortographicHeight = j.value("ortographicHeight", 50.f);
	setPerspective(deg2rad(fov_deg), z_near, z_far, isOrtographic,ortographicWidth, ortographicHeight);
}

void TCompCamera::update(float delta) {
	TCompTransform* transform = get<TCompTransform>();
	assert(transform);
	this->lookAt(transform->getPosition(), transform->getPosition() + transform->getFront(), transform->getUp());
}
