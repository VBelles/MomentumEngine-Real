#include "mcv_platform.h"
#include "DummyActionState.h"


DummyActionState::DummyActionState(StateManager* stateManager)
	: IActionState(stateManager, Dummy) {
};

void DummyActionState::update(float delta) {

}

void DummyActionState::onStateEnter(IActionState * lastState) {
	IActionState::onStateEnter(lastState);
	dbg("dummy state\n");
}

void DummyActionState::onStateExit(IActionState * nextState) {
	IActionState::onStateExit(nextState);
}