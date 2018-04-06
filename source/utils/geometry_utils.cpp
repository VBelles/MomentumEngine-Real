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

