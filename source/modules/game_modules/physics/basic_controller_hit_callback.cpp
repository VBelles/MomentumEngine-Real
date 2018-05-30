#include "mcv_platform.h"
#include "basic_controller_hit_callback.h"


void BasicControllerHitCallback::onShapeHit(const PxControllerShapeHit& hit) {
	CHandle controllerHandle;
	controllerHandle.fromVoidPtr(hit.controller->getActor()->userData);
	CEntity* controllerEntity = controllerHandle.getOwner();
	controllerEntity->sendMsg(TMsgShapeHit{ hit });
}

void BasicControllerHitCallback::onControllerHit(const PxControllersHit& hit) {
	CHandle controllerHandle;
	controllerHandle.fromVoidPtr(hit.controller->getActor()->userData);
	CEntity* controllerEntity = controllerHandle.getOwner();
	controllerEntity->sendMsg(TMsgControllerHit{ hit });
}

void BasicControllerHitCallback::onObstacleHit(const PxControllerObstacleHit & hit) {
}
