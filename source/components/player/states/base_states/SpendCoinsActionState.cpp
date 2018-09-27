#include "mcv_platform.h"
#include "SpendCoinsActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/comp_power_gauge.h"
#include "components/player/comp_collectable_manager.h"
#include "components/player/states/StateManager.h"


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
	dbg("spend coins\n");

}

void SpendCoinsActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	dbg("exit spend coins\n");
}

void SpendCoinsActionState::onSpendCoinsButtonReleased() {
	stateManager->changeState(Idle);//esperar que acabe la animación
}
