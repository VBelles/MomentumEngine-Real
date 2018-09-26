#include "mcv_platform.h"
#include "HorizontalLaunchedAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"

REGISTER_BTACTION("HorizontalLaunchedAction", HorizontalLaunchedAction);

HorizontalLaunchedAction::HorizontalLaunchedAction() {
	type = "HorizontalLaunchedAction";
}

HorizontalLaunchedAction::HorizontalLaunchedAction(Enemy* enemy, std::string attack) :
	HorizontalLaunchedAction() {
	this->enemy = enemy;
	this->attack = attack;
}

int HorizontalLaunchedAction::execAction(float delta) {
	enemy->updateGravity(delta);
	enemy->deltaMovement += enemy->velocity * delta;
	if (enemy->grounded || enemy->getTransform()->getPosition().y + enemy->deltaMovement.y - enemy->initialLaunchPos.y <= 0.f) {
		enemy->velocity.x = 0;
		enemy->velocity.z = 0;
		enemy->timer.reset();
		enemy->getHitboxes()->disable(enemy->attacks[attack].hitboxName);
		enemy->currentAttack = "";
		return Leave;
	}
	else {
		return Stay;
	}
}

void HorizontalLaunchedAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
	attack = j.value("attack", attack);
	soundLauncherLoop = j.value("sound_launcher_loop", soundLauncherLoop);
}

void HorizontalLaunchedAction::debugInMenu() {
	ImGui::Text("Attack: %s\n", attack.c_str());
}
