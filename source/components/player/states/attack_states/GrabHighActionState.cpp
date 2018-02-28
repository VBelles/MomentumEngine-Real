#include "mcv_platform.h"
#include "GrabHighActionState.h"
#include "components/comp_hitbox.h"
#include "components/comp_tags.h"

GrabHighActionState::GrabHighActionState(CHandle playerHandle, CHandle hitbox)
	: GrabActionState::GrabActionState(playerHandle, hitbox) {
}

void GrabHighActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerHandle.getOwner();
	if (entity != playerEntity) {
		//Si es enemigo transicionamos a propel high
		CEntity *otherEntity = entity;

		TCompTags* tag = otherEntity->get<TCompTags>();
		if (tag && tag->hasTag(getID("enemy"))) {
			otherEntity->sendMsg(TMsgGrabbed{ playerEntity });
			dbg("Grab High\n");
			GetPlayer()->grabTarget = entity;
			GetPlayer()->SetAttackState(TCompPlayerModel::ActionStates::PropelHigh);
		}
	}
}