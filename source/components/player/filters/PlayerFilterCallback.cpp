#include "mcv_platform.h"
#include "PlayerFilterCallback.h"
#include "components/comp_collider.h"


bool PlayerFilterCallback::filter(const PxController& a, const PxController& b) {
    CHandle colliderHandle;
    colliderHandle.fromVoidPtr(a.getActor()->userData);
    TCompCollider* collider = colliderHandle;
    const PxFilterData& filterData = PxFilterData(collider->config.group, collider->config.mask, 0, 0);

    CHandle colliderHandle1;
    colliderHandle1.fromVoidPtr(b.getActor()->userData);
    TCompCollider* collider1 = colliderHandle1;
    const PxFilterData& filterData1 = PxFilterData(collider1->config.group, collider1->config.mask, 0, 0);

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
