#include "mcv_platform.h"
#include "FastFinisher2ActionState.h"
#include "modules/system_modules/slash/comp_slash.h"


FastFinisher2ActionState::FastFinisher2ActionState(StateManager * stateManager) :
	GroundedActionState(stateManager, FastFinisher2),
	AttackState(stateManager) {
	hitboxOutTime = frames2sec(30);
	hitEndTime = frames2sec(13);
	animationEndTime = frames2sec(65);
	cancelableTime = frames2sec(3);
	interruptibleTime = frames2sec(85);
	walkableTime = frames2sec(85);
	hitbox = "fast_finisher2";
	superarmorStartTime = frames2sec(14);
	superarmorEndTime = frames2sec(30);
	invulnerabilityStartTime = frames2sec(30);
	invulnerabilityEndTime = frames2sec(43);
}

void FastFinisher2ActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;

	AttackState::update(delta);

	if (phase == AttackPhases::Startup) {
		//posicionamiento
		bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;

		if (hasInput) {
			VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
			rotatePlayerTowards(delta, targetPos, 10.f);
		}
	}

	float acceleration = 100.f;
	float maxSpeed = 30.f;
	float deceleration = 10.f;

	if (movementTimer.elapsed() >= frames2sec(7) && movementTimer.elapsed() < frames2sec(20)) {
		deltaMovement += -getPlayerTransform()->getFront() * 2.5F * delta;
	}

	if (movementTimer.elapsed() >= frames2sec(27) && movementTimer.elapsed() < frames2sec(35)) {
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

	if (!isSlashOut && movementTimer.elapsed() > frames2sec(32)) {
		isSlashOut = true;
		getTrailSlash(SlashType::LEFT_TENTACLE)->setEnable(true);
		getTrailSlash(SlashType::RIGHT_TENTACLE)->setEnable(true);
	}

	if (isSlashOut && movementTimer.elapsed() > frames2sec(54)) {
		isSlashOut = false;
		getTrailSlash(SlashType::LEFT_TENTACLE)->stopEmitting();
		getTrailSlash(SlashType::RIGHT_TENTACLE)->stopEmitting();
	}

	if (!isUltraSlashOut && movementTimer.elapsed() > frames2sec(45)) {
		isUltraSlashOut = true;
		getParticles()->launch("fast_finisher_2_right");
		getParticles()->launch("fast_finisher_2_left");
		getSound()->play("attack");
		//getSound()->play("finisher");
	}
}

void FastFinisher2ActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	//dbg("Fast Finisher 2\n");
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	*velocityVector = VEC3::Zero;
	stateManager->changeConcurrentState(Free);
	movementTimer.reset();
	isSlashOut = false;
	isUltraSlashOut = false;
}

void FastFinisher2ActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	getSkeleton()->removeAction(animation, 0.2f);
	isSlashOut = false;
	isUltraSlashOut = false;
	getTrailSlash(SlashType::LEFT_TENTACLE)->stopEmitting();
	getTrailSlash(SlashType::RIGHT_TENTACLE)->stopEmitting();
}

void FastFinisher2ActionState::setMovementInput(VEC2 input) {
	IActionState::setMovementInput(input);
	if (input.Length() > PAD_DEAD_ZONE) {
		if (canWalk()) stateManager->changeState(Walk);
	}
}

void FastFinisher2ActionState::onDodgeButton() {
	if (isCancelable() || isInterruptible()) GroundedActionState::onDodgeButton();
}

void FastFinisher2ActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = getPlayerEntity();
	CEntity *otherEntity = entity;
	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });
	TMsgAttackHit msgAttackHit = {};
	msgAttackHit.attacker = playerEntity;
	msgAttackHit.info = {};
	VEC3 launchVelocity = getPlayerTransform()->getFront() * launchSpeed.x;
	launchVelocity.y = launchSpeed.y;
	msgAttackHit.info.horizontalLauncher = new AttackInfo::HorizontalLauncher{
		suspensionTime,
		launchVelocity
	};
	msgAttackHit.info.stun = new AttackInfo::Stun{ stunTime };
	msgAttackHit.info.givesPower = true;
	msgAttackHit.info.damage = damage;
	otherEntity->sendMsg(msgAttackHit);
}
