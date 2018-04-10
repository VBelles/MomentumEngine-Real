#include "mcv_platform.h"
#include "GameSimulationEventCallback.h"
#include "components/comp_collider.h"

void GameSimulationEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 count) {
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
		CHandle h_trigger_comp_collider;
		h_trigger_comp_collider.fromVoidPtr(pairs[i].triggerActor->userData);

		CHandle h_other_comp_collider;
		h_other_comp_collider.fromVoidPtr(pairs[i].otherActor->userData);

		CEntity* e_other = h_other_comp_collider.getOwner();
		CEntity* e_trigger = h_trigger_comp_collider.getOwner();

		if (pairs[i].status == PxPairFlag::eNOTIFY_TOUCH_FOUND) {
			e_trigger->sendMsg(TMsgTriggerEnter{ e_other });
		}
		else if (pairs[i].status == PxPairFlag::eNOTIFY_TOUCH_LOST) {
			e_trigger->sendMsg(TMsgTriggerExit{ e_other });
		}
	}
}


