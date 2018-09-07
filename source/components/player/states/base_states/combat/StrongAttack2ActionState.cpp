#include "mcv_platform.h"
#include "StrongAttack2ActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"
#include "modules/system_modules/slash/comp_slash.h"


StrongAttack2ActionState::StrongAttack2ActionState(StateManager * stateManager) :
	GroundedActionState(stateManager, StrongAttack2),
	AttackState(stateManager) {
	hitboxOutTime = frames2sec(10);
	hitEndTime = frames2sec(7);
	animationEndTime = frames2sec(63);
	cancelableTime = frames2sec(2);
	interruptibleTime = frames2sec(34);
	walkableTime = frames2sec(65);
	hitbox = "strong_attack2";
}

void StrongAttack2ActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;

	AttackState::update(delta);

	if (phase == AttackPhases::Startup) {
		//posicionamiento
		bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;

		if (hasInput) {
			VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
			rotatePlayerTowards(delta, targetPos, 14.f);
		}
	}

	float acceleration = 100.f;
	float maxSpeed = 30.f;
	float deceleration = 40.f;

	if (movementTimer.elapsed() >= frames2sec(3) && movementTimer.elapsed() < frames2sec(12)) {
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

	if (!isSlashOut && movementTimer.elapsed() > frames2sec(9)) {
		isSlashOut = true;
		getSlash(SlashType::RIGHT_FOOT)->setEnable(true);
	}

	if (isSlashOut && movementTimer.elapsed() > frames2sec(24)) {
		isSlashOut = false;
		getSlash(SlashType::RIGHT_FOOT)->stopEmitting();
	}

	if (!isUltraSlashOut && phase == Active) {
		isUltraSlashOut = true;
		slash("data/particles/slash_clockwise.particles", VEC3(0.f, 1.f, 0.f), deg2rad(80), deg2rad(-90));
	}
}

void StrongAttack2ActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	//dbg("Strong 2\n");
	getSkeleton()->executeAction(animation, 0.0f, 0.2f);
	*velocityVector = VEC3::Zero;
	stateManager->changeConcurrentState(Free);
	movementTimer.reset();
	isSlashOut = false;
	isUltraSlashOut = false;
}

void StrongAttack2ActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	getSkeleton()->removeAction(animation, 0.2f);
	isSlashOut = false;
	isUltraSlashOut = false;
	getSlash(SlashType::RIGHT_FOOT)->stopEmitting();
}

void StrongAttack2ActionState::onStrongAttackButton() {
	if (isInterruptible()) {
		stateManager->changeState(StrongAttack3);
	}
}

void StrongAttack2ActionState::onFastAttackButton() {
	if (isInterruptible()) {
		stateManager->changeState(FastFinisher2);
	}
}

void StrongAttack2ActionState::setMovementInput(VEC2 input) {
	IActionState::setMovementInput(input);
	if (input.Length() > PAD_DEAD_ZONE) {
		if (canWalk()) stateManager->changeState(Walk);
	}
}

void StrongAttack2ActionState::onDodgeButton() {
	if (isCancelable() || isInterruptible()) {
		GroundedActionState::onDodgeButton();
	}
}

void StrongAttack2ActionState::onReleasePowerButton() {
	if (isInterruptible()) {
		GroundedActionState::onReleasePowerButton();
	}
}

void StrongAttack2ActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity *otherEntity = entity;
	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });
	TMsgAttackHit msgAttackHit = {};
	msgAttackHit.attacker = playerEntity;
	msgAttackHit.info = {};
	VEC3 launchVelocity = getPlayerTransform()->getFront() * launchSpeed.x;
	launchVelocity.y = launchSpeed.y;
	msgAttackHit.info.verticalLauncher = new AttackInfo::VerticalLauncher{
		suspensionTime,
		launchVelocity
	};
	msgAttackHit.info.stun = new AttackInfo::Stun{ 2.2f };
	msgAttackHit.info.givesPower = true;
	msgAttackHit.info.damage = damage;
	otherEntity->sendMsg(msgAttackHit);

}