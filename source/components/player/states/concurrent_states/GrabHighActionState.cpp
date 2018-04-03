#include "mcv_platform.h"
#include "GrabHighActionState.h"
#include "components/comp_hitbox.h"
#include "components/comp_tags.h"

GrabHighActionState::GrabHighActionState(CHandle playerModelHandle, CHandle hitbox)
	: GrabActionState::GrabActionState(playerModelHandle, hitbox) {
}

void GrabHighActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		//Si es enemigo transicionamos a propel high
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
			GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::PropelHigh);
			GetPlayerModel()->SetConcurrentState(TCompPlayerModel::ActionStates::Idle);
		}
	}
}