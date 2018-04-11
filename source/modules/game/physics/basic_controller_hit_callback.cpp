#include "mcv_platform.h"
#include "basic_controller_hit_callback.h"
#include "entity/common_msgs.h"

void BasicControllerHitCallback::onShapeHit(const PxControllerShapeHit& hit) {
	CHandle controllerHandle;
	controllerHandle.fromVoidPtr(hit.controller->getActor()->userData);
	CEntity* controllerEntity = controllerHandle.getOwner();
	controllerEntity->sendMsg(TMsgOnShapeHit{ hit });
}

void BasicControllerHitCallback::onControllerHit(const PxControllersHit & hit) {
}

void BasicControllerHitCallback::onObstacleHit(const PxControllerObstacleHit & hit) {
}
