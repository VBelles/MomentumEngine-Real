#include "mcv_platform.h"
#include "GameQueryFilterCallback.h"


PxQueryHitType::Enum GameQueryFilterCallback::preFilter(const PxFilterData& filterData0,
	const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) {
	PxFilterData& filterData1 = shape->getQueryFilterData();
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1)) {
		return PxQueryHitType::eTOUCH;
	}
	return PxQueryHitType::eNONE;

}

PxQueryHitType::Enum GameQueryFilterCallback::postFilter(const PxFilterData& filterData, const PxQueryHit& hit) {
	return PxQueryHitType::eNONE;
}
