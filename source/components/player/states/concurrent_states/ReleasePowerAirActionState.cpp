#include "mcv_platform.h"
#include "ReleasePowerAirActionState.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "components/postfx/comp_render_blur_radial.h"
#include "components/comp_camera.h"
#include "components/comp_collider.h"
#include "components/player/comp_power_gauge.h"
#include "components/player/comp_player_model.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"

ReleasePowerAirActionState::ReleasePowerAirActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, ReleasePowerAir),
	AttackState(stateManager) {
	cancelableTime = frames2sec(6);
	interruptibleTime = frames2sec(43);
	invulnerabilityStartTime = frames2sec(7);
	invulnerabilityEndTime = frames2sec(10);
	superarmorStartTime = frames2sec(0);
	superarmorEndTime = frames2sec(7);
}

void ReleasePowerAirActionState::update(float delta) {
	PxExtendedVec3 cctPos = getCollider()->controller->getPosition();
	getBlurRadial()->setCenterInWorldCoordinates(PhysxUtils::toVec3(cctPos));
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		stateManager->changeConcurrentState(Free);
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
		Particles::LaunchConfig releasePowerLaunchConfig{ getEntity(), "", VEC3(0.f, 0.75f, 0.f) };
		//Depende de buttonPresses y del nivel de poder sacará una hitbox u otra
		switch (getPowerGauge()->getPowerLevel()) {
		case 1:
			getPowerGauge()->releasePower();
			break;
		case 2:
			EngineParticles.launchSystem(PARTICLES_RELEASE_POWER, releasePowerLaunchConfig);
			getBlurRadial()->setEnable(true);
			getPowerGauge()->releasePower();
			getHitboxes()->enable(smallHitbox);
			if (buttonPresses > 1) getPowerGauge()->releasePower();
			break;
		case 3:
			EngineParticles.launchSystem(PARTICLES_RELEASE_POWER, releasePowerLaunchConfig);
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

void ReleasePowerAirActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	phase = AttackPhases::Startup;
	buttonPresses = 1;
	timer.reset();
	getPlayerModel()->lockTurning = true;
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void ReleasePowerAirActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	getHitboxes()->disable(smallHitbox);
	getHitboxes()->disable(bigHitbox);
	getPlayerModel()->lockTurning = false;
	getBlurRadial()->setEnable(false);
}

void ReleasePowerAirActionState::onReleasePowerButton() {
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

void ReleasePowerAirActionState::onDamage(const TMsgAttackHit & msg) {
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
		AirborneActionState::onDamage(msg);
	}
}

void ReleasePowerAirActionState::onLanding() {
	stateManager->changeState(SoftLanding);
	stateManager->changeConcurrentState(Free);
}


void ReleasePowerAirActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity *otherEntity = entity;
	TMsgAttackHit msgAtackHit = {};
	msgAtackHit.attacker = playerEntity;
	msgAtackHit.info = {};
	msgAtackHit.info.givesPower = false;
	msgAtackHit.info.damage = damage;
	msgAtackHit.info.stun = new AttackInfo::Stun{ stunDuration };
	msgAtackHit.info.activatesMechanism = true;
	otherEntity->sendMsg(msgAtackHit);
}