#include "mcv_platform.h"
#include "GameSimulationEventCallback.h"
#include "components/comp_collider.h"

void GameSimulationEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 count) {
	for (PxU32 i = 0; i < count; i++) {
		const PxContactPair& cp = pairs[i];
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
			CHandle colliderHandle1;
			colliderHandle1.fromVoidPtr(pairHeader.actors[0]->userData);
			CEntity* e1 = colliderHandle1.getOwner();

			CHandle colliderHandle2;
			colliderHandle2.fromVoidPtr(pairHeader.actors[1]->userData);
			CEntity* e2 = colliderHandle2.getOwner();

			dbg("e1: %s\n", e1->getName());
			dbg("e2: %s\n", e2->getName());
			if (strcmp(e1->getName(), "The Player") == 0 || strcmp(e2->getName(), "The Player") == 0) {
				dbg("TOUCH FOUND\n");
			}
		}
	}
	//dbg("Contact\n");
	/*if (pairHeader.flags & (PxContactPairHeaderFlag::eREMOVED_ACTOR_0 | PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
		return;
	for (PxU32 i = 0; i < count; ++i) {
		PxContactPair pair = pairs[i];

		CHandle colliderHandle1;
		colliderHandle1.fromVoidPtr(pairHeader.actors[0]->userData);
		CHandle colliderHandle2;
		colliderHandle2.fromVoidPtr(pairHeader.actors[1]->userData);

		CEntity* e1 = colliderHandle1.getOwner();
		CEntity* e2 = colliderHandle2.getOwner();

		if (pair.events.isSet(PxPairFlag::eNOTIFY_TOUCH_FOUND)) {
			if (strcmp(e1->getName(), "The Player") != 0
				|| strcmp(e2->getName(), "The Player") != 0) {
				//dbg("TOUCH FOUND\n");
			}
			e1->sendMsg(TMsgOnContact{ CHandle(e2), pair });
			e2->sendMsg(TMsgOnContact{ CHandle(e1), pair });
		}
		if (pair.events.isSet(PxPairFlag::eNOTIFY_TOUCH_LOST)) {
			if (strcmp(e1->getName(), "The Player") != 0
				|| strcmp(e2->getName(), "The Player") != 0) {
				//dbg("TOUCH LOST\n");

			}
			e1->sendMsg(TMsgOnContact{ CHandle(e2), pair });
			e2->sendMsg(TMsgOnContact{ CHandle(e1), pair });
		}
	}*/
}


void GameSimulationEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count) {
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
		}
		else if (pairs[i].status == PxPairFlag::eNOTIFY_TOUCH_LOST) {
			triggerEntity->sendMsg(TMsgTriggerExit{ otherEntity });
		}
	}
}


