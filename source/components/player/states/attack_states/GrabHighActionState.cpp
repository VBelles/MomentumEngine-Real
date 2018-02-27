#include "mcv_platform.h"
#include "GrabHighActionState.h"
#include "components/comp_hitbox.h"

GrabHighActionState::GrabHighActionState(TCompPlayerModel * player, CHandle hitbox)
	: GrabActionState::GrabActionState(player, hitbox) {
}

void GrabHighActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerHandle = CHandle(player).getOwner();
	if (entity != playerHandle) {
		//Si es enemigo transicionamos a propel high
		CEntity *otherEntity = entity;
		otherEntity->sendMsg(TMsgAttackHit{ playerHandle, damage });
		dbg("Grab High\n", damage);
	}
}