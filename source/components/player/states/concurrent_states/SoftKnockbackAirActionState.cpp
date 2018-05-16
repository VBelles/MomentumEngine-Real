#include "mcv_platform.h"
#include "SoftKnockbackAirActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/comp_power_gauge.h"
#include "entity/common_msgs.h"

SoftKnockbackAirActionState::SoftKnockbackAirActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
	animation = "ataquecaida";
}

void SoftKnockbackAirActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	if (timer.elapsed() >= duration) {
		getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
	}
}

void SoftKnockbackAirActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	timer.reset();
	//bloquear ataques
}

void SoftKnockbackAirActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
	//desbloquear ataques
}


