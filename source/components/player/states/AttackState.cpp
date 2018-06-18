#include "mcv_platform.h"
#include "AttackState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/player/states/StateManager.h"
#include "components/comp_slash.h"


AttackState::AttackState(StateManager* stateManager) {
	_stateManager = stateManager;
}

void AttackState::update(float delta) {
	CEntity* slashEntity = getEntityByName("slash01");
	TCompSlash* slash = slashEntity->get<TCompSlash>();
	if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		if (!_stateManager->isChangingBaseState) {
			_stateManager->changeState(Idle);
		}
	}
	else if (phase == AttackPhases::Active && timer.elapsed() >= hitEndTime) {
		timer.reset();
		getAttackHitboxes()->disable(hitbox);
		phase = AttackPhases::Recovery;
		slash->stopEmitting();
	}
	else if (phase == AttackPhases::Startup && timer.elapsed() >= hitboxOutTime) {
		timer.reset();
		getAttackHitboxes()->enable(hitbox);
		phase = AttackPhases::Active;
	}
}

void AttackState::onStateEnter(IActionState * lastState) {
	phase = AttackPhases::Startup;
	timer.reset();
	cancelTimer.reset();
	invulnerableTimer.reset();
	CEntity* slashEntity = getEntityByName("slash01");
	TCompSlash* slash = slashEntity->get<TCompSlash>();
	slash->setEnable(true);

}

void AttackState::onStateExit(IActionState * nextState) {
	getAttackHitboxes()->disable(hitbox);
	CEntity* slashEntity = getEntityByName("slash01");
	TCompSlash* slash = slashEntity->get<TCompSlash>();
	slash->stopEmitting();
}

bool AttackState::isCancelable() {
	if (cancelTimer.elapsed() <= cancelableTime) {
		return true;
	}
	return false;
}

bool AttackState::isInterruptible() {
	if (cancelTimer.elapsed() >= interruptibleTime) {
		return true;
	}
	return false;
}

bool AttackState::hasInvulnerability() {
	if (invulnerableTimer.elapsed() >= invulnerabilityStartTime && invulnerableTimer.elapsed() < invulnerabilityEndTime) {
		return true;
	}
	return false;
}

bool AttackState::hasSuperArmor() {
	if (invulnerableTimer.elapsed() >= superarmorStartTime && invulnerableTimer.elapsed() < superarmorEndTime) {
		return true;
	}
	return false;
}

TCompHitboxes * AttackState::getAttackHitboxes() {
	return _stateManager->getHitboxes();
}