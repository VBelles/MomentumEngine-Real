#include "mcv_platform.h"
#include "FastFinisher1ActionState.h"
#include "modules/system_modules/slash/comp_slash.h"


FastFinisher1ActionState::FastFinisher1ActionState(StateManager * stateManager) :
	GroundedActionState(stateManager, FastFinisher1),
	AttackState(stateManager) {
	hitboxOutTime = frames2sec(24);
	hitEndTime = frames2sec(11);
	animationEndTime = frames2sec(65);
	cancelableTime = frames2sec(5);
	interruptibleTime = frames2sec(75);
	walkableTime = frames2sec(75);
	hitbox = "fast_finisher1";
	superarmorStartTime = frames2sec(14);
	superarmorEndTime = frames2sec(24);
	invulnerabilityStartTime = frames2sec(24);
	invulnerabilityEndTime = frames2sec(35);
}

void FastFinisher1ActionState::update(float delta) {
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
	float maxSpeed = 25.f;
	float deceleration = 8.f;

	if (movementTimer.elapsed() >= frames2sec(22) && movementTimer.elapsed() < frames2sec(34)) {
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

	
	if (!isSlashOut && movementTimer.elapsed() > frames2sec(24) && movementTimer.elapsed() <= frames2sec(54)) {
		isSlashOut = true;
		getTrailSlash(SlashType::RIGHT_FOOT)->setEnable(true);
		EngineSound.emitEvent(SOUND_ATTACK_MOVEMENT, getPlayerTransform());
	}
	if (isSlashOut && movementTimer.elapsed() > frames2sec(54)) {
		isSlashOut = false;
		getTrailSlash(SlashType::RIGHT_FOOT)->stopEmitting();
	}
}

void FastFinisher1ActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	//dbg("Fast Finisher 1\n");
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	*velocityVector = VEC3::Zero;
	stateManager->changeConcurrentState(Free);
	movementTimer.reset();
	isSlashOut = false;
}

void FastFinisher1ActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	getSkeleton()->removeAction(animation, 0.2f);
	isSlashOut = false;
	getTrailSlash(SlashType::RIGHT_FOOT)->stopEmitting();
}

void FastFinisher1ActionState::setMovementInput(VEC2 input) {
	IActionState::setMovementInput(input);
	if (input.Length() > PAD_DEAD_ZONE) {
		if (canWalk()) stateManager->changeState(Walk);
	}
}

void FastFinisher1ActionState::onDodgeButton() {
	if (isCancelable() || isInterruptible()) GroundedActionState::onDodgeButton();
}

void FastFinisher1ActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = getPlayerEntity();
	CEntity *otherEntity = entity;
	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });
	TMsgAttackHit msgAttackHit = {};
	msgAttackHit.attacker = playerEntity;
	msgAttackHit.info = {};
	//VEC3 launchVelocity = getPlayerTransform()->getFront() * launchSpeed.x;
	//launchVelocity.y = launchSpeed.y;
	VEC3 launchVelocity = getPlayerTransform()->getFront() * getPlayerModel()->getPowerStats()->longJumpVelocityVector.z;
	launchVelocity.y = getPlayerModel()->getPowerStats()->longJumpVelocityVector.y;
	msgAttackHit.info.horizontalLauncher = new AttackInfo::HorizontalLauncher{
		suspensionTime,
		launchVelocity
	};
	msgAttackHit.info.gravityMultiplier = getPlayerModel()->getPowerStats()->longGravityMultiplier;
	msgAttackHit.info.stun = new AttackInfo::Stun{ stunTime };
	msgAttackHit.info.givesPower = true;
	msgAttackHit.info.damage = damage;
	otherEntity->sendMsg(msgAttackHit);
}
