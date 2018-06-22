#include "mcv_platform.h"
#include "DeadAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

DeadAction::DeadAction(Dreidel* dreidel): dreidel(dreidel) {
}

int DeadAction::execAction(float delta) {
	return Leave;
}
