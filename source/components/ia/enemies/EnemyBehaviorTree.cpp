#include "mcv_platform.h"
#include "EnemyBehaviorTree.h"

#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"
#include "components/comp_give_power.h"
#include "components/player/comp_player_model.h"
#include "modules/game_modules/game/module_game.h"
#include "modules/game_modules/game/enemy_manager.h"

DECL_OBJ_MANAGER("enemy_bt", EnemyBehaviorTree);

EnemyBehaviorTree::~EnemyBehaviorTree() {
	SAFE_DELETE(enemy);
}

void EnemyBehaviorTree::load(const json& j, TEntityParseContext& ctx) {
	SAFE_DELETE(enemy);
	enemy = new Enemy();
	enemy->load(j);
}

void EnemyBehaviorTree::debugInMenu() {
	enemy->debugInMenu();
}

void EnemyBehaviorTree::registerMsgs() {
	DECL_MSG(EnemyBehaviorTree, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(EnemyBehaviorTree, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(EnemyBehaviorTree, TMsgAttackHit, onAttackHit);
	DECL_MSG(EnemyBehaviorTree, TMsgOutOfBounds, onOutOfBounds);
	DECL_MSG(EnemyBehaviorTree, TMsgPerfectDodged, onPerfectDodged);
	DECL_MSG(EnemyBehaviorTree, TMsgColliderDestroyed, onColliderDestroyed);
	DECL_MSG(EnemyBehaviorTree, TMsgHitboxEnter, onHitboxEnter);
}

void EnemyBehaviorTree::update(float delta) {
	if (!enemy->getCollider()->toDestroy) {
		enemy->deltaMovement = VEC3::Zero;
		enemy->recalc(delta);
		if (enemy->getCollider()->isCreated()) enemy->move(delta);
	}
}

void EnemyBehaviorTree::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	enemy->playerHandle = getEntityByName(PLAYER_NAME);
	if (enemy->playerHandle.isValid()) {
		CEntity* player = enemy->playerHandle;
		enemy->playerTransformHandle = player->get<TCompTransform>();
		enemy->playerModelHandle = player->get<TCompPlayerModel>();
	}
	enemy->transformHandle = get<TCompTransform>();
	enemy->colliderHandle = get<TCompCollider>();
	enemy->skeletonHandle = get<TCompSkeleton>();
	enemy->hitboxesHandle = get<TCompHitboxes>();
	enemy->powerHandle = get<TCompGivePower>();

	enemy->spawnPosition = enemy->getTransform()->getPosition();

	enemy->onSpawn = true;
	enemy->resetCurrent();
}

void EnemyBehaviorTree::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	enemy->playerHandle = getEntityByName(PLAYER_NAME);
	if (enemy->playerHandle.isValid()) {
		CEntity* player = enemy->playerHandle;
		enemy->playerTransformHandle = player->get<TCompTransform>();
		enemy->playerModelHandle = player->get<TCompPlayerModel>();
	}
	enemy->spawnPosition = enemy->getTransform()->getPosition();
}

void EnemyBehaviorTree::onAttackHit(const TMsgAttackHit& msg) {
	if (!enemy->getCollider()->toDestroy) {
		enemy->receivedAttack = msg.info;
		enemy->onHit = true;
		enemy->resetCurrent();
	}
}

void EnemyBehaviorTree::onOutOfBounds(const TMsgOutOfBounds& msg) {
	if (!enemy->getCollider()->toDestroy) {
		enemy->onOutOfBounds = true;
		enemy->resetCurrent();
	}
}

void EnemyBehaviorTree::onPerfectDodged(const TMsgPerfectDodged & msg) {
	if (!enemy->getCollider()->toDestroy) {
		dbg("Damn! I've been dodged.\n");
		if (hpGiven < maxHpToGive) {
			enemy->getPlayerModel()->setHp(enemy->getPlayerModel()->getHp() + 1);
			hpGiven++;
		}
	}
}

void EnemyBehaviorTree::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	if (enemy->getCollider()->toDestroy) {
		CEntity* entity = CHandle(this).getOwner();
		((CModuleGame*)(EngineModules.getModule("game")))->getEnemyManager()->onDead(entity->getName());
		enemy->getSkeleton()->stop();
		CHandle(this).getOwner().destroy();
	}
}

void EnemyBehaviorTree::onHitboxEnter(const TMsgHitboxEnter& msg) {
	if (enemy->currentAttack != "") {
		if (enemy->attacks[enemy->currentAttack].hitboxName == msg.hitbox) {
			TMsgAttackHit attackHit = {};
			attackHit.attacker = CHandle(this).getOwner();
			attackHit.info = enemy->attacks[enemy->currentAttack].attackInfo;
			((CEntity*)msg.h_other_entity)->sendMsg(attackHit);
		}
	}
}