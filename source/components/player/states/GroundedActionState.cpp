#include "mcv_platform.h"
#include "GroundedActionState.h"

GroundedActionState::GroundedActionState(CHandle adf)
	: IActionState::IActionState(adf) {
	accelerationVector = GetPlayerModel()->GetAccelerationVector();
	velocityVector = GetPlayerModel()->GetVelocityVector();
}

void GroundedActionState::update(float delta) {

}

void GroundedActionState::OnStateEnter(IActionState * lastState) {
	IActionState::OnStateEnter(lastState);
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	if (currentPowerStats) GetPlayerModel()->maxVerticalSpeed = currentPowerStats->maxVelocityVertical;
	GetPlayerModel()->ResetGravity();
	backwardsMaxDotProduct = cos(backwardsdMinAngle);
	GetPlayerModel()->lastWallEntered = nullptr;//En realidad al tocar el suelo ya se sobreescribe la variable
}

void GroundedActionState::OnStateExit(IActionState * nextState) {
	IActionState::OnStateExit(nextState);
	GetPlayerModel()->lastWallEntered = nullptr;
	GetPlayerModel()->lastWallNormal = PxVec3(0,0,0);
}

void GroundedActionState::OnJumpHighButton() {
	GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquat);
}

void GroundedActionState::OnJumpLongButton() {
	GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void GroundedActionState::OnStrongAttackButton() {
	GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::StrongAttack);
}

void GroundedActionState::OnFastAttackButton() {
	if (GetPlayerModel()->isConcurrentActionFree()) {
		GetPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::FastAttack);
	}
}

void GroundedActionState::OnReleasePowerButton() {
	GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::ReleasePowerGround);
}

void GroundedActionState::OnLeavingGround() {
	//Set state a alguno por defecto, luego las clases derivadas de esta ya sabrán qué hacer
	GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}
