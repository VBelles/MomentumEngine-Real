#include "mcv_platform.h"
#include "GrabLongActionState.h"
#include "components/comp_hitbox.h"
#include "components/comp_tags.h"

GrabLongActionState::GrabLongActionState(TCompPlayerModel * player, CHandle hitbox)
	: GrabActionState::GrabActionState(player, hitbox) {
}

void GrabLongActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerHandle = CHandle(player).getOwner();
	if (entity != playerHandle) {
		//Si es enemigo transicionamos a propel long
		CEntity *otherEntity = entity;

		TCompTags* tag = otherEntity->get<TCompTags>();
		if (tag && tag->hasTag(getID("enemy"))) {
			otherEntity->sendMsg(TMsgGrabbed{ playerHandle });
			dbg("Grab Long\n");
			player->grabTarget = entity;
			player->SetAttackState(TCompPlayerModel::ActionStates::PropelLong);
		}
	}
}