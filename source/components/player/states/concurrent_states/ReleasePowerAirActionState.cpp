#include "mcv_platform.h"
#include "ReleasePowerAirActionState.h"
#include "components/comp_hitbox.h"

ReleasePowerAirActionState::ReleasePowerAirActionState(CHandle playerModelHandle, CHandle hitboxSmall, CHandle hitboxBig)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
	hitboxSmallHandle = hitboxSmall;
	hitboxBigHandle = hitboxBig;
}

void ReleasePowerAirActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		GetPlayerModel()->SetConcurrentState(TCompPlayerModel::ActionStates::Idle);
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
				GetPlayerModel()->GetPowerGauge()->ReleasePower();
				break;
			case 2:
				GetPlayerModel()->GetPowerGauge()->ReleasePower();
				hitboxSmall->enable();
				if (buttonPresses > 1) GetPlayerModel()->GetPowerGauge()->ReleasePower();
				break;
			case 3:
				GetPlayerModel()->GetPowerGauge()->ReleasePower();
				if (buttonPresses > 1) {
					GetPlayerModel()->GetPowerGauge()->ReleasePower();
					//bola grande
					hitboxBig->enable();
					if (buttonPresses > 2) GetPlayerModel()->GetPowerGauge()->ReleasePower();
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

void ReleasePowerAirActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	phase = AttackPhases::Startup;
	buttonPresses = 1;
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = activeFrames * (1.f / 60);
	animationEndTime = endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	timer.reset();
}

void ReleasePowerAirActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	CEntity *hitboxEntity = hitboxSmallHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
	hitboxEntity = hitboxBigHandle;
	hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
	GetPlayerModel()->baseState->SetPose();
}

void ReleasePowerAirActionState::OnReleasePowerButton() {
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
		GetPlayerModel()->GetPowerGauge()->ReleasePower();
	}
	if (phase == AttackPhases::Recovery) GetPlayerModel()->GetPowerGauge()->ReleasePower();
}

void ReleasePowerAirActionState::OnLanding() {
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::Landing);
	GetPlayerModel()->SetConcurrentState(TCompPlayerModel::ActionStates::Idle);
}

void ReleasePowerAirActionState::OnHitboxEnter(CHandle entity) {
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