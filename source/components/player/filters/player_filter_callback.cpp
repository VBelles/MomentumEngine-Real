#include "mcv_platform.h"
#include "player_filter_callback.h"
#include "components/comp_collider.h"


bool PlayerFilterCallback::filter(const PxController& a, const PxController& b) {
	PxShape* tempShape;
	a.getActor()->getShapes(&tempShape, 1);
	PxFilterData filterData = tempShape->getSimulationFilterData();

	PxShape* tempShape1;
	b.getActor()->getShapes(&tempShape1, 1);
	PxFilterData filterData1 = tempShape1->getSimulationFilterData();

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
