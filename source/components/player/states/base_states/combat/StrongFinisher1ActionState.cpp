#include "mcv_platform.h"
#include "StrongFinisher1ActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"
#include "modules/system_modules/slash/comp_slash.h"


StrongFinisher1ActionState::StrongFinisher1ActionState(StateManager * stateManager) :
	GroundedActionState(stateManager, StrongFinisher1),
	AttackState(stateManager) {
	hitboxOutTime = frames2sec(24);
	hitEndTime = frames2sec(6);
	animationEndTime = frames2sec(41);
	cancelableTime = frames2sec(15);
	interruptibleTime = frames2sec(50);
	walkableTime = frames2sec(60);
	hitbox = "strong_finisher1";
}

void StrongFinisher1ActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;

	AttackState::update(delta);

	if (phase == AttackPhases::Startup) {
		//posicionamiento
		bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;

		if (hasInput) {
			VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
			rotatePlayerTowards(delta, targetPos, 3.f);
		}
	}

	float acceleration = 100.f;
	float maxSpeed = 25.f;
	float deceleration = 40.f;

	if (movementTimer.elapsed() >= frames2sec(0) && movementTimer.elapsed() < frames2sec(5)) {
		//deltaMovement += getPlayerTransform()->getFront() * maxSpeed * delta;
		deltaMovement += calculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			getPlayerTransform()->getFront(), acceleration,
			maxSpeed);

		transferVelocityToDirectionAndAccelerate(delta, true, getPlayerTransform()->getFront(), acceleration);
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

	if (!isSlashOut && movementTimer.elapsed() > frames2sec(20)) {
		isSlashOut = true;
		getTrailSlash(SlashType::LEFT_HAND)->setEnable(true);
		getTrailSlash(SlashType::RIGHT_HAND)->setEnable(true);
	}

	if (isSlashOut && movementTimer.elapsed() > frames2sec(30)) {
		isSlashOut = false;
		getTrailSlash(SlashType::LEFT_HAND)->stopEmitting();
		getTrailSlash(SlashType::RIGHT_HAND)->stopEmitting();
	}

}

void StrongFinisher1ActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	//dbg("Strong Finisher 1\n");
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	*velocityVector = VEC3::Zero;
	stateManager->changeConcurrentState(Free);
	movementTimer.reset();
	isSlashOut = false;
}

void StrongFinisher1ActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	getSkeleton()->removeAction(animation, 0.2f);
	isSlashOut = false;
	getTrailSlash(SlashType::LEFT_HAND)->stopEmitting();
	getTrailSlash(SlashType::RIGHT_HAND)->stopEmitting();
}

void StrongFinisher1ActionState::setMovementInput(VEC2 input) {
	IActionState::setMovementInput(input);
	if (input.Length() > PAD_DEAD_ZONE) {
		if (canWalk()) stateManager->changeState(Walk);
	}
}

void StrongFinisher1ActionState::onJumpHighButton() {
	if (canWalk()) GroundedActionState::onJumpHighButton();
}

void StrongFinisher1ActionState::onJumpLongButton() {
	if (canWalk()) GroundedActionState::onJumpLongButton();
}

void StrongFinisher1ActionState::onDodgeButton() {
	if (isCancelable() || isInterruptible()) GroundedActionState::onDodgeButton();
}

void StrongFinisher1ActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity *otherEntity = entity;
	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });
	TMsgAttackHit msgAttackHit = {};
	msgAttackHit.attacker = playerEntity;
	msgAttackHit.info = {};
	msgAttackHit.info.givesPower = true;
	msgAttackHit.info.damage = damage;
	msgAttackHit.info.verticalLauncher = new AttackInfo::VerticalLauncher{
		suspensionTime,
		getPlayerModel()->getPowerStats()->jumpVelocityVector
	};
	msgAttackHit.info.stun = new AttackInfo::Stun{ 2.2f };
	otherEntity->sendMsg(msgAttackHit);
}