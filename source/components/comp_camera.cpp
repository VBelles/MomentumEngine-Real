#include "mcv_platform.h"
#include "comp_camera.h"
#include "comp_transform.h"
#include "render/render_objects.h"
#include "camera/camera_orthographic.h"

DECL_OBJ_MANAGER("camera", TCompCamera);

TCompCamera::~TCompCamera() {
	SAFE_DELETE(camera);
}

void TCompCamera::debugInMenu() {
	camera->debugInMenu();
}

void TCompCamera::renderDebug() {
	MAT44 inv_view_proj = camera->getViewProjection().Invert();

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
	SAFE_DELETE(camera);

	float z_near = j.value("z_near", 1.f);
	float z_far = j.value("z_far", 1000.f);

	bool isOrthographic = j.value("isOrthographic", false);
	if (isOrthographic) {
		camera = new CCameraOrthographic();
		float orthographicWidth = j.value("orthographicWidth", 50.f);
		float orthographicHeight = j.value("orthographicHeight", 50.f);
		((CCameraOrthographic*)camera)->setPerspective(z_near, z_far, orthographicWidth, orthographicHeight);

	}
	else {
		camera = new CCamera();
		float fov_deg = j.value("fov", deg2rad(60.f));
		camera->setPerspective(deg2rad(fov_deg), z_near, z_far);
	}
}

void TCompCamera::update(float delta) {
	TCompTransform* transform = get<TCompTransform>();
	assert(transform);
	camera->lookAt(transform->getPosition(), transform->getPosition() + transform->getFront(), transform->getUp());
}

CCamera* TCompCamera::getCamera() {
	return camera;
}