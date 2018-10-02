#include "mcv_platform.h"
#include "camera_gui.h"

void CCameraGui::setPerspective(float new_fov_vertical, float new_z_near, float new_z_far) {
	setPerspective(new_z_near, new_z_far, ortographicWidth, ortographicHeight);
}

void CCameraGui::setPerspective(float new_z_near,
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

	MAT44 custom_ortho = loadOrtho(0.f, ortographicWidth, ortographicHeight, 0.f, z_near, z_far);
	proj = custom_ortho;

	// Position and target and unset!
	pos = VEC3(ortographicWidth * 0.5f, ortographicHeight * 0.5f, z_near);
	front.x = 0;
	front.y = 0;
	front.z = 1;
	left.x = 1;
	left.y = 0;
	left.z = 0;
	up.x = 0;
	up.y = 1;
	up.z = 0;
	view = MAT44::Identity;
	view_proj = proj;

	updateViewProj();
}

MAT44 CCameraGui::loadOrtho(float x_min, float x_max, float y_min, float y_max, float z_min, float z_max) {
	MAT44 m = MAT44::Identity;

	// (RH) l=x_min; r=x_max; b=y_min; t=y_max; zn=z_min; zf=z_max; 
	//
	// 2/(r-l)      0            0           0
	// 0            2/(t-b)      0           0
	// 0            0            1/(zf-zn)   0
	// (l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  1

	assert(x_max != x_min);
	assert(y_max != y_min);
	assert(z_max != z_min);

	m._11 = 2.0f / (x_max - x_min);
	m._22 = 2.0f / (y_max - y_min);
	m._33 = 1.0f / (z_max - z_min);
	m._41 = (x_min + x_max) / (x_min - x_max);
	m._42 = (y_max + y_min) / (y_min - y_max);
	m._43 = z_min / (z_min - z_max);
	return m;
}
