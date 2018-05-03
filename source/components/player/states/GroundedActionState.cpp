#include "mcv_platform.h"
#include "GroundedActionState.h"
#include "components/player/comp_player_model.h"


void GroundedActionState::update(float delta) {

}

void GroundedActionState::onStateEnter(IActionState * lastState) {
	IActionState::onStateEnter(lastState);
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	if (currentPowerStats) getPlayerModel()->maxVerticalSpeed = currentPowerStats->maxVelocityVertical;
	getPlayerModel()->resetGravity();
	backwardsMaxDotProduct = cos(backwardsdMinAngle);
	getPlayerModel()->lastWallEntered = nullptr;//En realidad al tocar el suelo ya se sobreescribe la variable
}

void GroundedActionState::onStateExit(IActionState * nextState) {
	IActionState::onStateExit(nextState);
	getPlayerModel()->lastWallEntered = nullptr;
	getPlayerModel()->lastWallNormal = PxVec3(0,0,0);
}

void GroundedActionState::onJumpHighButton() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquat);
}

void GroundedActionState::onJumpLongButton() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void GroundedActionState::onStrongAttackButton() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::StrongAttack);
}

void GroundedActionState::onFastAttackButton() {
	if (getPlayerModel()->isConcurrentActionFree()) {
		getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::FastAttack);
	}
}

void GroundedActionState::onReleasePowerButton() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::ReleasePowerGround);
}

void GroundedActionState::onLeavingGround() {
	//Set state a alguno por defecto, luego las clases derivadas de esta ya sabrán qué hacer
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void GroundedActionState::onDamage(float damage, bool isHard) {
	IActionState::onDamage(damage, isHard);
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::HardKnockbackGround);
}
