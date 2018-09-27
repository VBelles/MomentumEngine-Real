#include "mcv_platform.h"
#include "GrabHighActionState.h"


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
		getPlayerModel()->lockTurning = true;
	}
	else if (phase == AttackPhases::Startup && timer.elapsed() >= hitboxOutTime) {
		timer.reset();
		getHitboxes()->enable(hitbox);
		phase = AttackPhases::Active;
		getSound()->play("attack");
	}
}

void GrabActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	phase = AttackPhases::Startup;
	timer.reset();
	
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

