#include "mcv_platform.h"
#include "DeadAction.h"


DeadAction::DeadAction(Enemy* enemy): enemy(enemy) {
}

int DeadAction::execAction(float delta) {
	return Leave;
}
