#pragma once

/*VEC3 fromPhysx(physx::PxVec3& v);
QUAT fromPhysx(physx::PxQuat& quat);

physx::PxVec3 toPhysx(VEC3& v);
physx::PxQuat toPhysx(QUAT& quat);
physx::PxTransform toPhysx(CTransform* transform);
physx::PxTransform toPhysx(VEC3& v, QUAT& quat);*/


#define toVec3(pxVec3) VEC3(pxVec3.x, pxVec3.y, pxVec3.z)
#define toQuat(pxQuat) QUAT(pxQuat.x, pxQuat.y, pxQuat.z, pxQuat.w)
#define toTransform(pxTransform) CTransform(toVec3(pxTransform.p), toQuat(pxTransform.q))

#define toPxVec3(vec3) PxVec3(vec3.x, vec3.y, vec3.z)
#define toPxQuat(quat) PxQuat(quat.x, quat.y, quat.z, quat.w)
#define toPxTransform(transform) PxTransform(toPxVec3(transform.getPosition()), toPxQuat(transform.getRotation()))