#include "mcv_platform.h"
#include "HuggingWallActionState.h"

HuggingWallActionState::HuggingWallActionState(CHandle playerModelHandle)
	:AirborneActionState::AirborneActionState(playerModelHandle) {
}

void HuggingWallActionState::update (float delta) {

}

void HuggingWallActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	float y, p, r;
	GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	y = atan2(hit.worldNormal.x, hit.worldNormal.z);
	GetPlayerTransform()->setYawPitchRoll(y, p, r);
	SetPose();
}

void HuggingWallActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
}

void HuggingWallActionState::OnJumpHighButton() {
	
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::HuggingWallJumpSquat);
}

void HuggingWallActionState::OnJumpLongButton() {
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::JumpSquatLong);
}