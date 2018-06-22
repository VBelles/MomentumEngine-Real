#include "mcv_platform.h"
#include "DamageCalcAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

DamageCalcAction::DamageCalcAction(Dreidel* dreidle): dreidel(dreidel) {
}

int DamageCalcAction::execAction(float delta) {
	return 0;
}
