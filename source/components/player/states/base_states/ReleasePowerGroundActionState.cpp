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
		GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::Idle);
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
		switch (GetPlayerModel()->GetPowerGauge()->powerLevel) {
		case 1:
			GetPlayerModel()->GetPowerGauge()->releasePower();
			break;
		case 2:
			GetPlayerModel()->GetPowerGauge()->releasePower();
			hitboxSmall->enable();
			if (buttonPresses > 1) GetPlayerModel()->GetPowerGauge()->releasePower();
			break;
		case 3:
			GetPlayerModel()->GetPowerGauge()->releasePower();
			if (buttonPresses > 1) {
				GetPlayerModel()->GetPowerGauge()->releasePower();
				//bola grande
				hitboxBig->enable();
				if (buttonPresses > 2) GetPlayerModel()->GetPowerGauge()->releasePower();
			}
			else {
				//bola pequeña
				hitboxSmall->enable();
			}
			break;
		}
		SetPose();
		phase = AttackPhases::Active;
	}
}

void ReleasePowerGroundActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	phase = AttackPhases::Startup;
	*velocityVector = VEC3::Zero;
	buttonPresses = 1;
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = activeFrames * (1.f / 60);
	animationEndTime = endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	timer.reset();
}

void ReleasePowerGroundActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	CEntity *hitboxEntity = hitboxSmallHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
	hitboxEntity = hitboxBigHandle;
	hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
}

void ReleasePowerGroundActionState::OnReleasePowerButton() {
	buttonPresses++;
	//Si está en active, release energy
	if (phase == AttackPhases::Active) {
		//si además button presses es == 2 y ssj2, agrandar bola
		if (GetPlayerModel()->GetPowerGauge()->powerLevel == 2) {
			CEntity *hitboxEntity = hitboxSmallHandle;
			TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
			hitbox->disable();
			hitboxEntity = hitboxBigHandle;
			hitbox = hitboxEntity->get<TCompHitbox>();
			hitbox->enable();
		}
		GetPlayerModel()->GetPowerGauge()->releasePower();
	}
	if (phase == AttackPhases::Recovery) GetPlayerModel()->GetPowerGauge()->releasePower();
}

void ReleasePowerGroundActionState::OnHitboxEnter(CHandle entity) {
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