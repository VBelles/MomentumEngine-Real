#include "mcv_platform.h"
#include "StrongAttackActionState.h"
#include "components/player/states/base_states/moving_around/RunActionState.h"
#include "modules/system_modules/slash/comp_slash.h"


StrongAttackActionState::StrongAttackActionState(StateManager * stateManager) :
	GroundedActionState(stateManager, StrongAttack),
	AttackState(stateManager) {
	hitboxOutTime = frames2sec(30);
	hitEndTime = frames2sec(26);
	animationEndTime = frames2sec(47);
	cancelableTime = frames2sec(29);
	interruptibleTime = frames2sec(57);
	walkableTime = frames2sec(90);
	hitbox = "strong_attack";
	superarmorStartTime = frames2sec(14);//1 frame menos de lo que tarda en salir el ataque m�s r�pido de un dreidel
	superarmorEndTime = frames2sec(30);
	invulnerabilityStartTime = frames2sec(30);
	invulnerabilityEndTime = frames2sec(56);
}

void StrongAttackActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;

	if (phase == AttackPhases::Launch && timer.elapsed() >= beginLauncherTime) {
		if (!stateManager->isChangingBaseState) {
			stateManager->changeState(VerticalLauncher);
		}
	}
	else {
		AttackState::update(delta);
	}

	bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;
	if (phase == AttackPhases::Startup || phase == AttackPhases::Launch) {
		//posicionamiento

		if (hasInput) {
			VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
			rotatePlayerTowards(delta, targetPos, 3.f);
		}
	}

	float acceleration = 100.f;
	float maxSpeed = 10.f;
	float deceleration = 12.f;

	if (fromRun) {
		maxSpeed = 30.f;
	}

	if (phase == AttackPhases::Launch && fromRun) {
		deceleration = 1.f;
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

	if (movementTimer.elapsed() > frames2sec(30) && movementTimer.elapsed() < frames2sec(38)) {
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



	if (!isSlashOut && movementTimer.elapsed() > frames2sec(30) && movementTimer.elapsed() <= frames2sec(50)) {
		isSlashOut = true;
		getTrailSlash(SlashType::LEFT_TENTACLE)->setEnable(true);
		getTrailSlash(SlashType::RIGHT_TENTACLE)->setEnable(true);
		EngineSound.emitEvent(SOUND_ATTACK_MOVEMENT, getPlayerTransform());
	}

	if (isSlashOut && movementTimer.elapsed() > frames2sec(50)) {
		isSlashOut = false;
		getTrailSlash(SlashType::LEFT_TENTACLE)->stopEmitting();
		getTrailSlash(SlashType::RIGHT_TENTACLE)->stopEmitting();
	}

}

void StrongAttackActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	//dbg("Strong 1\n");
	phase = AttackPhases::Launch;
	stateManager->changeConcurrentState(Free);
	movementTimer.reset();
	getSkeleton()->blendCycle(animationIdle, 0.4f, 0.4f);
	fromRun = lastState->state == Run ? true : false;
	if (!fromRun) *velocityVector = VEC3::Zero;
	isSlashOut = false;
}

void StrongAttackActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	getSkeleton()->removeAction(animation, 0.2f);
	isSlashOut = false;
	getTrailSlash(SlashType::LEFT_TENTACLE)->stopEmitting();
	getTrailSlash(SlashType::RIGHT_TENTACLE)->stopEmitting();
}

void StrongAttackActionState::onStrongAttackButton() {
	if (isInterruptible()) stateManager->changeState(StrongAttack2);
}

void StrongAttackActionState::onStrongAttackButtonReleased() {
	if (phase == AttackPhases::Launch) {
		phase = AttackPhases::Startup;
		*velocityVector = VEC3::Zero;
		getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	}
}

void StrongAttackActionState::onFastAttackButton() {
	if (isInterruptible()) stateManager->changeState(FastFinisher1);
}

void StrongAttackActionState::setMovementInput(VEC2 input) {
	IActionState::setMovementInput(input);
	if (input.Length() > PAD_DEAD_ZONE) {
		if (canWalk()) stateManager->changeState(Walk);
	}
}

void StrongAttackActionState::onDodgeButton() {
	if (isCancelable() || isInterruptible()) GroundedActionState::onDodgeButton();
}

void StrongAttackActionState::onReleasePowerButton() {
	if (isInterruptible()) GroundedActionState::onReleasePowerButton();
}

void StrongAttackActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = getPlayerEntity();
	CEntity *otherEntity = entity;
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
	msgAttackHit.info.stun = new AttackInfo::Stun{ stunTime };
	msgAttackHit.info.givesPower = true;
	msgAttackHit.info.damage = damage;
	otherEntity->sendMsg(msgAttackHit);

}
