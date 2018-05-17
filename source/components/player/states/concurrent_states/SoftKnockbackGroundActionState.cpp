#include "mcv_platform.h"
#include "SoftKnockbackGroundActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/comp_power_gauge.h"
#include "entity/common_msgs.h"
#include "components/player/states/StateManager.h"
#include "components/player/states/AttackActionState.h"


SoftKnockbackGroundActionState::SoftKnockbackGroundActionState(StateManager* stateManager) :
	GroundedActionState(stateManager, SoftKnockbackGround) {
}

void SoftKnockbackGroundActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	if (timer.elapsed() >= duration) {
		stateManager->changeConcurrentState(Free);
	}
}

void SoftKnockbackGroundActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	timer.reset();
	if (dynamic_cast<AttackActionState*>(stateManager->getState())) {
		stateManager->changeState(Idle);
	}
	//bloquear ataques
}

void SoftKnockbackGroundActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
	//desbloquear ataques
}


