#include "mcv_platform.h"
#include "StrongAttack3ActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"
#include "modules/system_modules/slash/comp_slash.h"


StrongAttack3ActionState::StrongAttack3ActionState(StateManager * stateManager) :
	GroundedActionState(stateManager, StrongAttack3),
	AttackState(stateManager) {
	hitboxOutTime = frames2sec(18);
	hitEndTime = frames2sec(17);
	animationEndTime = frames2sec(40);
	cancelableTime = frames2sec(17);
	interruptibleTime = frames2sec(70);
	walkableTime = frames2sec(70);
	hitbox = "strong_attack3";
	superarmorStartTime = frames2sec(14);
	superarmorEndTime = frames2sec(18);
	invulnerabilityStartTime = frames2sec(18);
	invulnerabilityEndTime = frames2sec(29);
}

void StrongAttack3ActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;

	AttackState::update(delta);

	bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;

	if (phase == AttackPhases::Startup || phase == AttackPhases::Active) {
		//posicionamiento

		if (hasInput) {
			VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
			rotatePlayerTowards(delta, targetPos, 10.f);
		}
	}

	float acceleration = 100.f;
	float maxSpeed = 10.f;
	float deceleration = 40.f;

	if (hasInput && movementTimer.elapsed() >= frames2sec(8) && movementTimer.elapsed() < frames2sec(35)) {

		VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);

		deltaMovement += calculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			desiredDirection, acceleration,
			maxSpeed);

		transferVelocityToDirectionAndAccelerate(delta, true, desiredDirection, acceleration);
		clampHorizontalVelocity(maxSpeed);
	}
	else {
		VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
		if (deceleration * delta < horizontalVelocity.Length()) {
			deltaMovement = calculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
				-VEC3(velocityVector->x, 0, velocityVector->z), deceleration, maxSpeed);

			transferVelocityToDirectionAndAccelerate(delta, false, -VEC3(velocityVector->x, 0, velocityVector->z), deceleration);
		}
		else {
			velocityVector->x = 0.f;
			velocityVector->z = 0.f;
		}
	}

	if (!isSlashOut && movementTimer.elapsed() > frames2sec(8)) {
		isSlashOut = true;
		getTrailSlash(SlashType::LEFT_TENTACLE)->setEnable(true);
		getTrailSlash(SlashType::LEFT_TENTACLE_SHORT)->setEnable(true);
	}

	if (isSlashOut && movementTimer.elapsed() > frames2sec(35)) {
		isSlashOut = false;
		getTrailSlash(SlashType::LEFT_TENTACLE)->stopEmitting();
		getTrailSlash(SlashType::LEFT_TENTACLE_SHORT)->stopEmitting();
	}

	if (!isUltraSlash1Out && movementTimer.elapsed() > frames2sec(18)) {
		isUltraSlash1Out = true;
		slash(SLASH_STRONG_3, VEC3(0, 1.3f, 0));
		EngineSound.emitEvent(SOUND_ATTACK_MOVEMENT, getPlayerTransform());
	}
	if (!isUltraSlash2Out && movementTimer.elapsed() > frames2sec(25)) {
		isUltraSlash2Out = true;
		slash(SLASH_STRONG_3, VEC3(0, 1.3f, 0));
		EngineSound.emitEvent(SOUND_ATTACK_MOVEMENT, getPlayerTransform());
	}
	if (!isUltraSlash3Out && movementTimer.elapsed() > frames2sec(32)) {
		isUltraSlash3Out = true;
		slash(SLASH_STRONG_3, VEC3(0, 1.3f, 0));
		EngineSound.emitEvent(SOUND_ATTACK_MOVEMENT, getPlayerTransform());
	}
}

void StrongAttack3ActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	//dbg("Strong 3\n");
	getSkeleton()->executeAction(animation, 0.0f, 0.2f);
	*velocityVector = VEC3::Zero;
	stateManager->changeConcurrentState(Free);
	movementTimer.reset();
	isSlashOut = false;
	isUltraSlash1Out = false;
	isUltraSlash2Out = false;
	isUltraSlash3Out = false;
}

void StrongAttack3ActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState); 
	getSkeleton()->removeAction(animation, 0.2f);
	isSlashOut = false;
	isUltraSlash1Out = false;
	isUltraSlash2Out = false;
	isUltraSlash3Out = false;
	getTrailSlash(SlashType::LEFT_TENTACLE)->stopEmitting();
	getTrailSlash(SlashType::LEFT_TENTACLE_SHORT)->stopEmitting();
}

void StrongAttack3ActionState::setMovementInput(VEC2 input) {
	IActionState::setMovementInput(input);
	if (input.Length() > PAD_DEAD_ZONE) {
		if (canWalk()) stateManager->changeState(Walk);
	}
}

void StrongAttack3ActionState::onDodgeButton() {
	if (isCancelable() || isInterruptible()) GroundedActionState::onDodgeButton();
}

void StrongAttack3ActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity* otherEntity = entity;
	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });
	TMsgAttackHit msgAttackHit = {};
	msgAttackHit.attacker = playerEntity;
	msgAttackHit.info = {};
	TCompTransform* otherTransform = otherEntity->get<TCompTransform>();
	VEC3 launchVelocity = otherTransform->getPosition() - getPlayerTransform()->getPosition();
	launchVelocity.Normalize();
	launchVelocity *= launchSpeed.x;
	launchVelocity.y = launchSpeed.y;
	msgAttackHit.info.horizontalLauncher = new AttackInfo::HorizontalLauncher{
		suspensionTime,
		launchVelocity
	};
	msgAttackHit.info.stun = new AttackInfo::Stun{ 3.0f };
	msgAttackHit.info.givesPower = true;
	msgAttackHit.info.damage = damage;
	otherEntity->sendMsg(msgAttackHit);

}