#include "mcv_platform.h"
#include "SoftKnockbackAirActionState.h"


SoftKnockbackAirActionState::SoftKnockbackAirActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, SoftKnockbackAir) {
}

void SoftKnockbackAirActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	if (timer.elapsed() >= duration) {
		stateManager->changeConcurrentState(Free);
	}
}

void SoftKnockbackAirActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	timer.reset();
	if (dynamic_cast<AttackState*>(stateManager->getState())) {
		stateManager->changeState(AirborneNormal);
	}
	//bloquear ataques
	getPlayerModel()->lockAttack = true;
}

void SoftKnockbackAirActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
	//desbloquear ataques
	getPlayerModel()->lockAttack = false;
}


