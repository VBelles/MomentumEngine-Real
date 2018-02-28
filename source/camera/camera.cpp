#include "mcv_platform.h"
#include "camera.h"
/*
CCamera::CCamera() {
  aspect_ratio = 1.f;
  setPerspective(deg2rad(60.f), 1.0f, 1000.f);
  lookAt(VEC3(1, 0, 0), VEC3(0, 0, 0), VEC3(0, 1, 0));
}
*/
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

