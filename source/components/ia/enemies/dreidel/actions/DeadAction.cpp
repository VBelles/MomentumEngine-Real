#include "mcv_platform.h"
#include "DeadAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

DeadAction::DeadAction(Dreidel* dreidle): dreidel(dreidel) {
}

int DeadAction::execAction(float delta) {
	return 0;
}
