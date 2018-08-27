#include "mcv_platform.h"
#include "HardKnockbackAirActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/comp_power_gauge.h"
#include "entity/common_msgs.h"
#include "components/player/states/StateManager.h"
#include "components/player/states/base_states/LandingHurtActionState.h"



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
	stateManager->changeConcurrentState(Free);
	getSkeleton()->blendCycle(animationLoop, 0.2f, 0.2f);
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	timer.reset();
	//sólo entra en este estado si tiene stun
	duration = getPlayerModel()->getReceivedAttack()->stun->duration;
}

void HardKnockbackAirActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
}

void HardKnockbackAirActionState::onMove(MoveState& moveState) {
	//Touching top
	if (moveState.isTouchingTop && velocityVector->y > 0.f) {
		velocityVector->y = 0.f;
	}
	//Land or slide
	if (moveState.isTouchingBot) {
		if (!isWalkable(moveState)) {
			if (slideWindowTimer.elapsed() >= slideWindowTime) {
				stateManager->changeState(Slide);
				getPlayerModel()->lockFallingAttack = false;
				getPlayerModel()->lockAirDodge = false;
				*velocityVector = VEC3::Zero;
			}
		}
		else {
			((LandingHurtActionState*)stateManager->getState(LandingHurt))->setRemainingTime(duration - timer.elapsed());
			stateManager->changeState(LandingHurt);
		}
	}
	else {
		slideWindowTimer.reset();
	}
}

