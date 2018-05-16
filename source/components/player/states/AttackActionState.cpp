#include "mcv_platform.h"
#include "AttackActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"


void AttackActionState::update(float delta) {
	if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		if (!isChangingBaseState) {
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Idle);
		}
	}
	else if (phase == AttackPhases::Active && timer.elapsed() >= hitEndTime) {
		timer.reset();
		getHitboxes()->disable(hitbox);
		phase = AttackPhases::Recovery;
	}
	else if (phase == AttackPhases::Startup && timer.elapsed() >= hitboxOutTime) {
		timer.reset();
		getHitboxes()->enable(hitbox);
		phase = AttackPhases::Active;
	}
}

void AttackActionState::onStateEnter(IActionState * lastState) {
	IActionState::onStateEnter(lastState);
	timer.reset();
}

void AttackActionState::onStateExit(IActionState * nextState) {
	IActionState::onStateExit(nextState);
	getHitboxes()->disable(hitbox);
}