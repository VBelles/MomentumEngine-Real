#include "mcv_platform.h"
#include "StrongFinisher2ActionState.h"
#include "modules/system_modules/slash/comp_slash.h"


StrongFinisher2ActionState::StrongFinisher2ActionState(StateManager * stateManager) :
	GroundedActionState(stateManager, StrongFinisher2),
	AttackState(stateManager) {
	hitboxOutTime = frames2sec(31);
	hitEndTime = frames2sec(13);
	animationEndTime = frames2sec(50);
	cancelableTime = frames2sec(23);
	interruptibleTime = frames2sec(50);
	walkableTime = frames2sec(55);
	hitbox = "strong_finisher2";
	superarmorStartTime = frames2sec(14);
	superarmorEndTime = frames2sec(31);
	invulnerabilityStartTime = frames2sec(31);
	invulnerabilityEndTime = frames2sec(44);
}

void StrongFinisher2ActionState::update(float delta) {
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
	float maxSpeed = 18.f;
	float deceleration = 40.f;

	if (movementTimer.elapsed() >= frames2sec(29) && movementTimer.elapsed() < frames2sec(42)) {
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

	if (!isSlashOut && movementTimer.elapsed() > frames2sec(28) && movementTimer.elapsed() <= frames2sec(42)) {
		isSlashOut = true;
		getTrailSlash(SlashType::LEFT_HAND)->setEnable(true);
		getTrailSlash(SlashType::RIGHT_HAND)->setEnable(true);
		getSound()->play("attack");
		//getSound()->play("finisher");
	}
	if (isSlashOut && movementTimer.elapsed() > frames2sec(42)) {
		isSlashOut = false;
		getTrailSlash(SlashType::LEFT_HAND)->stopEmitting();
		getTrailSlash(SlashType::RIGHT_HAND)->stopEmitting();
	}
	

}

void StrongFinisher2ActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	//dbg("Strong Finisher 2\n");
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	*velocityVector = VEC3::Zero;
	stateManager->changeConcurrentState(Free);
	movementTimer.reset();
	isSlashOut = false;
}

void StrongFinisher2ActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	getSkeleton()->removeAction(animation, 0.2f);
	isSlashOut = false;
	getTrailSlash(SlashType::LEFT_HAND)->stopEmitting();
	getTrailSlash(SlashType::RIGHT_HAND)->stopEmitting();
}

void StrongFinisher2ActionState::setMovementInput(VEC2 input) {
	IActionState::setMovementInput(input);
	if (input.Length() > PAD_DEAD_ZONE) {
		if (canWalk()) stateManager->changeState(Walk);
	}
}

void StrongFinisher2ActionState::onJumpHighButton() {
	if (canWalk()) GroundedActionState::onJumpHighButton();
}

void StrongFinisher2ActionState::onJumpLongButton() {
	if (canWalk()) GroundedActionState::onJumpLongButton();
}

void StrongFinisher2ActionState::onDodgeButton() {
	if (isCancelable() || isInterruptible()) GroundedActionState::onDodgeButton();
}

void StrongFinisher2ActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = getPlayerEntity();
	CEntity *otherEntity = entity;
	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });
	TMsgAttackHit msgAttackHit = {};
	msgAttackHit.attacker = playerEntity;
	msgAttackHit.info = {};
	VEC3 launchVelocity = getPlayerTransform()->getFront() * getPlayerModel()->getPowerStats()->longJumpVelocityVector.z;
	launchVelocity.y = getPlayerModel()->getPowerStats()->longJumpVelocityVector.y;
	msgAttackHit.info.horizontalLauncher = new AttackInfo::HorizontalLauncher{
		suspensionTime,
		launchVelocity
	};
	msgAttackHit.info.stun = new AttackInfo::Stun{ stunTime };
	msgAttackHit.info.givesPower = true;
	msgAttackHit.info.damage = damage;
	otherEntity->sendMsg(msgAttackHit);
}
