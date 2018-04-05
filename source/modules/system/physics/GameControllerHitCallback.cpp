#include "mcv_platform.h"
#include "GameControllerHitCallback.h"

void GameControllerHitCallback::onShapeHit(const PxControllerShapeHit& hit) {
	CHandle controllerHandle;
	controllerHandle.fromVoidPtr(hit.controller->getActor()->userData);
	CEntity* controllerEntity = controllerHandle.getOwner();
	controllerEntity->sendMsg(TMsgOnShapeHit{ hit });
}