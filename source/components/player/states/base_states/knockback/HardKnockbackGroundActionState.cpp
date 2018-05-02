#include "mcv_platform.h"
#include "HardKnockbackGroundActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/comp_power_gauge.h"

HardKnockbackGroundActionState::HardKnockbackGroundActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	animation = "jump_volando";
}

void HardKnockbackGroundActionState::update (float delta) {
	if (timer.elapsed() >= duration) {
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Idle);
	}
}

void HardKnockbackGroundActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);

	getPlayerModel()->getSkeleton()->blendCycle(animation);
	timer.reset();
}

void HardKnockbackGroundActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
}


