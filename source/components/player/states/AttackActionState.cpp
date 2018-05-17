#include "mcv_platform.h"
#include "AttackActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/player/states/StateManager.h"


AttackActionState::AttackActionState(StateManager* stateManager) {
	_stateManager = stateManager;
}

void AttackActionState::update(float delta) {
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

void AttackActionState::onStateEnter(IActionState * lastState) {
	timer.reset();
}

void AttackActionState::onStateExit(IActionState * nextState) {
	getAttackHitboxes()->disable(hitbox);
}

TCompHitboxes * AttackActionState::getAttackHitboxes() {
	return _stateManager->getHitboxes();
}