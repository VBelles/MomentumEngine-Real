#include "mcv_platform.h"
#include "HardKnockbackGroundActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/comp_power_gauge.h"
#include "entity/common_msgs.h"

HardKnockbackGroundActionState::HardKnockbackGroundActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	animation = "jump_volando";
}

void HardKnockbackGroundActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	if (timer.elapsed() >= duration) {
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Idle);
	}
}

void HardKnockbackGroundActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);
	*velocityVector = VEC3::Zero;
	getPlayerModel()->getSkeleton()->blendCycle(animation);
	timer.reset();
	//sólo entra en este estado si tiene stun
	duration = getPlayerModel()->getReceivedAttack()->stun->duration;
}

void HardKnockbackGroundActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
}


