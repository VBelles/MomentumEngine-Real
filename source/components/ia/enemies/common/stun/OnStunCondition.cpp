#include "mcv_platform.h"
#include "OnStunCondition.h"
#include "components/ia/enemies/Enemy.h"

OnStunCondition::OnStunCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnStunCondition::testCondition(float delta) {
	return enemy->stunTimer.elapsed() < enemy->stunDuration;
}
