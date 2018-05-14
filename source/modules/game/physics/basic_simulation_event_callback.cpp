#include "mcv_platform.h"
#include "basic_simulation_event_callback.h"
#include "components/comp_collider.h"

void BasicSimulationEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 count) {
	if (pairHeader.flags & (PxContactPairHeaderFlag::eREMOVED_ACTOR_0 | PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
		return;
	for (PxU32 i = 0; i < count; i++) {
		const PxContactPair& cp = pairs[i];
		CHandle colliderHandle1;
		colliderHandle1.fromVoidPtr(pairHeader.actors[0]->userData);
		CEntity* e1 = colliderHandle1.getOwner();

		CHandle colliderHandle2;
		colliderHandle2.fromVoidPtr(pairHeader.actors[1]->userData);
		CEntity* e2 = colliderHandle2.getOwner();

		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
			e1->sendMsg(TMsgOnContact{ CHandle(e2), cp });
			e2->sendMsg(TMsgOnContact{ CHandle(e1), cp });
		}
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {
			e1->sendMsg(TMsgOnContact{ CHandle(e2), cp });
			e2->sendMsg(TMsgOnContact{ CHandle(e1), cp });
		}

	}
}


void BasicSimulationEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count) {
	for (PxU32 i = 0; i < count; ++i) {
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER)) {
			continue;
		}
		CHandle triggerHandle;
		triggerHandle.fromVoidPtr(pairs[i].triggerActor->userData);
		CEntity* triggerEntity = triggerHandle.getOwner();

		CHandle otherHandle;
		otherHandle.fromVoidPtr(pairs[i].otherActor->userData);
		CEntity* otherEntity = otherHandle.getOwner();

		if (pairs[i].status == PxPairFlag::eNOTIFY_TOUCH_FOUND) {
			triggerEntity->sendMsg(TMsgTriggerEnter{ otherEntity });
			std::string params = triggerEntity->getName();
			params += ",";
			params += otherEntity->getName();
			EngineScripting.throwEvent(onTriggerEnter, params);
		}
		else if (pairs[i].status == PxPairFlag::eNOTIFY_TOUCH_LOST) {
			triggerEntity->sendMsg(TMsgTriggerExit{ otherEntity });
			std::string params = triggerEntity->getName();
			params += ",";
			params += otherEntity->getName();
			EngineScripting.throwEvent(onTriggerExit, params);
		}
	}
}


