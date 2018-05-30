#include "mcv_platform.h"
#include "basic_query_filter_callback.h"


PxQueryHitType::Enum BasicQueryFilterCallback::preFilter(const PxFilterData& filterData0,
	const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) {
	PxFilterData& filterData1 = shape->getQueryFilterData();
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1)) {
		return PxQueryHitType::eBLOCK;
	}
	return PxQueryHitType::eNONE;
}

PxQueryHitType::Enum BasicQueryFilterCallback::postFilter(const PxFilterData& filterData, const PxQueryHit& hit) {
	return PxQueryHitType::eNONE;
}
