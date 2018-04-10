#include "mcv_platform.h"
#include "GhostJumpWindowActionState.h"

GhostJumpWindowActionState::GhostJumpWindowActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void GhostJumpWindowActionState::update (float delta) {
	if (timer.elapsed() >= squatTime) {
		//Como estamos ya en el aire, hacemos el cambio nosotros mismos
		GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	AirborneActionState::update(delta);
}

void GhostJumpWindowActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	squatTime = frameWindow * (1.f / 60);
	timer.reset();
	enteringVelocity = GetPlayerModel()->GetVelocityVector()->Length();
}

void GhostJumpWindowActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
}

void GhostJumpWindowActionState::OnJumpHighButton() {
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	velocityVector->y = 0.f;
	*velocityVector += currentPowerStats->jumpVelocityVector;
	//Como estamos ya en el aire, hacemos el cambio nosotros mismos
	GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
}

void GhostJumpWindowActionState::OnJumpLongButton() {
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	*velocityVector = GetPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
	velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
	GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneLong);
}
