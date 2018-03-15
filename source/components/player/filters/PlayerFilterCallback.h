#pragma once
#include "components/player/comp_player_model.h"

class PlayerFilterCallback: public PxControllerFilterCallback, public PxQueryFilterCallback {

private:
	CHandle playerModelHandle;


public:

	PlayerFilterCallback(CHandle playerModelHandle);
	TCompPlayerModel* getPlayerModel() {
		return playerModelHandle;
	}

	bool filter(const PxController& a, const PxController& b) override;
	PxQueryHitType::Enum preFilter(const physx::PxFilterData& filterData, 
		const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags);
	PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit);
	
};