#include "mcv_platform.h"
#include "GrabHighActionState.h"
#include "components/comp_hitbox.h"
#include "components/comp_tags.h"

GrabHighActionState::GrabHighActionState(TCompPlayerModel * player, CHandle hitbox)
	: GrabActionState::GrabActionState(player, hitbox) {
}

void GrabHighActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerHandle = CHandle(player).getOwner();
	if (entity != playerHandle) {
		//Si es enemigo transicionamos a propel high
		CEntity *otherEntity = entity;

		TCompTags* tag = otherEntity->get<TCompTags>();
		if (tag && tag->hasTag(getID("enemy"))) {
			otherEntity->sendMsg(TMsgGrabbed{ playerHandle});
			dbg("Grab High\n");
			TCompTransform* enemyTransform = otherEntity->get<TCompTransform>();
			player->grabTarget = enemyTransform->getPosition() + VEC3::Up * 2.f;
			player->SetAttackState(TCompPlayerModel::ActionStates::PropelHigh);
		}
	}
}