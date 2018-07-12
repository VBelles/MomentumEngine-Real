#include "mcv_platform.h"
#include "WallJumpPlummetActionState.h"
#include "components/player/states/base_states/HardLandingActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"
#include "modules/system_modules/slash/comp_slash.h"


WallJumpPlummetActionState::WallJumpPlummetActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, WallJumpPlummet),
	AttackState(stateManager) {
}

void WallJumpPlummetActionState::update(float delta) {
	deltaMovement = VEC3::Zero;

	VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
	if (deceleration * delta < horizontalVelocity.Length()) {
		deltaMovement = calculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			-VEC3(velocityVector->x, 0, velocityVector->z), deceleration, 45.f);

		transferVelocityToDirectionAndAccelerate(delta, false, -VEC3(velocityVector->x, 0, velocityVector->z), deceleration);
	}

	deltaMovement = *velocityVector * delta;
	if (!stateManager->isChangingBaseState) {
		if (timer.elapsed() >= endingTime) {
			stateManager->changeState(AirborneNormal);
		}
	}

	if (!isSlashOut && movementTimer.elapsed() > frames2sec(2)) {
		isSlashOut = true;
		getSlash(SlashType::LEFT_HAND)->setEnable(true);
		getSlash(SlashType::RIGHT_HAND)->setEnable(true);
	}
}

void WallJumpPlummetActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	getPlayerModel()->setGravityMultiplier(0.f);
	//Se calcula cada vez que se entra, ya que depende del poder
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	endingTime = currentPowerStats->plummetTime;
	getHitboxes()->enable(hitbox);
	timer.reset();
	getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
	movementTimer.reset();
	isSlashOut = false;
}

void WallJumpPlummetActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	clampHorizontalVelocity(currentPowerStats->maxHorizontalSpeed);

	if (nextState->state != HardLanding) {
		getHitboxes()->disable(hitbox);
	}

	isSlashOut = false;
	getSlash(SlashType::LEFT_HAND)->stopEmitting();
	getSlash(SlashType::RIGHT_HAND)->stopEmitting();
}

void WallJumpPlummetActionState::onJumpHighButton() {
	//Se recupera y agarra
	stateManager->changeState(AirborneNormal);
	stateManager->changeConcurrentState(GrabHigh);
}

void WallJumpPlummetActionState::onJumpLongButton() {
	stateManager->changeState(AirborneNormal);
	stateManager->changeConcurrentState(GrabLong);
}

void WallJumpPlummetActionState::onFastAttackButton() {
	//Se recupera
	stateManager->changeState(AirborneNormal);
}

void WallJumpPlummetActionState::onStrongAttackButton() {
	stateManager->changeState(AirborneNormal);
}

void WallJumpPlummetActionState::onLanding() {
	stateManager->changeState(HardLanding);
}

void WallJumpPlummetActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity *otherEntity = entity;
	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });
	TMsgAttackHit msgAttackHit = {};
	msgAttackHit.attacker = playerEntity;
	msgAttackHit.info = {};
	msgAttackHit.info.damage = damage;
	msgAttackHit.info.givesPower = true;
	msgAttackHit.info.propel = new AttackInfo::Propel{ propelDuration, *velocityVector };
	msgAttackHit.info.stun = new AttackInfo::Stun{ 1.7f };
	otherEntity->sendMsg(msgAttackHit);

}
