#include "mcv_platform.h"
#include "ReleasePowerGroundActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "components/player/comp_power_gauge.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"


void ReleasePowerGroundActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		if (!isChangingBaseState) {
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Idle);
		}
	}
	else if (phase == AttackPhases::Active && timer.elapsed() >= hitEndTime) {
		timer.reset();
		//Deshabilitar ambas y ya est�
		getHitboxes()->disable(smallHitbox);
		getHitboxes()->disable(bigHitbox);
		phase = AttackPhases::Recovery;
	}
	else if (phase == AttackPhases::Startup && timer.elapsed() >= hitboxOutTime) {
		timer.reset();
		//Depende de buttonPresses y del nivel de poder sacar� una hitbox u otra
		switch (getPlayerModel()->getPowerGauge()->getPowerLevel()) {
		case 1:
			getPlayerModel()->getPowerGauge()->releasePower();
			break;
		case 2:
			getPlayerModel()->getPowerGauge()->releasePower();
			getHitboxes()->enable(smallHitbox);
			if (buttonPresses > 1) getPlayerModel()->getPowerGauge()->releasePower();
			break;
		case 3:
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
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void ReleasePowerGroundActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	getHitboxes()->disable(smallHitbox);
	getHitboxes()->disable(bigHitbox);
}

void ReleasePowerGroundActionState::onReleasePowerButton() {
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


void ReleasePowerGroundActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	CEntity *otherEntity = entity;
	TMsgAttackHit msgAtackHit = {};
	msgAtackHit.attacker = playerEntity;
	msgAtackHit.info = {};
	msgAtackHit.info.givesPower = false;
	msgAtackHit.info.damage = damage;
	msgAtackHit.info.stun = new AttackInfo::Stun{stunDuration};
	msgAtackHit.info.activatesMechanism = true;
	otherEntity->sendMsg(msgAtackHit);
}