#include "mcv_platform.h"
#include "FreeActionState.h"

FreeActionState::FreeActionState(StateManager* stateManager)
	: IActionState(stateManager, Free) {
}
