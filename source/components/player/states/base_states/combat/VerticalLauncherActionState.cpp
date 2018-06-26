#include "mcv_platform.h"
#include "VerticalLauncherActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"
#include "components/comp_transform.h"

VerticalLauncherActionState::VerticalLauncherActionState(StateManager * stateManager) :
	LauncherActionState(stateManager, VerticalLauncher, "vertical_launcher", "vertical_launcher") {
	hitboxOutTime = frames2sec(18);
	hitEndTime = frames2sec(7);
	animationEndTime = frames2sec(35);
	cancelableTime = frames2sec(15);
	interruptibleTime = frames2sec(30);
	walkableTime = frames2sec(44);
}

void VerticalLauncherActionState::update(float delta) {
	LauncherActionState::update(delta);
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