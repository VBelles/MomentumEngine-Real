#include "mcv_platform.h"
#include "WallJumpPlummetActionState.h"
#include "components/player/states/base_states/FallingAttackLandingActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"

void WallJumpPlummetActionState::update(float delta) {
	deltaMovement = VEC3::Zero;

	VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
	if (deceleration * delta < horizontalVelocity.Length()) {
		deltaMovement = calculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			-VEC3(velocityVector->x, 0, velocityVector->z), deceleration, 45.f);

		transferVelocityToDirectionAndAccelerate(delta, false, -VEC3(velocityVector->x, 0, velocityVector->z), deceleration);
	}

	deltaMovement = *velocityVector * delta;
	if (!isChangingBaseState) {
		if (timer.elapsed() >= endingTime) {
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
		}
	}
}

void WallJumpPlummetActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	getPlayerModel()->setGravityMultiplier(0.f);
	//Se calcula cada vez que se entra, ya que depende del poder
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	endingTime = currentPowerStats->plummetTime;
	getHitboxes()->enable(hitbox);
	timer.reset();
	getPlayerModel()->getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
}

void WallJumpPlummetActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	clampHorizontalVelocity(currentPowerStats->maxHorizontalSpeed);

	if (!dynamic_cast<FallingAttackLandingActionState*>(nextState)) {
		getHitboxes()->disable(hitbox); //que la deshabilite LandingFallingAttack si es posible
	}

}

void WallJumpPlummetActionState::onJumpHighButton() {
	//Se recupera y agarra
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::GrabHigh);
}

void WallJumpPlummetActionState::onJumpLongButton() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::GrabLong);
}

void WallJumpPlummetActionState::onFastAttackButton() {
	//Se recupera
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
}

void WallJumpPlummetActionState::onStrongAttackButton() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
}

void WallJumpPlummetActionState::onLanding() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::LandingFallingAttack);
}

void WallJumpPlummetActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	CEntity *otherEntity = entity;
	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });
	TMsgAttackHit msgAtackHit = {};
	msgAtackHit.attacker = playerEntity;
	msgAtackHit.info = {};
	msgAtackHit.info.damage = damage;
	msgAtackHit.info.givesPower = true;
	msgAtackHit.info.propel = new AttackInfo::Propel{ *velocityVector };
	otherEntity->sendMsg(msgAtackHit);

}
