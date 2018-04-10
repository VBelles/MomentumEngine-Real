#include "mcv_platform.h"
#include "ReleasePowerGroundActionState.h"
#include "components/comp_hitbox.h"

ReleasePowerGroundActionState::ReleasePowerGroundActionState(CHandle playerModelHandle, CHandle hitboxSmall, CHandle hitboxBig)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	hitboxSmallHandle = hitboxSmall;
	hitboxBigHandle = hitboxBig;
}

void ReleasePowerGroundActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Idle);
	}
	else if (phase == AttackPhases::Active && timer.elapsed() >= hitEndTime) {
		timer.reset();
		//Deshabilitar ambas y ya está
		CEntity *hitboxEntity = hitboxSmallHandle;
		TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
		hitbox->disable();
		hitboxEntity = hitboxBigHandle;
		hitbox = hitboxEntity->get<TCompHitbox>();
		hitbox->disable();
		phase = AttackPhases::Recovery;
	}
	else if (phase == AttackPhases::Startup && timer.elapsed() >= hitboxOutTime) {
		timer.reset();
		CEntity *hitboxSmallEntity = hitboxSmallHandle;
		TCompHitbox *hitboxSmall = hitboxSmallEntity->get<TCompHitbox>();
		CEntity *hitboxBigEntity = hitboxBigHandle;
		TCompHitbox *hitboxBig = hitboxBigEntity->get<TCompHitbox>();
		//Depende de buttonPresses y del nivel de poder sacará una hitbox u otra
		switch (getPlayerModel()->getPowerGauge()->powerLevel) {
		case 1:
			getPlayerModel()->getPowerGauge()->releasePower();
			break;
		case 2:
			getPlayerModel()->getPowerGauge()->releasePower();
			hitboxSmall->enable();
			if (buttonPresses > 1) getPlayerModel()->getPowerGauge()->releasePower();
			break;
		case 3:
			getPlayerModel()->getPowerGauge()->releasePower();
			if (buttonPresses > 1) {
				getPlayerModel()->getPowerGauge()->releasePower();
				//bola grande
				hitboxBig->enable();
				if (buttonPresses > 2) getPlayerModel()->getPowerGauge()->releasePower();
			}
			else {
				//bola pequeña
				hitboxSmall->enable();
			}
			break;
		}
		setPose();
		phase = AttackPhases::Active;
	}
}

void ReleasePowerGroundActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	phase = AttackPhases::Startup;
	*velocityVector = VEC3::Zero;
	buttonPresses = 1;
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = activeFrames * (1.f / 60);
	animationEndTime = endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	timer.reset();
}

void ReleasePowerGroundActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	CEntity *hitboxEntity = hitboxSmallHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
	hitboxEntity = hitboxBigHandle;
	hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
}

void ReleasePowerGroundActionState::onReleasePowerButton() {
	buttonPresses++;
	//Si está en active, release energy
	if (phase == AttackPhases::Active) {
		//si además button presses es == 2 y ssj2, agrandar bola
		if (getPlayerModel()->getPowerGauge()->powerLevel == 2) {
			CEntity *hitboxEntity = hitboxSmallHandle;
			TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
			hitbox->disable();
			hitboxEntity = hitboxBigHandle;
			hitbox = hitboxEntity->get<TCompHitbox>();
			hitbox->enable();
		}
		getPlayerModel()->getPowerGauge()->releasePower();
	}
	if (phase == AttackPhases::Recovery) getPlayerModel()->getPowerGauge()->releasePower();
}

void ReleasePowerGroundActionState::onHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;
		TMsgAttackHit msgAtackHit = {};
		msgAtackHit.attacker = playerEntity;
		msgAtackHit.info = {};
		msgAtackHit.info.givesPower = false;
		msgAtackHit.info.damage = damage;
		msgAtackHit.info.stun = new AttackInfo::Stun{
			stunDuration
		};
		msgAtackHit.info.activatesMechanism = true;
		otherEntity->sendMsg(msgAtackHit);
	}
}