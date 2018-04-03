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
		if (tag && tag->hasTag(enemyId)) {
			otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });

			TMsgAttackHit msgAtackHit = {};
			msgAtackHit.attacker = playerEntity;
			msgAtackHit.info = {};
			msgAtackHit.info.givesPower = true;
			msgAtackHit.info.grab = new AttackInfo::Grab{
				lockDuration
			};
			otherEntity->sendMsg(msgAtackHit);
			GetPlayerModel()->grabTarget = entity;
			GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::PropelLong);
			GetPlayerModel()->SetConcurrentState(TCompPlayerModel::ActionStates::Idle);
		}
	}
}