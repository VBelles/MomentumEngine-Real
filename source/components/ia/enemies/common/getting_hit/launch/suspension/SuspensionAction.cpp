#include "mcv_platform.h"
#include "SuspensionAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTACTION("SuspensionAction", SuspensionAction);

SuspensionAction::SuspensionAction() {
	type = "SuspensionAction";
}

SuspensionAction::SuspensionAction(Enemy* enemy) :
	SuspensionAction() {
	this->enemy = enemy;
}

int SuspensionAction::execAction(float delta) {
	if (!soundEmited) {
		eventInstance = EngineSound.emitEvent(soundLoop, enemy->getTransform());
		soundEmited = true;
	}
	if (enemy->timer.elapsed() < enemy->suspensionDuration) {
		return Stay;
	}
	EngineSound.stopEvent(eventInstance);
	EngineSound.emitEvent(soundEnd, enemy->getTransform());
	return Leave;
}

void SuspensionAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
	soundLoop = j.value("sound_loop", soundLoop);
	soundEnd = j.value("sound_end", soundEnd);
}