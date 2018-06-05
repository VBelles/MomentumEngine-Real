#pragma once


class CHandle;

namespace PhysxUtils {
	inline VEC3 toVec3(const physx::PxVec3& pxVec3) {
		return VEC3(pxVec3.x, pxVec3.y, pxVec3.z);
	}

	inline VEC3 toVec3(const physx::PxExtendedVec3& pxVec3) {
		return VEC3(float(pxVec3.x), float(pxVec3.y), float(pxVec3.z));
	}

	inline QUAT toQuat(const physx::PxQuat& pxQuat) {
		return QUAT(pxQuat.x, pxQuat.y, pxQuat.z, pxQuat.w);
	}

	inline CTransform toTransform(const physx::PxTransform& pxTransform) {
		return CTransform(toVec3(pxTransform.p), toQuat(pxTransform.q));
	}

	inline physx::PxVec3 toPxVec3(const VEC3& vec3) {
		return physx::PxVec3(vec3.x, vec3.y, vec3.z);
	}

	inline physx::PxExtendedVec3 toPxtendedVec3(const VEC3& vec3) {
		return physx::PxExtendedVec3(vec3.x, vec3.y, vec3.z);
	}

	inline physx::PxQuat toPxQuat(const QUAT& quat) {
		return physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
	}

	inline physx::PxTransform toPxTransform(const CTransform& transform) {
		return physx::PxTransform(toPxVec3(transform.getPosition()), toPxQuat(transform.getRotation()));
	}

	inline physx::PxTransform toPxTransform(const CTransform* transform) {
		return physx::PxTransform(toPxVec3(transform->getPosition()), toPxQuat(transform->getRotation()));
	}

	physx::PxFilterData getFilterData(const physx::PxController* controller);

	CHandle getCollider(const physx::PxActor* actor);

	CHandle getEntity(const physx::PxActor* actor);

}