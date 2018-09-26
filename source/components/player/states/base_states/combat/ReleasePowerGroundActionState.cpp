#include "mcv_platform.h"
#include "ReleasePowerGroundActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "components/player/comp_power_gauge.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/postfx/comp_render_blur_radial.h"
#include "components/comp_camera.h"
#include "components/comp_collider.h"
#include "components/player/states/StateManager.h"


ReleasePowerGroundActionState::ReleasePowerGroundActionState(StateManager* stateManager) :
	GroundedActionState(stateManager, ReleasePowerGround),
	AttackState(stateManager) {
	cancelableTime = frames2sec(6);
	interruptibleTime = frames2sec(43);
	walkableTime = frames2sec(60);
	invulnerabilityStartTime = frames2sec(7);
	invulnerabilityEndTime = frames2sec(10);
	superarmorStartTime = frames2sec(0);
	superarmorEndTime = frames2sec(7);
}

void ReleasePowerGroundActionState::update(float delta) {
	PxExtendedVec3 cctPos = getCollider()->controller->getPosition();
	getBlurRadial()->setCenterInWorldCoordinates(VEC3((float)cctPos.x, (float)cctPos.y, (float)cctPos.z));
	deltaMovement = VEC3::Zero;
	if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		if (!stateManager->isChangingBaseState) {
			stateManager->changeState(Idle);
		}
	}
	else if (phase == AttackPhases::Active && timer.elapsed() >= hitEndTime) {
		timer.reset();
		//Deshabilitar ambas y ya está
		getHitboxes()->disable(smallHitbox);
		getHitboxes()->disable(bigHitbox);
		phase = AttackPhases::Recovery;
		getBlurRadial()->setEnable(false);
	}
	else if (phase == AttackPhases::Startup && timer.elapsed() >= hitboxOutTime) {
		timer.reset();
		//Depende de buttonPresses y del nivel de poder sacará una hitbox u otra
		Particles::LaunchConfig releasePowerLaunchConfig{ getEntity(), "", VEC3(0.f, 0.75f, 0.f) };
		switch (getPowerGauge()->getPowerLevel()) {
		case 1:
			getPowerGauge()->releasePower();
			break;
		case 2:
			EngineParticles.launchSystem(PARTICLES_RELEASE_POWER, releasePowerLaunchConfig);
			EngineSound.emitEvent(SOUND_RELEASE_POWER);
			getBlurRadial()->setEnable(true);
			getPowerGauge()->releasePower();
			getHitboxes()->enable(smallHitbox);
			if (buttonPresses > 1) getPowerGauge()->releasePower();
			break;
		case 3:
			EngineParticles.launchSystem(PARTICLES_RELEASE_POWER, releasePowerLaunchConfig);
			EngineSound.emitEvent(SOUND_RELEASE_POWER);
			getBlurRadial()->setEnable(true);
			getPowerGauge()->releasePower();
			if (buttonPresses > 1) {
				getPowerGauge()->releasePower();
				//bola grande
				getHitboxes()->enable(bigHitbox);
				if (buttonPresses > 2) getPowerGauge()->releasePower();
			}
			else {
				//bola pequeña
				getHitboxes()->enable(smallHitbox);
			}
			break;
		}
		phase = AttackPhases::Active;
	}
}

void ReleasePowerGroundActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	phase = AttackPhases::Startup;
	stateManager->changeConcurrentState(Free);
	*velocityVector = VEC3::Zero;
	buttonPresses = 1;
	timer.reset();
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void ReleasePowerGroundActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	getHitboxes()->disable(smallHitbox);
	getHitboxes()->disable(bigHitbox);
	getBlurRadial()->setEnable(false);
	getSkeleton()->removeAction(animation, 0.2f);
}

void ReleasePowerGroundActionState::setMovementInput(VEC2 input) {
	IActionState::setMovementInput(input);
	if (input.Length() > PAD_DEAD_ZONE) {
		if (canWalk()) stateManager->changeState(Walk);
	}
}

void ReleasePowerGroundActionState::onDodgeButton() {
	if (isCancelable() || isInterruptible()) GroundedActionState::onDodgeButton();
}

void ReleasePowerGroundActionState::onReleasePowerButton() {
	buttonPresses++;
	//Si está en active, release energy
	if (phase == AttackPhases::Active) {
		//si además button presses es == 2 y ssj2, agrandar bola
		if (getPowerGauge()->getPowerLevel() == 2) {
			getHitboxes()->disable(smallHitbox);
			getHitboxes()->enable(bigHitbox);
		}
		getPowerGauge()->releasePower();
	}
	if (phase == AttackPhases::Recovery) getPowerGauge()->releasePower();
}

void ReleasePowerGroundActionState::onDamage(const TMsgAttackHit & msg) {
	if (hasInvulnerability()) {
		//enviar mensaje a enemigo conforme ha sido esquivado finamente
		dbg("Can't touch this! Hammer Time!\n");
		CEntity* attacker = msg.attacker.getOwner();
		attacker->sendMsg(TMsgPerfectDodged{});
	}
	else if (hasSuperArmor()) {
		dbg("super armor!\n");
		IActionState::onDamage(msg);
	}
	else {
		GroundedActionState::onDamage(msg);
	}
}


void ReleasePowerGroundActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity *otherEntity = entity;
	TMsgAttackHit msgAtackHit = {};
	msgAtackHit.attacker = playerEntity;
	msgAtackHit.info = {};
	msgAtackHit.info.givesPower = false;
	msgAtackHit.info.damage = damage;
	msgAtackHit.info.stun = new AttackInfo::Stun{ stunTime };
	msgAtackHit.info.activatesMechanism = true;
	otherEntity->sendMsg(msgAtackHit);
}