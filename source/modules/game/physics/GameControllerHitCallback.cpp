#include "mcv_platform.h"
#include "GameControllerHitCallback.h"
#include "entity/common_msgs.h"

void GameControllerHitCallback::onShapeHit(const PxControllerShapeHit& hit) {
	CHandle controllerHandle;
	controllerHandle.fromVoidPtr(hit.controller->getActor()->userData);
	CEntity* controllerEntity = controllerHandle.getOwner();
	controllerEntity->sendMsg(TMsgOnShapeHit{ hit });
}

void GameControllerHitCallback::onControllerHit(const PxControllersHit & hit) {
}

void GameControllerHitCallback::onObstacleHit(const PxControllerObstacleHit & hit) {
}
