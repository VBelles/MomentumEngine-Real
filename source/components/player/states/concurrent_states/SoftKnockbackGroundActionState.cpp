#include "mcv_platform.h"
#include "SoftKnockbackGroundActionState.h"


SoftKnockbackGroundActionState::SoftKnockbackGroundActionState(StateManager* stateManager) :
	GroundedActionState(stateManager, SoftKnockbackGround) {
}

void SoftKnockbackGroundActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	if (timer.elapsed() >= duration) {
		stateManager->changeConcurrentState(Free);
	}
}

void SoftKnockbackGroundActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	timer.reset();
	if (dynamic_cast<AttackState*>(stateManager->getState())) {
		stateManager->changeState(Idle);
	}
	//bloquear ataques
	getPlayerModel()->lockAttack = true;
}

void SoftKnockbackGroundActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
	//desbloquear ataques
	getPlayerModel()->lockAttack = false;
}


