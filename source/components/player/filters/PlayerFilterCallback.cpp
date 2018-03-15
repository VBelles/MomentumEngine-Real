#include "mcv_platform.h"
#include "PlayerFilterCallback.h"

PlayerFilterCallback::PlayerFilterCallback(CHandle playerModelHandle) {
	this->playerModelHandle = playerModelHandle;
}


bool PlayerFilterCallback::filter(const PxController& a, const PxController& b) {
	CHandle colliderHandle;
	colliderHandle.fromVoidPtr(a.getActor()->userData);
	TCompCollider* collider = colliderHandle;
	PxFilterData filterData = collider->config.filterData;

	CHandle colliderHandle1;
	colliderHandle1.fromVoidPtr(b.getActor()->userData);
	TCompCollider* collider1 = colliderHandle1;
	PxFilterData filterData1 = collider1->config.filterData;

	if ((filterData.word0 & filterData1.word1) && (filterData1.word0 & filterData.word1)) {
		return true;
	}
	return false;

}

physx::PxQueryHitType::Enum PlayerFilterCallback::preFilter(const physx::PxFilterData& filterData,
	const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags) {

	const physx::PxFilterData& filterData1 = shape->getSimulationFilterData();
	if ((filterData.word0 & filterData1.word1) && (filterData1.word0 & filterData.word1)) {
		return PxQueryHitType::eBLOCK;
	}
	return PxQueryHitType::eNONE;

}

PxQueryHitType::Enum PlayerFilterCallback::postFilter(const PxFilterData& filterData, const PxQueryHit & hit) {
	return PxQueryHitType::eBLOCK;
}
