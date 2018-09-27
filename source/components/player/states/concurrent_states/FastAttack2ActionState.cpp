#include "mcv_platform.h"
#include "FastAttack2ActionState.h"


FastAttack2ActionState::FastAttack2ActionState(StateManager* stateManager)
	: GroundedActionState(stateManager, FastAttack2),
	AttackState(stateManager) {
	cancelableTime = frames2sec(9);;
	interruptibleTime = frames2sec(26);
}

void FastAttack2ActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	if (phase == AttackPhases::Launch && timer.elapsed() >= beginLauncherTime) {
		stateManager->changeConcurrentState(Free);
		stateManager->changeState(HorizontalLauncher);
	}
	else if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		stateManager->changeConcurrentState(Free);
	}
	else if (phase == AttackPhases::Active && timer.elapsed() >= hitEndTime) {
		timer.reset();
		getHitboxes()->disable(hitbox);
		phase = AttackPhases::Recovery;
		slash(SLASH_CLOCKWISE, VEC3(0, 1, 0));
	}
	else if (phase == AttackPhases::Startup && timer.elapsed() >= hitboxOutTime) {
		timer.reset();
		getHitboxes()->enable(hitbox);
		phase = AttackPhases::Active;
		EngineSound.emitEvent(SOUND_ATTACK_MOVEMENT, getPlayerTransform());
	}
}

void FastAttack2ActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	//dbg("Fast attack 2\n");
	phase = AttackPhases::Launch;
	timer.reset();
	getPlayerModel()->lockWalk = false;
	getPlayerModel()->lockAttack = true;
	EngineParticles.launchSystem(PARTICLES_PLAYER_ATTACK, { getPlayerEntity(), "Bip001 L Hand", {0.12f, 0.f, 0.f} });
}

void FastAttack2ActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	getHitboxes()->disable(hitbox);
	getPlayerModel()->lockAttack = false;
	getPlayerModel()->lockWalk = false;
	getSkeleton()->removeAction(animation, 0.2f);
}

void FastAttack2ActionState::onFastAttackButton() {
	if (isInterruptible()) stateManager->changeConcurrentState(FastAttack);
}

void FastAttack2ActionState::onStrongAttackButton() {
	if (isInterruptible()) stateManager->changeState(StrongFinisher2);
}

void FastAttack2ActionState::onDodgeButton() {
	if (isCancelable() || isInterruptible()) stateManager->changeState(Dodge);
}

void FastAttack2ActionState::onReleasePowerButton() {
	if (isInterruptible()) GroundedActionState::onReleasePowerButton();
}

void FastAttack2ActionState::onFastAttackButtonReleased() {
	if (phase == AttackPhases::Launch) {
		phase = AttackPhases::Startup;
		getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	}
}

void FastAttack2ActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = getPlayerEntity();
	CEntity* otherEntity = entity;

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
	//EngineParticles.launchSystem("data/particles/player/attack_hit.particles", { getEntity(), "Bip001 L Hand", {0.4f, 0.f, 0.f} });

}
