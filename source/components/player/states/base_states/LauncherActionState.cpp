#include "mcv_platform.h"
#include "LauncherActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"


LauncherActionState::LauncherActionState(StateManager* stateManager, State state, std::string animation, std::string hitbox) :
	GroundedActionState(stateManager, state),
	animation(animation),
	hitbox(hitbox) {
}

void LauncherActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
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

void LauncherActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	phase = AttackPhases::Startup;
	timer.reset();
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void LauncherActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	getHitboxes()->disable(hitbox);

}

void LauncherActionState::onLeavingGround() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}