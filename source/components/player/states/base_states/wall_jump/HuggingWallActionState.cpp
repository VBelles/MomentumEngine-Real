#include "mcv_platform.h"
#include "HuggingWallActionState.h"

HuggingWallActionState::HuggingWallActionState(CHandle playerModelHandle)
	:AirborneActionState::AirborneActionState(playerModelHandle) {
}

void HuggingWallActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
}

void HuggingWallActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	//TODO tirar rayo para atrás y coger la normal real de la pared, rotarse correctamente
	//y guardar esa dirección para poder desengancharse poniendo el stick en dirección opuesta
	float y, p, r;
	GetPlayerModel()->SetGravityMultiplier(8.f);
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