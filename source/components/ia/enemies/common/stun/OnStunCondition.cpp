#include "mcv_platform.h"
#include "OnStunCondition.h"

OnStunCondition::OnStunCondition(Enemy* enemy) : enemy(enemy) {
}

bool OnStunCondition::testCondition(float delta) {
	return enemy->stunTimer.elapsed() < enemy->stunDuration;
}
