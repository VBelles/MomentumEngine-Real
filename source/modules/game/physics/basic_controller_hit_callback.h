#pragma once
#include "entity/msgs.h"

struct TMsgShapeHit {
	const physx::PxControllerShapeHit& hit;
	DECL_MSG_ID();
};
struct TMsgControllerHit {
	const physx::PxControllersHit& hit;
	DECL_MSG_ID();
};

class BasicControllerHitCallback : public PxUserControllerHitReport {
	virtual void onShapeHit(const PxControllerShapeHit& hit) override;
	virtual void onControllerHit(const PxControllersHit& hit) override;
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) override;
};
