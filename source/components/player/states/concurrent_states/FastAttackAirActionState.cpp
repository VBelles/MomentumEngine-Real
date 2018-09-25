#include "mcv_platform.h"
#include "FastAttackAirActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"
#include "modules/system_modules/slash/comp_slash.h"
#include "components/comp_transform.h"

FastAttackAirActionState::FastAttackAirActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, FastAttackAir),
	AttackState(stateManager) {
	cancelableTime = frames2sec(5);
	interruptibleTime = frames2sec(38);
}

void FastAttackAirActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		stateManager->changeConcurrentState(Free);
	}
	else if (phase == AttackPhases::Active && timer.elapsed() >= hitEndTime) {
		timer.reset();
		getHitboxes()->disable(hitbox);
		phase = AttackPhases::Recovery;
	}
	else if (phase == AttackPhases::Startup && timer.elapsed() >= hitboxOutTime) {
		timer.reset();
		getHitboxes()->enable(hitbox);
		phase = AttackPhases::Active;
		EngineSound.emitEvent(SOUND_ATTACK_MOVEMENT, *getPlayerTransform());
	}

	/*if (!isSlashOut && movementTimer.elapsed() > frames2sec(10)) {
		isSlashOut = true;
		getTrailSlash(SlashType::RIGHT_FOOT)->setEnable(true);
	}

	if (isSlashOut && movementTimer.elapsed() > frames2sec(21)) {
		isSlashOut = false;
		getTrailSlash(SlashType::RIGHT_FOOT)->stopEmitting();
	}*/

	if (!isUltraSlashOut && phase == AttackPhases::Active) {
		isUltraSlashOut = true;
		slash(SLASH_CLOCKWISE, VEC3(0, 1, 0), deg2rad(80), deg2rad(-90));
	}
}

void FastAttackAirActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	phase = AttackPhases::Startup;
	timer.reset();
	getPlayerModel()->lockWalk = false;
	getPlayerModel()->lockTurning = true;
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	movementTimer.reset();
	isSlashOut = false;
	isUltraSlashOut = false;
}

void FastAttackAirActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	getHitboxes()->disable(hitbox);
	getPlayerModel()->lockWalk = false;
	getPlayerModel()->lockTurning = false;
	getSkeleton()->removeAction(animation, 0.2f);
	isSlashOut = false;
	isUltraSlashOut = false;
	//getTrailSlash(SlashType::RIGHT_FOOT)->stopEmitting();
}

void FastAttackAirActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());

	CEntity *otherEntity = entity;

	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });

	TMsgAttackHit msgAttackHit = {};
	msgAttackHit.attacker = playerEntity;
	msgAttackHit.info = {};
	msgAttackHit.info.stun = new AttackInfo::Stun{ 1.5f };
	msgAttackHit.info.givesPower = true;
	msgAttackHit.info.damage = damage;
	otherEntity->sendMsg(msgAttackHit);
	EngineSound.emitEvent(SOUND_ATTACK_HIT, *getPlayerTransform());

}