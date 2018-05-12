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


void ReleasePowerAirActionState::update(float delta) {
	PxExtendedVec3 cctPos = getCollider()->controller->getPosition();
	getBlurRadial()->setCenterInWorldCoordinates(VEC3(cctPos.x, cctPos.y, cctPos.z));
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
	}
	else if (phase == AttackPhases::Active && timer.elapsed() >= hitEndTime) {
		timer.reset();
		//Deshabilitar ambas y ya est�
		getHitboxes()->disable(smallHitbox);
		getHitboxes()->disable(bigHitbox);
		phase = AttackPhases::Recovery;
		getBlurRadial()->setEnable(false);
	}
	else if (phase == AttackPhases::Startup && timer.elapsed() >= hitboxOutTime) {
		timer.reset();
		//Depende de buttonPresses y del nivel de poder sacar� una hitbox u otra
		switch (getPlayerModel()->getPowerGauge()->getPowerLevel()) {
		case 1:
			getPlayerModel()->getPowerGauge()->releasePower();
			break;
		case 2:
			getBlurRadial()->setEnable(true);
			getPlayerModel()->getPowerGauge()->releasePower();
			getHitboxes()->enable(smallHitbox);
			if (buttonPresses > 1) getPlayerModel()->getPowerGauge()->releasePower();
			break;
		case 3:
			getBlurRadial()->setEnable(true);
			getPlayerModel()->getPowerGauge()->releasePower();
			if (buttonPresses > 1) {
				getPlayerModel()->getPowerGauge()->releasePower();
				//bola grande
				getHitboxes()->enable(bigHitbox);
				if (buttonPresses > 2) getPlayerModel()->getPowerGauge()->releasePower();
			}
			else {
				//bola peque�a
				getHitboxes()->enable(smallHitbox);
			}
			break;
		}
		phase = AttackPhases::Active;
	}
}

void ReleasePowerAirActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	phase = AttackPhases::Startup;
	buttonPresses = 1;
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = activeFrames * (1.f / 60);
	animationEndTime = endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	timer.reset();
	getPlayerModel()->lockTurning = true;
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void ReleasePowerAirActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	getHitboxes()->disable(smallHitbox);
	getHitboxes()->disable(bigHitbox);
	getPlayerModel()->lockTurning = false;
	getBlurRadial()->setEnable(false);
}

void ReleasePowerAirActionState::onReleasePowerButton() {
	buttonPresses++;
	//Si est� en active, release energy
	if (phase == AttackPhases::Active) {
		//si adem�s button presses es == 2 y ssj2, agrandar bola
		if (getPlayerModel()->getPowerGauge()->getPowerLevel() == 2) {
			getHitboxes()->disable(smallHitbox);
			getHitboxes()->enable(bigHitbox);

		}
		getPlayerModel()->getPowerGauge()->releasePower();
	}
	if (phase == AttackPhases::Recovery) getPlayerModel()->getPowerGauge()->releasePower();
}

void ReleasePowerAirActionState::onLanding() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Landing);
	getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
}


void ReleasePowerAirActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
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