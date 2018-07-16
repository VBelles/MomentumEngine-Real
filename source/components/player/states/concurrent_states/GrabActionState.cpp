#include "mcv_platform.h"
#include "GrabHighActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


GrabActionState::GrabActionState(StateManager* stateManager, ConcurrentState state) :
	AirborneActionState(stateManager, state) ,
	AttackState(stateManager) {
	cancelableTime = frames2sec(1);
	interruptibleTime = frames2sec(25);
}

void GrabActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		stateManager->changeConcurrentState(Free);
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
	AttackState::onStateEnter(lastState);
	phase = AttackPhases::Startup;
	timer.reset();
	getPlayerModel()->lockTurning = true;
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void GrabActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	getHitboxes()->disable(hitbox);
	getPlayerModel()->lockTurning = false;
	getSkeleton()->removeAction(animation, 0.02f);
}

void GrabActionState::onLanding() {
	getHitboxes()->disable(hitbox);
	stateManager->changeState(SoftLanding);
	stateManager->changeConcurrentState(Free);
}

