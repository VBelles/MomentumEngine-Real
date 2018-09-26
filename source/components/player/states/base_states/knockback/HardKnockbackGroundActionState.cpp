#include "mcv_platform.h"
#include "HardKnockbackGroundActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/comp_power_gauge.h"
#include "components/player/states/StateManager.h"
#include "entity/common_msgs.h"


HardKnockbackGroundActionState::HardKnockbackGroundActionState(StateManager* stateManager) :
	GroundedActionState(stateManager, HardKnockbackGround) {
}


void HardKnockbackGroundActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	if (timer.elapsed() >= duration) {
		stateManager->changeState(Idle);
	}
}

void HardKnockbackGroundActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);
	*velocityVector = VEC3::Zero; 
	getSkeleton()->blendCycle(animationLoop, 0.2f, 0.1f);
	//getSkeleton()->executeAction(animation, 0.2f, 0.0f);
	timer.reset();
	//s�lo entra en este estado si tiene stun
	duration = getPlayerModel()->getReceivedAttack()->stun->duration;
}

void HardKnockbackGroundActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
}


