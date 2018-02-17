#include "mcv_platform.h"
#include "AirborneActionState.h"

AirborneActionState::AirborneActionState(TCompPlayerModel * player) 
	: IActionState::IActionState(player) {
}

void AirborneActionState::update (float delta) {
}

void AirborneActionState::OnStateEnter(IActionState * lastState) {
	IActionState::OnStateEnter(lastState);
}

void AirborneActionState::OnStateExit(IActionState * nextState) {
	IActionState::OnStateExit(nextState);
}
