#include "mcv_platform.h"
#include "GrabLongActionState.h"
#include "components/comp_hitbox.h"
#include "components/comp_tags.h"

GrabLongActionState::GrabLongActionState(CHandle playerModelHandle, CHandle hitbox)
	: GrabActionState::GrabActionState(playerModelHandle, hitbox) {
}

void GrabLongActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		//Si es enemigo transicionamos a propel long
		CEntity *otherEntity = entity;

		TCompTags* tag = otherEntity->get<TCompTags>();
		if (tag && tag->hasTag(getID("enemy"))) {
			otherEntity->sendMsg(TMsgGrabbed{ playerEntity });
			dbg("Grab Long\n");
			GetPlayerModel()->grabTarget = entity;
			GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::PropelLong);
		}
	}
}