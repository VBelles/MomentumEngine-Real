#include "mcv_platform.h"
#include "JumpSquatLongActionState.h"

JumpSquatLongActionState::JumpSquatLongActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void JumpSquatLongActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	
	if (timer.elapsed() >= squatTime) {
		//saltar
		getPlayerModel()->isAttachedToPlatform = false;
		*velocityVector = getPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;
	}
}

void JumpSquatLongActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	setPose();
	//dbg("Entrando en JumpSquatLong\n");
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
}

void JumpSquatLongActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	//dbg("Saliendo de JumpSquatLong\n");
}

void JumpSquatLongActionState::onLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneLong);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpSquatLong);
	}
}
