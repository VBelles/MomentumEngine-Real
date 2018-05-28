#include "mcv_platform.h"
#include "SpendCoinsActionState.h"
#include "components/player/comp_player_model.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_transform.h"
#include "components/player/states/StateManager.h"


SpendCoinsActionState::SpendCoinsActionState(StateManager* stateManager) :
	GroundedActionState(stateManager, SpendCoins) {
}

void SpendCoinsActionState::update(float delta) {
	/*deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;*/
	if (buttonReleased) {
		stateManager->changeState(Idle);
	}
}

void SpendCoinsActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	spendTimer.reset();
	buttonReleased = false;
	*velocityVector = VEC3::Zero;
	getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
	dbg("spend coins\n");
}

void SpendCoinsActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	dbg("exit spend coins\n");
}

void SpendCoinsActionState::onSpendCoinsButtonReleased() {
	buttonReleased = true;
}
