#include "mcv_platform.h"
#include "GhostJumpWindowActionState.h"
#include "components/player/comp_player_model.h"

GhostJumpWindowActionState::GhostJumpWindowActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void GhostJumpWindowActionState::update (float delta) {
	if (timer.elapsed() >= squatTime) {
		//Como estamos ya en el aire, hacemos el cambio nosotros mismos
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	AirborneActionState::update(delta);
}

void GhostJumpWindowActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	squatTime = frameWindow * (1.f / 60);
	timer.reset();
	enteringVelocity = getPlayerModel()->getVelocityVector()->Length();
}

void GhostJumpWindowActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}

void GhostJumpWindowActionState::onJumpHighButton() {
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	velocityVector->y = 0.f;
	*velocityVector += currentPowerStats->jumpVelocityVector;
	//Como estamos ya en el aire, hacemos el cambio nosotros mismos
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
}

void GhostJumpWindowActionState::onJumpLongButton() {
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	*velocityVector = getPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
	velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneLong);
}
