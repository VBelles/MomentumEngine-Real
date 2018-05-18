#include "mcv_platform.h"
#include "AttackState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/player/states/StateManager.h"


AttackState::AttackState(StateManager* stateManager) {
	_stateManager = stateManager;
}

void AttackState::update(float delta) {
	if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		if (!_stateManager->isChangingBaseState) {
			_stateManager->changeState(Idle);
		}
	}
	else if (phase == AttackPhases::Active && timer.elapsed() >= hitEndTime) {
		timer.reset();
		getAttackHitboxes()->disable(hitbox);
		phase = AttackPhases::Recovery;
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
}

void AttackState::onStateExit(IActionState * nextState) {
	getAttackHitboxes()->disable(hitbox);
}

TCompHitboxes * AttackState::getAttackHitboxes() {
	return _stateManager->getHitboxes();
}