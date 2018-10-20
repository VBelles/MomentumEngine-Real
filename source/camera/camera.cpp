#include "mcv_platform.h"
#include "camera.h"

void CCamera::debugInMenu() {
	float fov_deg = rad2deg(getFov());
	float new_znear = getZNear();
	float new_zfar = getZFar();
	bool changed = false;
	changed |= ImGui::DragFloat3("Pos", &pos.x, 0.025f, -2000.f, 2000.f);
	changed |= ImGui::DragFloat3("Target", &target.x, 0.025f, -2000.f, 2000.f);
	changed |= ImGui::DragFloat("Fov", &fov_deg, 0.1f, 30.f, 175.f);
	changed |= ImGui::DragFloat("Z Near", &new_znear, 0.001f, 0.01f, 5.0f);
	changed |= ImGui::DragFloat("Z Far", &new_zfar, 0.001f, 0.01f, 300.0f);
	if (changed) {
		setPerspective(deg2rad(fov_deg), new_znear, new_zfar);
		lookAt(pos, target, up_aux);
	}
	ImGui::LabelText("AspectRatio", "%f", getAspectRatio());
}

void CCamera::updateViewProj() {
    view_proj = view * proj;
}

void CCamera::lookAt(VEC3 new_pos, VEC3 new_target, VEC3 new_up_aux) {
    pos = new_pos;
    target = new_target;
    up_aux = new_up_aux;

    front = (target - pos);
    front.Normalize();
    left = front.Cross(new_up_aux);
    left.Normalize();
    up = front.Cross(left);
    up.Normalize();

    left = new_up_aux.Cross(front);
    left.Normalize();

    up = front.Cross(left);

    view = MAT44::CreateLookAt(pos, target, up_aux);
    updateViewProj();
}

void CCamera::setPerspective(float new_fov_vertical,
                             float new_z_near,
                             float new_z_far) {
    fov_vertical = new_fov_vertical;
    z_near = new_z_near;
    z_far = new_z_far;
    if (z_far <= z_near) z_far = z_near + 0.1f;
    assert(z_far > z_near);
    aspect_ratio = (float)Render.width / (float)Render.height;
    proj = MAT44::CreatePerspectiveFieldOfView(new_fov_vertical, aspect_ratio, new_z_near, new_z_far);
    updateViewProj();
}

void CCamera::setViewport(int x0, int y0, int width, int height) {
    // save params
    viewport.x0 = x0;
    viewport.y0 = y0;
    viewport.width = width;
    viewport.height = height;

    aspect_ratio = (float)width / (float)height;

    setPerspective(fov_vertical, z_near, z_far);
}

bool CCamera::getScreenCoordsOfWorldCoord(VEC3 world_pos, VEC3* result) const {
    // It's also dividing by w  -> [-1..1]
    VEC3 pos_in_homo_space = VEC3::Transform(world_pos, getViewProjection());

    // Convert to 0..1 and then to viewport coordinates
    VEC3 pos_in_screen_space(
        viewport.x0 + (pos_in_homo_space.x + 1.0f) * 0.5f * viewport.width,
        viewport.y0 + (1.0f - pos_in_homo_space.y) * 0.5f * viewport.height,
        pos_in_homo_space.z
    );

    assert(result);
    *result = pos_in_screen_space;

    // Return true if the coord is inside the frustum
    return pos_in_homo_space.x >= -1.0f && pos_in_homo_space.x <= 1.0f
        && pos_in_homo_space.y >= -1.0f && pos_in_homo_space.y <= 1.0f
        && pos_in_homo_space.z >= 0.0f && pos_in_homo_space.z <= 1.0f;
}

VEC3 CCamera::TransformToWorld(VEC3 velocityVector) {
    velocityVector.Normalize();
    VEC3 cameraFront = getFront();
    VEC3 cameraRight = -getLeft();
    cameraFront.y = 0.f;
    cameraRight.y = 0.f;
    cameraFront.Normalize();
    cameraRight.Normalize();
    VEC3 desiredDirection = cameraFront * velocityVector.z + cameraRight * velocityVector.x;
    desiredDirection.Normalize();
    return desiredDirection;
}

VEC3 CCamera::TransformToWorld(VEC2 velocityVector) {
    VEC3 direction3D = { velocityVector.x, 0, velocityVector.y };
    return TransformToWorld(direction3D);
}

void CCamera::getYawPitchRoll(float * yaw, float * pitch, float* roll) const {
    getYawPitchFromVector(front, yaw, pitch);

    // If requested...
    if (roll) {
        VEC3 roll_zero_left = VEC3(0, 1, 0).Cross(getFront());
        VEC3 roll_zero_up = VEC3(getFront()).Cross(roll_zero_left);
        VEC3 my_real_left = getLeft();
        float rolled_left_on_up = my_real_left.Dot(roll_zero_up);
        float rolled_left_on_left = my_real_left.Dot(roll_zero_left);
        *roll = atan2f(rolled_left_on_up, rolled_left_on_left);
    }
}
