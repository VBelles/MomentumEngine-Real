#include "mcv_platform.h"
#include "comp_enemy.h"

#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"
#include "components/comp_give_power.h"
#include "components/player/comp_player_model.h"
#include "modules/game_modules/game/module_game.h"
#include "modules/game_modules/game/enemy_manager.h"

DECL_OBJ_MANAGER("enemy", TCompEnemy);

TCompEnemy::~TCompEnemy() {
	safeDelete(enemy);
}

void TCompEnemy::load(const json& j, TEntityParseContext& ctx) {
	safeDelete(enemy);
	enemy = new Enemy();
	enemy->load(j);
}

void TCompEnemy::debugInMenu() {
	enemy->debugInMenu();
}

void TCompEnemy::registerMsgs() {
	DECL_MSG(TCompEnemy, TMsgEntityCreated, onEntityCreated);
	DECL_MSG(TCompEnemy, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompEnemy, TMsgAttackHit, onAttackHit);
	DECL_MSG(TCompEnemy, TMsgOutOfBounds, onOutOfBounds);
	DECL_MSG(TCompEnemy, TMsgPerfectDodged, onPerfectDodged);
	DECL_MSG(TCompEnemy, TMsgColliderDestroyed, onColliderDestroyed);
	DECL_MSG(TCompEnemy, TMsgHitboxEnter, onHitboxEnter);
}

void TCompEnemy::update(float delta) {
	if (!enemy->getCollider()->toDestroy) {
		enemy->deltaMovement = VEC3::Zero;
		enemy->recalc(delta);
		if (enemy->getCollider()->isCreated()) enemy->move(delta);
	}
}

void TCompEnemy::onEntityCreated(const TMsgEntityCreated& msg) {
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
	enemy->soundHandle = get<TCompSound>();

	enemy->spawnPosition = enemy->getTransform()->getPosition();

	enemy->onSpawn = true;
	enemy->resetCurrent();
}

void TCompEnemy::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	enemy->playerHandle = getEntityByName(PLAYER_NAME);
	if (enemy->playerHandle.isValid()) {
		CEntity* player = enemy->playerHandle;
		enemy->playerTransformHandle = player->get<TCompTransform>();
		enemy->playerModelHandle = player->get<TCompPlayerModel>();
	}
	enemy->spawnPosition = enemy->getTransform()->getPosition();
}

void TCompEnemy::onAttackHit(const TMsgAttackHit& msg) {
	if (!enemy->getCollider()->toDestroy) {
		enemy->receivedAttack = msg.info;
		enemy->onHit = true;
		enemy->resetCurrent();
	}
}

void TCompEnemy::onOutOfBounds(const TMsgOutOfBounds& msg) {
	if (!enemy->getCollider()->toDestroy) {
		enemy->onOutOfBounds = true;
		enemy->resetCurrent();
	}
}

void TCompEnemy::onPerfectDodged(const TMsgPerfectDodged & msg) {
	if (!enemy->getCollider()->toDestroy) {
		dbg("Damn! I've been dodged.\n");
		if (enemy->hpGiven < enemy->maxHpToGive) {
			enemy->getPlayerModel()->setHp(enemy->getPlayerModel()->getHp() + 1);
			enemy->hpGiven++;
		}
	}
}

void TCompEnemy::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	if (enemy->getCollider()->toDestroy) {
		CEntity* entity = CHandle(this).getOwner();
		((CModuleGame*)(EngineModules.getModule("game")))->getEnemyManager()->onDead(entity->getName());
		enemy->getSkeleton()->stop();
		CHandle(this).getOwner().destroy();
	}
}

void TCompEnemy::onHitboxEnter(const TMsgHitboxEnter& msg) {
	if (enemy->currentAttack != "") {
		if (msg.h_other_entity != CHandle(this).getOwner()) {
			if (enemy->attacks[enemy->currentAttack].hitboxName == msg.hitbox) {
				TMsgAttackHit attackHit = {};
				attackHit.attacker = CHandle(this).getOwner();
				attackHit.info = enemy->attacks[enemy->currentAttack].attackInfo;
				//check if attack info is propel, if it is override velocity (radial)
				if (attackHit.info.propel) {
					CEntity* otherEntity = msg.h_other_entity;
					TCompTransform* otherTransform = otherEntity->get<TCompTransform>();
					VEC3 launchVelocity = otherTransform->getPosition() - enemy->getTransform()->getPosition();
					launchVelocity.Normalize();
					launchVelocity *= enemy->velocity.Length() * 0.3f;
					if (launchVelocity.Length() < 3.0f) {
						safeDelete(attackHit.info.propel);
					}
					else {
						attackHit.info.propel->velocity = launchVelocity;
					}
				}
				((CEntity*)msg.h_other_entity)->sendMsg(attackHit);
			}
		}
	}
}
