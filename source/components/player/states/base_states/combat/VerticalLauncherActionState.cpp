#include "mcv_platform.h"
#include "VerticalLauncherActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"

VerticalLauncherActionState::VerticalLauncherActionState(StateManager * stateManager) :
	LauncherActionState(stateManager, VerticalLauncher, "Vlauncher", "vertical_launcher") {
}

void VerticalLauncherActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity *otherEntity = entity;
	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });
	TMsgAttackHit msgAtackHit = {};
	msgAtackHit.attacker = playerEntity;
	msgAtackHit.info = {};
	msgAtackHit.info.givesPower = true;
	msgAtackHit.info.verticalLauncher = new AttackInfo::VerticalLauncher{
		suspensionTime,
		getPlayerModel()->getPowerStats()->jumpVelocityVector
	};
	otherEntity->sendMsg(msgAtackHit);

}