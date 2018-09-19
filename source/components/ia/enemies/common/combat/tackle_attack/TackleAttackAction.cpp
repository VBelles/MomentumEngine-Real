#include "mcv_platform.h"
#include "TackleAttackAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"
#include "components/comp_transform.h"

REGISTER_BTACTION("TackleAttackAction", TackleAttackAction);

TackleAttackAction::TackleAttackAction() {
	type = "TackleAttackAction";
}

TackleAttackAction::TackleAttackAction(Enemy* enemy, std::string animation, std::string attack, float speed, float time) :
	TackleAttackAction() {
	this->enemy = enemy;
	this->animation = animation;
	this->attack = attack;
	this->speed = speed;
	this->time = time;
}

int TackleAttackAction::execAction(float delta) {
	enemy->updateGravity(delta);
	EnemyAttack enemyAttack = enemy->attacks[attack];
	VEC3 movementIncrement = enemy->getTransform()->getFront() * speed * delta;
	VEC3 enemyPos = enemy->getTransform()->getPosition();
	if (enemy->timer.elapsed() >= time
		|| !(enemy->navMeshQuery && enemy->navMeshQuery->existsConnection(enemyPos, enemyPos + movementIncrement)
			&& !enemy->navMeshQuery->raycast2D(enemyPos, enemyPos + movementIncrement))) {
		enemy->getHitboxes()->disable(enemyAttack.hitboxName);
		enemy->currentAttack = "";
		enemy->attackTimer.reset();
		return Leave;
	}
	else {
		enemy->deltaMovement += movementIncrement;
		if (enemy->animationTimer.elapsed() >= frames2sec(enemyAttack.hitboxEnd)) {
			enemy->getHitboxes()->disable(enemyAttack.hitboxName);

		}
		else if (enemy->animationTimer.elapsed() >= frames2sec(enemyAttack.hitboxStart)) {
			enemy->getHitboxes()->enable(enemyAttack.hitboxName);
		}
		return Stay;
	}
}

void TackleAttackAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	attack = j.value("attack", attack);
	speed = j.value("speed", speed);
	time = j.value("time", time);
}

void TackleAttackAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::Text("Attack: %s\n", attack.c_str());
	ImGui::DragFloat("Speed", &speed, 0.1f, 0.0f, 500.0f);
	ImGui::DragFloat("Time", &time, 0.1f, 0.0f, 500.0f);
}