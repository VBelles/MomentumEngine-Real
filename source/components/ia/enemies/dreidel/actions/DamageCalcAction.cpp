#include "mcv_platform.h"
#include "DamageCalcAction.h"


DamageCalcAction::DamageCalcAction(Enemy* enemy): enemy(enemy) {
}

int DamageCalcAction::execAction(float delta) {
	enemy->health -= enemy->receivedAttack.damage;
	return Leave;
}
