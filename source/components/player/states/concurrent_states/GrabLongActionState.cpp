#include "mcv_platform.h"
#include "GrabLongActionState.h"

GrabLongActionState::GrabLongActionState(StateManager* stateManager) :
	GrabActionState(stateManager, GrabLong) {
}

void GrabLongActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = getPlayerEntity();
	CEntity *otherEntity = entity;

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
	stateManager->changeState(PropelLong);
	stateManager->changeConcurrentState(Free);


}
