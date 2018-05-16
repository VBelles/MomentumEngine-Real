#include "mcv_platform.h"
#include "GrabLongActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitbox.h"
#include "entity/common_msgs.h"
#include "components/player/states/StateManager.h"

GrabLongActionState::GrabLongActionState(StateManager* stageManager) :
	GrabActionState(stateManager, GrabLong) {
}

void GrabLongActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
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
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::PropelLong);
	getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);


}