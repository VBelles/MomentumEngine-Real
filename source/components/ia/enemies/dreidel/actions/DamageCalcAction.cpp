#include "mcv_platform.h"
#include "DamageCalcAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

DamageCalcAction::DamageCalcAction(Dreidel* dreidel): dreidel(dreidel) {
}

int DamageCalcAction::execAction(float delta) {
	dreidel->health -= dreidel->receivedAttack.damage;
	return Leave;
}
