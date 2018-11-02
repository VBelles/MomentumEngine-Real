#include "mcv_platform.h"
#include "GrabHighActionState.h"
#include "components/player/states/base_states/wall_jump/HuggingWallActionState.h"


GrabHighActionState::GrabHighActionState(StateManager* stateManager) :
	GrabActionState(stateManager, GrabHigh) {
}

void GrabHighActionState::onStateEnter(IActionState * lastState) {
	GrabActionState::onStateEnter(lastState);
	(dynamic_cast<HuggingWallActionState*>(stateManager->getState(HuggingWall)))->bufferWallJump(true);
}

void GrabHighActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
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
	stateManager->changeState(PropelHigh);
	stateManager->changeConcurrentState(Free);


}
