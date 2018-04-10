#include "mcv_platform.h"
#include "JumpSquatActionState.h"

JumpSquatActionState::JumpSquatActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void JumpSquatActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	if (timer.elapsed() >= squatTime) {
		//saltar
		getPlayerModel()->isAttachedToPlatform = false;
		velocityVector->y = isShortHop ? currentPowerStats->shortHopVelocity : currentPowerStats->jumpVelocityVector.y;
		//Dejamos que el cambio de estado se haga cuando lo detecte ground sensor
		deltaMovement = *velocityVector * delta;
	}
	else {
		bool hasInput = movementInput != VEC2::Zero;
		if (hasInput) {
			deltaMovement += getPlayerTransform()->getFront() * enteringVelocity * delta;
		}
	}
}

void JumpSquatActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	setPose();
	//dbg("Entrando en JumpSquat\n");
	squatTime = squatFrames * (1.f / 60);
	isShortHop = false;
	timer.reset();
	enteringVelocity = getPlayerModel()->getVelocityVector()->Length();
}

void JumpSquatActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	//dbg("Saliendo de JumpSquat\n");
}

void JumpSquatActionState::onJumpHighButtonReleased() {
	isShortHop = true;
}

void JumpSquatActionState::OnLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpSquat);
	}
}
