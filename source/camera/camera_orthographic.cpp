#include "mcv_platform.h"
#include "camera_orthographic.h"

void CCameraOrthographic::debugInMenu() {
	float new_znear = getZNear();
	float new_zfar = getZFar();
	float width = getOrtographicWidth();
	float height = getOrtographicHeight();
	bool changed = ImGui::DragFloat("Z Near", &new_znear, 0.001f, 0.01f, 5.0f);
	changed |= ImGui::DragFloat("Z Far", &new_zfar, 0.1f, 6.f, 2000.0f);
	changed |= ImGui::DragFloat("ortographicWidth", &width, 1.0f, 1.0f, 3000.0f);
	changed |= ImGui::DragFloat("ortographicHeight", &height, 1.0f, 1.0f, 3000.0f);
	if (changed) {
		setPerspective(new_znear, new_zfar, width, height);
	}
	ImGui::LabelText("AspectRatio", "%f", getAspectRatio());
}

void CCameraOrthographic::setPerspective(float new_fov_vertical, float new_z_near, float new_z_far) {
	setPerspective(new_z_near, new_z_far, ortographicWidth, ortographicHeight);
}

void CCameraOrthographic::setPerspective(float new_z_near,
	float new_z_far,
	float new_width,
	float new_height) {
	z_near = new_z_near;
	z_far = new_z_far;
	ortographicWidth = new_width;
	ortographicHeight = new_height;
	if (z_far <= z_near) z_far = z_near + 0.1f;
	assert(z_far > z_near);
	aspect_ratio = (float)Render.width / (float)Render.height;
	proj = MAT44::CreateOrthographic(new_width, new_height, new_z_near, new_z_far);
	updateViewProj();
}