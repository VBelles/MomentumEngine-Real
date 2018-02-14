#include "mcv_platform.h"
#include "camera.h"

void CCamera::updateViewProj() {
  view_proj = view * proj;
}

// 
void CCamera::lookAt(VEC3 new_pos, VEC3 new_target, VEC3 new_up_aux) {
  
  pos = new_pos;
  target = new_target;
  up_aux = new_up_aux;

  front = (target - pos);
  front.Normalize();

  left = new_up_aux.Cross(front);
  left.Normalize();

  up = front.Cross(left);

  view = MAT44::CreateLookAt(pos, target, up_aux);
  updateViewProj();
}

void CCamera::setPerspective(float new_fov_vertical, float new_z_near, float new_z_far) {
  proj = MAT44::CreatePerspectiveFieldOfView(new_fov_vertical, (float)Render.width / (float)Render.height, new_z_near, new_z_far);
  updateViewProj();
}

VEC3 CCamera::TransformToWorld(VEC3 direction) {
	direction.Normalize();
	VEC3 cameraFront = getFront();
	VEC3 cameraRight = -getLeft();
	cameraFront.y = 0.f;
	cameraRight.y = 0.f;
	cameraFront.Normalize();
	cameraRight.Normalize();
	VEC3 desiredDirection = cameraFront * direction.z + cameraRight * direction.x;
	return desiredDirection;
}

VEC3 CCamera::TransformToWorld(VEC2 direction) {
	VEC3 direction3D = { direction.x, 0, direction.y };
	return TransformToWorld(direction3D);
}

