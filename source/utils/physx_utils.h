#pragma once

#define toVec3(pxVec3) VEC3(pxVec3.x, pxVec3.y, pxVec3.z)
#define toQuat(pxQuat) QUAT(pxQuat.x, pxQuat.y, pxQuat.z, pxQuat.w)
#define toTransform(pxTransform) CTransform(toVec3(pxTransform.p), toQuat(pxTransform.q))

#define toPxVec3(vec3) physx::PxVec3(vec3.x, vec3.y, vec3.z)
#define toPxtendedVec3(vec3) physx::PxExtendedVec3(vec3.x, vec3.y, vec3.z)
#define toPxQuat(quat) physx::PxQuat(quat.x, quat.y, quat.z, quat.w)

template <typename T>
physx::PxTransform toPxTransform(T transform) {
	return physx::PxTransform(toPxVec3(transform.getPosition()), toPxQuat(transform.getRotation()));
}
template <typename T>
physx::PxTransform toPxTransform(T* transform) {
	return physx::PxTransform(toPxVec3(transform->getPosition()), toPxQuat(transform->getRotation()));
}


physx::PxFilterData getFilterData(const physx::PxController* controller);

