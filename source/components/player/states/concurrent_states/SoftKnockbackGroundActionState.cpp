#include "mcv_platform.h"
#include "SoftKnockbackGroundActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/comp_power_gauge.h"
#include "entity/common_msgs.h"

SoftKnockbackGroundActionState::SoftKnockbackGroundActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	animation = "ataquecaida";
}

void SoftKnockbackGroundActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	if (timer.elapsed() >= duration) {
		getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
	}
}

void SoftKnockbackGroundActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	timer.reset();
	//bloquear ataques
}

void SoftKnockbackGroundActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
	//desbloquear ataques
}


