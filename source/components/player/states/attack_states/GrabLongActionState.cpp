#include "mcv_platform.h"
#include "GrabLongActionState.h"
#include "components/comp_hitbox.h"

GrabLongActionState::GrabLongActionState(TCompPlayerModel * player, CHandle hitbox)
	: GrabActionState::GrabActionState(player, hitbox) {
}

void GrabLongActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerHandle = CHandle(player).getOwner();
	if (entity != playerHandle) {
		//Si es enemigo transicionamos a propel long
		CEntity *otherEntity = entity;
		otherEntity->sendMsg(TMsgAttackHit{ playerHandle, damage });
		dbg("Grab Long\n", damage);
	}
}