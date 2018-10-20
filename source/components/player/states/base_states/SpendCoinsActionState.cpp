#include "mcv_platform.h"
#include "SpendCoinsActionState.h"
#include "components/player/comp_collectable_manager.h"


SpendCoinsActionState::SpendCoinsActionState(StateManager* stateManager) :
	GroundedActionState(stateManager, SpendCoins) {
}

void SpendCoinsActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (spendTimer.elapsed() >= cycleTime) {
		if (getPowerGauge()->getPower() < getPowerGauge()->getMaxPower() && getCollectableManager()->spendCoins(coinsEachCycle)) {
			spendTimer.reset();
			getPowerGauge()->increasePowerInTime(powerPerCoin * coinsEachCycle, timeToIncreasePower);
		}
		else {
			stateManager->changeState(Idle);//esperar que acabe la animación
		}
	}
}

void SpendCoinsActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	spendTimer.setTimeStamp(0);//para entrar en el if en el primer frame
	*velocityVector = VEC3::Zero;
	getSkeleton()->blendCycle(animationLoop, 0.2f, 0.2f);
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	getSound()->play("spend_coins");
	dbg("spend coins\n");

}

void SpendCoinsActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	getSound()->stop("spend_coins");
	dbg("exit spend coins\n");
}

void SpendCoinsActionState::onSpendCoinsButtonReleased() {
	stateManager->changeState(Idle);//esperar que acabe la animación
}
