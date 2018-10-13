#include "mcv_platform.h"
#include "DummyActionState.h"


DummyActionState::DummyActionState(StateManager* stateManager)
	: IActionState(stateManager, Dummy) {
};

void DummyActionState::update(float delta) {
	if (duration <= 0) return;
	if (timer.elapsed() >= duration) {
		stateManager->changeState(AirborneNormal);
	}
}

void DummyActionState::onStateEnter(IActionState * lastState) {
	IActionState::onStateEnter(lastState);
	//se debería quitar collider, pero hay muchas cosas que lo usan
	stateManager->changeConcurrentState(Free);
	getPlayerModel()->setGravityMultiplier(0);
	timer.reset();
	if (isLoop) {
		getSkeleton()->clear(0.2f);
		getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
	}
	else {
		getSkeleton()->blendCycle(exitAnimation, 0.2f, 0.2f);
		getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	}
	dbg("dummy state\n");
}

void DummyActionState::onStateExit(IActionState * nextState) {
	IActionState::onStateExit(nextState);
	getPlayerModel()->resetGravity();
	getPlayerModel()->makeInvulnerable(0.5f);
}

void DummyActionState::setDummyState(std::string animation, bool isLoop, float duration, std::string exitAnimation) {
	this->animation = animation;
	this->isLoop = isLoop;
	this->duration = duration;
	this->exitAnimation = exitAnimation;
}
