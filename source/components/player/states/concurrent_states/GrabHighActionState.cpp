#include "mcv_platform.h"
#include "GrabHighActionState.h"
#include "components/player/comp_player_model.h"
#include "entity/common_msgs.h"
#include "components/player/states/StateManager.h"


GrabHighActionState::GrabHighActionState(StateManager* stateManager) :
	GrabActionState(stateManager, GrabHigh) {
}

void GrabHighActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
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
	stateManager->changeState(PropelHigh);
	stateManager->changeConcurrentState(Free);


}
