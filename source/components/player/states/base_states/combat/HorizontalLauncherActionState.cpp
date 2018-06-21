#include "mcv_platform.h"
#include "HorizontalLauncherActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


HorizontalLauncherActionState::HorizontalLauncherActionState(StateManager * stateManager) :
	LauncherActionState(stateManager, HorizontalLauncher, "horizontal_launcher", "horizontal_launcher") {
	hitboxOutTime = frames2sec(22);
	hitEndTime = frames2sec(12);
	animationEndTime = frames2sec(26);
	cancelableTime = frames2sec(15);
	interruptibleTime = frames2sec(40);
	walkableTime = frames2sec(44);
}

void HorizontalLauncherActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity *otherEntity = entity;
	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });
	TMsgAttackHit msgAtackHit = {};
	msgAtackHit.attacker = playerEntity;
	msgAtackHit.info = {};
	msgAtackHit.info.givesPower = true;
	VEC3 launchVelocity = getPlayerTransform()->getFront() * getPlayerModel()->getPowerStats()->longJumpVelocityVector.z;
	launchVelocity.y = getPlayerModel()->getPowerStats()->longJumpVelocityVector.y;
	msgAtackHit.info.horizontalLauncher = new AttackInfo::HorizontalLauncher{
		suspensionTime,
		launchVelocity
	};
	otherEntity->sendMsg(msgAtackHit);

}