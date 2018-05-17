#include "mcv_platform.h"
#include "player_filter_callback.h"


bool PlayerFilterCallback::filter(const PxController& a, const PxController& b) {
	PxFilterData filterData = getFilterData(&a);
	PxFilterData filterData1 = getFilterData(&b);
	if ((filterData.word0 & filterData1.word1) && (filterData1.word0 & filterData.word1)) {
		return true;
	}
	return false;
}

PxQueryHitType::Enum PlayerFilterCallback::preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) {
	const PxFilterData& filterData1 = shape->getSimulationFilterData();
	if ((filterData.word0 & filterData1.word1) && (filterData1.word0 & filterData.word1)) {
		return PxQueryHitType::eBLOCK;
	}
	return PxQueryHitType::eNONE;

}

PxQueryHitType::Enum PlayerFilterCallback::postFilter(const PxFilterData& filterData, const PxQueryHit & hit) {
	return PxQueryHitType::eBLOCK;
}
