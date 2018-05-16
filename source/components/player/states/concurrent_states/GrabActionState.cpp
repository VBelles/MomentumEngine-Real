#include "mcv_platform.h"
#include "GrabHighActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "skeleton/comp_skeleton.h"



GrabActionState::GrabActionState(StateManager* stateManager, ConcurrentState state) :
	AirborneActionState(stateManager, state) {
}

void GrabActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
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

void GrabActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	phase = AttackPhases::Startup;
	timer.reset();
	getPlayerModel()->lockTurning = true;
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void GrabActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	getHitboxes()->disable(hitbox);
	getPlayerModel()->lockTurning = false;
}

void GrabActionState::onLanding() {
	getHitboxes()->disable(hitbox);
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Landing);
	getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
}

