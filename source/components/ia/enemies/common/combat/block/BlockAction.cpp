#include "mcv_platform.h"
#include "BlockAction.h"
#include "components/ia/enemies/Enemy.h"

BlockAction::BlockAction(Enemy* enemy) :
	enemy(enemy) {
}

int BlockAction::execAction(float delta) {
	if (enemy->timer.elapsed() < enemy->blockTime) {
		return Stay;
	}
	enemy->isBlocking = false;
	return Leave;
}
