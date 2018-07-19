#include "mcv_platform.h"
#include "BlockAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_give_power.h"

REGISTER_BTACTION("BlockAction", BlockAction);

BlockAction::BlockAction(Enemy* enemy) :
	enemy(enemy) {
}

int BlockAction::execAction(float delta) {
	if (enemy->blockTimer.elapsed() < enemy->blockTime) {
		return Stay;
	}
	enemy->isBlocking = false;
	enemy->getPower()->setStateMultiplier(1.f);
	return Leave;
}

void BlockAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}