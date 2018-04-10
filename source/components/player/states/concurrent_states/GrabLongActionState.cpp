#include "mcv_platform.h"
#include "GrabLongActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitbox.h"
#include "components/comp_tags.h"

GrabLongActionState::GrabLongActionState(CHandle playerModelHandle, CHandle hitbox)
	: GrabActionState::GrabActionState(playerModelHandle, hitbox) {
}

void GrabLongActionState::onHitboxEnter(CHandle entity) {
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
			getPlayerModel()->grabTarget = entity;
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::PropelLong);
			getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
		}
	}
}