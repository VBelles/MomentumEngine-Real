#pragma once


VEC3 fromPhysx(physx::PxVec3& v);
QUAT fromPhysx(physx::PxQuat& quat);

physx::PxVec3 toPhysx(VEC3& v);
physx::PxQuat toPhysx(QUAT& quat);
physx::PxTransform toPhysx(CTransform* transform);
physx::PxTransform toPhysx(VEC3& v, QUAT& quat);



