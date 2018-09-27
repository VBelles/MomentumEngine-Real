#include "mcv_platform.h"
#include "FallingAttackActionState.h"
#include "HardLandingActionState.h"
#include "modules/system_modules/slash/comp_slash.h"


FallingAttackActionState::FallingAttackActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, FallingAttack),
	AttackState(stateManager) {
	cancelableTime = frames2sec(16);
	superarmorStartTime = frames2sec(0);
	superarmorEndTime = frames2sec(600);
}

void FallingAttackActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;

	if (timer.elapsed() < hitboxOutTime) {
		//posicionamiento
		bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;
		PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();

		if (hasInput) {
			VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
			rotatePlayerTowards(delta, targetPos, 10.f);
		}
		velocityVector->y = 0.f;
		deltaMovement.y = upwardsVelocity * delta;
	}
	else {
		//ataque ca�da
		if (timer.elapsed() >= hitboxOutTime) {
			getHitboxes()->enable(hitbox);
		}
		getPlayerModel()->setGravityMultiplier(fallingMultiplier);
	}

	if (!isSlashOut && movementTimer.elapsed() > frames2sec(12)) {
		isSlashOut = true;
		getTrailSlash(SlashType::LEFT_TENTACLE)->setEnable(true);
		getTrailSlash(SlashType::RIGHT_TENTACLE)->setEnable(true);
		getTrailSlash(SlashType::LEFT_TENTACLE_SHORT)->setEnable(true);
		getTrailSlash(SlashType::RIGHT_TENTACLE_SHORT)->setEnable(true);
	}
}

void FallingAttackActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	getPlayerModel()->maxVerticalSpeed = maxFallingVelocity;
	getPlayerModel()->setGravity(0.f);
	*velocityVector = VEC3::Zero;
	timer.reset();
	getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
	getSkeleton()->executeAction(animationPositioning, 0.2f, 0.2f);
	movementTimer.reset();
	isSlashOut = false;
}

void FallingAttackActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	getSkeleton()->removeAction(animation, 0.2f);
	getSkeleton()->removeAction(animationPositioning, 0.2f);
	if (nextState->state != HardLanding) {
		getHitboxes()->disable(hitbox);
	}
	getPlayerModel()->resetGravity();
	isSlashOut = false;
	getTrailSlash(SlashType::LEFT_TENTACLE)->stopEmitting();
	getTrailSlash(SlashType::RIGHT_TENTACLE)->stopEmitting();
	getTrailSlash(SlashType::LEFT_TENTACLE_SHORT)->stopEmitting();
	getTrailSlash(SlashType::RIGHT_TENTACLE_SHORT)->stopEmitting();
}

void FallingAttackActionState::onDodgeButton() {
	if (isCancelable()) {
		AirborneActionState::onDodgeButton();
	}
}

void FallingAttackActionState::onLanding() {
	//getHitboxes()->disable(hitbox); //que la deshabilite HardLanding
	*velocityVector = VEC3::Zero;
	stateManager->changeState(HardLanding);
	stateManager->changeConcurrentState(Free);
	getPlayerModel()->lockFallingAttack = false;
}


void FallingAttackActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	//Para y su velocidad se transmite al enemigo
	*velocityVector = VEC3::Zero;
	//getPlayerModel()->lockFallingAttack = true;
	stateManager->changeState(AirborneNormal);

	CHandle playerEntity = getPlayerEntity();
	CEntity *otherEntity = entity;

	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });
	VEC3 propelVelocity = { 0, -maxFallingVelocity, 0 };
	TMsgAttackHit msgAttackHit = {};
	msgAttackHit.attacker = playerEntity;
	msgAttackHit.info = {};
	msgAttackHit.info.givesPower = true;
	msgAttackHit.info.damage = damage;
	msgAttackHit.info.propel = new AttackInfo::Propel{ propelDuration, propelVelocity };
	msgAttackHit.info.stun = new AttackInfo::Stun{ stunTime };
	otherEntity->sendMsg(msgAttackHit);
}
