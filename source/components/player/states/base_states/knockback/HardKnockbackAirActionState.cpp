#include "mcv_platform.h"
#include "HardKnockbackAirActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/comp_power_gauge.h"
#include "entity/common_msgs.h"

HardKnockbackAirActionState::HardKnockbackAirActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
	animation = "recibedanio";
}

void HardKnockbackAirActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	if (timer.elapsed() >= duration) {
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Idle);
	}
}

void HardKnockbackAirActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);
	*velocityVector = VEC3::Zero;
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	timer.reset();
	//s�lo entra en este estado si tiene stun
	duration = getPlayerModel()->getReceivedAttack()->stun->duration;
}

void HardKnockbackAirActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
}


