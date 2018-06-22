#include "mcv_platform.h"
#include "IdleAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

IdleAction::IdleAction(Dreidel* dreidle): dreidel(dreidel) {
}

int IdleAction::execAction(float delta) {
	return 0;
}
