#include "mcv_platform.h"
#include "HardKnockbackAirActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/comp_power_gauge.h"
#include "entity/common_msgs.h"
#include "components/player/states/StateManager.h"


HardKnockbackAirActionState::HardKnockbackAirActionState(StateManager * stateManager):
AirborneActionState(stateManager, HardKnockbackAir){
}

void HardKnockbackAirActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (timer.elapsed() >= duration) {
		stateManager->changeState(Idle);
	}
}

void HardKnockbackAirActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);
	*velocityVector = VEC3::Zero;
	getSkeleton()->blendCycle(animationLoop, 0.2f, 0.2f);
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	timer.reset();
	//sólo entra en este estado si tiene stun
	duration = getPlayerModel()->getReceivedAttack()->stun->duration;
}

void HardKnockbackAirActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
}


