#include "mcv_platform.h"
#include "SpendCoinsActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_transform.h"
#include "components/player/states/StateManager.h"


SpendCoinsActionState::SpendCoinsActionState(StateManager* stateManager) :
	GroundedActionState(stateManager, SpendCoins) {
}

void SpendCoinsActionState::update(float delta) {
	//if button released exit
}

void SpendCoinsActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	spendTimer.reset();
	dbg("spend coins\n");
}

void SpendCoinsActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	dbg("exit spend coins\n");
}