#include "mcv_platform.h"
#include "SoftKnockbackAirActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/comp_power_gauge.h"
#include "entity/common_msgs.h"
#include "components/player/states/StateManager.h"
#include "components/player/states/AttackState.h"


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


