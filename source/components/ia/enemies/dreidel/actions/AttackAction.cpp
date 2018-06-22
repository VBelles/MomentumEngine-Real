#include "mcv_platform.h"
#include "AttackAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

AttackAction::AttackAction(Dreidel* dreidle): dreidel(dreidel) {
}

int AttackAction::execAction(float delta) {
	return 0;
}
