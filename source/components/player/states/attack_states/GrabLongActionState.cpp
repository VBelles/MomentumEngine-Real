#include "mcv_platform.h"
#include "GrabLongActionState.h"
#include "components/comp_hitbox.h"
#include "components/comp_tags.h"

GrabLongActionState::GrabLongActionState(CHandle playerHandle, CHandle hitbox)
	: GrabActionState::GrabActionState(playerHandle, hitbox) {
}

void GrabLongActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerHandle.getOwner();
	if (entity != playerEntity) {
		//Si es enemigo transicionamos a propel long
		CEntity *otherEntity = entity;

		TCompTags* tag = otherEntity->get<TCompTags>();
		if (tag && tag->hasTag(getID("enemy"))) {
			otherEntity->sendMsg(TMsgGrabbed{ playerEntity });
			dbg("Grab Long\n");
			GetPlayer()->grabTarget = entity;
			GetPlayer()->SetAttackState(TCompPlayerModel::ActionStates::PropelLong);
		}
	}
}