#include "mcv_platform.h"

VEC3 fromPhysx(physx::PxVec3& v) {
	return VEC3(v.x, v.y, v.z);
}

QUAT fromPhysx(physx::PxQuat& quat) {
	return QUAT(quat.x, quat.y, quat.z, quat.w);
}

physx::PxVec3 toPhysx(VEC3& v) {
	return physx::PxVec3(v.x, v.y, v.z);
}

physx::PxQuat toPhysx(QUAT& quat) {
	return physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
}

physx::PxTransform toPhysx(CTransform* transform) {
	VEC3& p = transform->getPosition();
	QUAT& q = transform->getRotation();
	return physx::PxTransform(physx::PxVec3(p.x, p.y, p.z), physx::PxQuat(q.x, q.y, q.z, q.w));
}

physx::PxTransform toPhysx(VEC3& p, QUAT& q) {
	return physx::PxTransform(physx::PxVec3(p.x, p.y, p.z), physx::PxQuat(q.x, q.y, q.z, q.w));
}

