#include "mcv_platform.h"
#include "PlayerFilterCallback.h"

PlayerFilterCallback::PlayerFilterCallback(CHandle playerModelHandle) {
	this->playerModelHandle = playerModelHandle;
}


bool PlayerFilterCallback::filter(const PxController& a, const PxController& b) {
	
	return true;
}

physx::PxQueryHitType::Enum PlayerFilterCallback::preFilter(const physx::PxFilterData& filterData,
	const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags) {
	//const physx::PxFilterData& filterData1 = shape->getQueryFilterData();

	//if ((filterData.word0 & filterData1.word1) && (filterData1.word0 & filterData.word1)) {
	//	return PxQueryHitType::eBLOCK;
	//}
	//return PxQueryHitType::eNONE;
	return PxQueryHitType::eBLOCK;
}

PxQueryHitType::Enum PlayerFilterCallback::postFilter(const PxFilterData & filterData, const PxQueryHit & hit) {
	return PxQueryHitType::eBLOCK;
}
