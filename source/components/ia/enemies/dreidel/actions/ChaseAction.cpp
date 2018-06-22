#include "mcv_platform.h"
#include "ChaseAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

ChaseAction::ChaseAction(Dreidel* dreidle): dreidel(dreidel) {
}

int ChaseAction::execAction(float delta) {
	return 0;
}
