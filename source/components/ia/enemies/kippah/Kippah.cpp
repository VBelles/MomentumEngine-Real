#include "mcv_platform.h"
#include "Kippah.h"

#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"
#include "components/player/comp_player_model.h"
#include "components/comp_give_power.h"
#include "modules/game_modules/game/module_game.h"
#include "modules/game_modules/game/enemy_manager.h"

#include "components/ia/enemies/common/FalseCondition.h"
#include "components/ia/enemies/common/death/DeathAction.h"
#include "components/ia/enemies/common/death/DisappearAction.h"
#include "components/ia/enemies/common/death/OnDeathAction.h"
#include "components/ia/enemies/common/death/OnDeathCondition.h"
#include "components/ia/enemies/common/death/OnDisappearAction.h"
#include "components/ia/enemies/common/getting_hit/grab/GrabAction.h"
#include "components/ia/enemies/common/getting_hit/grab/OnGrabAction.h"
#include "components/ia/enemies/common/getting_hit/grab/OnGrabCondition.h"
#include "components/ia/enemies/common/getting_hit/hit/HitStun.h"
#include "components/ia/enemies/common/getting_hit/hit/OnHit.h"
#include "components/ia/enemies/common/getting_hit/launch/horizontal/HorizontalLaunchedAction.h"
#include "components/ia/enemies/common/getting_hit/launch/horizontal/OnHorizontalLaunchAction.h"
#include "components/ia/enemies/common/getting_hit/launch/horizontal/OnHorizontalLaunchCondition.h"
#include "components/ia/enemies/common/getting_hit/launch/vertical/OnVerticalLaunchAction.h"
#include "components/ia/enemies/common/getting_hit/launch/vertical/OnVerticalLaunchCondition.h"
#include "components/ia/enemies/common/getting_hit/launch/vertical/VerticalLaunchedAction.h"
#include "components/ia/enemies/common/getting_hit/propel/OnPropelAction.h"
#include "components/ia/enemies/common/getting_hit/propel/OnPropelCondition.h"
#include "components/ia/enemies/common/getting_hit/propel/PropelAction.h"
#include "components/ia/enemies/common/stun/OnStunAction.h"
#include "components/ia/enemies/common/stun/OnStunCondition.h"
#include "components/ia/enemies/common/stun/StunAction.h"
#include "components/ia/enemies/common/idle/OnIdleAction.h"
#include "components/ia/enemies/common/idle/IdleLoopFlying.h"
#include "components/ia/enemies/common/idle/OnIdleLoop.h"
#include "components/ia/enemies/common/idle/IdleActionFlying.h"
#include "components/ia/enemies/common/teleport/OnTeleportAction.h"
#include "components/ia/enemies/common/teleport/OnTeleportCondition.h"
#include "components/ia/enemies/common/teleport/TeleportAction.h"
#include "components/ia/enemies/common/return_to_spawn/OnReturnToSpawnAction.h"
#include "components/ia/enemies/common/return_to_spawn/OnReturnToSpawnCondition.h"
#include "components/ia/enemies/common/return_to_spawn/ReturnToSpawnFlyingAction.h"
#include "components/ia/enemies/common/appear/AppearAction.h"
#include "components/ia/enemies/common/appear/OnAppearAction.h"
#include "components/ia/enemies/common/combat/CombatCondition.h"
#include "components/ia/enemies/common/combat/attack/AttackActionFlying.h"
#include "components/ia/enemies/common/combat/attack/OnAttackAction.h"
#include "components/ia/enemies/common/combat/idle_war/IdleWarActionFlying.h"
#include "components/ia/enemies/common/combat/idle_war/OnIdleWarAction.h"
#include "components/ia/enemies/common/combat/long_distance/LongDistanceCombatCondition.h"
#include "components/ia/enemies/common/combat/medium_distance/MediumDistanceCombatCondition.h"
#include "components/ia/enemies/common/combat/short_distance/ShortDistanceCombatCondition.h"
#include "components/ia/enemies/common/combat/ranged_attack/RangedAttackAction.h"
#include "components/ia/enemies/common/combat/AttackCoolDownCondition.h"

DECL_OBJ_MANAGER("kippah", Kippah);

Kippah::~Kippah() {
	SAFE_DELETE(enemy);
}

void Kippah::initBehaviorTree() {
	enemy->clear();

	FalseCondition* falseCondition = new FalseCondition();
	LongDistanceCombatCondition* longDistanceCombatCondition = new LongDistanceCombatCondition(enemy);
	MediumDistanceCombatCondition* mediumDistanceCombatCondition = new MediumDistanceCombatCondition(enemy);
	ShortDistanceCombatCondition* shortDistanceCombatCondition = new ShortDistanceCombatCondition(enemy);
	DeathAction* deathAction = new DeathAction(enemy, "medusa_death");
	DisappearAction* disappearAction = new DisappearAction(enemy, "medusa_desaparicion");
	OnDeathAction* onDeathAction = new OnDeathAction(enemy, "medusa_death");
	OnDeathCondition* onDeathCondition = new OnDeathCondition(enemy);
	OnDisappearAction* onDisappearAction = new OnDisappearAction(enemy, "medusa_desaparicion");
	GrabAction* grabAction = new GrabAction(enemy);
	OnGrabAction* onGrabAction = new OnGrabAction(enemy);
	OnGrabCondition* onGrabCondition = new OnGrabCondition(enemy);
	HitStun* hitStun = new HitStun(enemy, "medusa_damage");
	OnHit* onHit = new OnHit(enemy, "medusa_damage");
	HorizontalLaunchedAction* horizontalLaunchedAction = new HorizontalLaunchedAction(enemy);
	OnHorizontalLaunchAction* onHorizontalLaunchAction = new OnHorizontalLaunchAction(enemy, "medusa_damage");
	OnHorizontalLaunchCondition* onHorizontalLaunchCondition = new OnHorizontalLaunchCondition(enemy);
	OnVerticalLaunchAction* onVerticalLaunchAction = new OnVerticalLaunchAction(enemy, "medusa_damage");
	OnVerticalLaunchCondition* onVerticalLaunchCondition = new OnVerticalLaunchCondition(enemy);
	VerticalLaunchedAction* verticalLaunchedAction = new VerticalLaunchedAction(enemy);
	OnPropelAction* onPropelAction = new OnPropelAction(enemy);
	OnPropelCondition * onPropelCondition = new OnPropelCondition(enemy);
	PropelAction* propelAction = new PropelAction(enemy);
	OnStunAction* onStunAction = new OnStunAction(enemy, "", "medusa_stun");
	OnStunCondition* onStunCondition = new OnStunCondition(enemy);
	StunAction* stunAction = new StunAction(enemy);
	OnIdleAction* onIdleAction = new OnIdleAction(enemy, "medusa_idle");
	IdleActionFlying* idleActionFlying = new IdleActionFlying(enemy, "medusa_idle", "shortDistanceCombat");
	OnIdleLoop* onIdleLoop = new OnIdleLoop(enemy, "medusa_idle");
	IdleLoopFlying* idleLoopFlying = new IdleLoopFlying(enemy, "medusa_idle", "shortDistanceCombat");
	OnTeleportAction* onTeleportAction = new OnTeleportAction(enemy, "medusa_desaparicion");
	OnTeleportCondition* onTeleportCondition = new OnTeleportCondition(enemy);
	TeleportAction* teleportAction = new TeleportAction(enemy, "medusa_desaparicion");
	AppearAction* appearAction = new AppearAction(enemy, "medusa_aparicion");
	OnAppearAction* onAppearAction = new OnAppearAction(enemy, "medusa_aparicion");
	CombatCondition* combatCondition = new CombatCondition(enemy);
	AttackActionFlying* spinAttackAction = new AttackActionFlying(enemy, "medusa_revolverse", "spinAttack");
	OnAttackAction* onSpinAttackAction = new OnAttackAction(enemy, "medusa_revolverse", "spinAttack");
	IdleWarActionFlying* idleWarActionFlying = new IdleWarActionFlying(enemy, "medusa_idle_war");
	OnIdleWarAction* onIdleWarAction = new OnIdleWarAction(enemy, "medusa_idle_war");
	OnReturnToSpawnAction* onReturnToSpawnAction = new OnReturnToSpawnAction(enemy, "medusa_idle");
	OnReturnToSpawnCondition* onReturnToSpawnCondition = new OnReturnToSpawnCondition(enemy);
	ReturnToSpawnFlyingAction* returnToSpawnFlyingAction = new ReturnToSpawnFlyingAction(enemy, "shortDistanceCombat");
	OnAttackAction* onRangedAttackAction = new OnAttackAction(enemy, "medusa_shot", "rangedAttack");
	RangedAttackAction* rangedAttackAction = new RangedAttackAction(enemy, "medusa_shot", "rangedAttack", "shortDistanceCombat");
	AttackCoolDownCondition* attackCoolDownCondition = new AttackCoolDownCondition(enemy);

	//root
	enemy->createRoot("kippah", Priority, nullptr, nullptr);

	//getting hit
	enemy->addChild("kippah", "hit", Sequence, falseCondition, nullptr);
	//damage calculation
	enemy->addChild("hit", "onHit", Action, nullptr, onHit);
	//attack properties
	enemy->addChild("hit", "attackProperties", Priority, nullptr, nullptr);
	//horizontal launch
	enemy->addChild("attackProperties", "horizontalLaunch", Sequence, onHorizontalLaunchCondition, nullptr);
	enemy->addChild("horizontalLaunch", "onHorizontalLaunchAction", Action, nullptr, onHorizontalLaunchAction);
	enemy->addChild("horizontalLaunch", "horizontalLaunchedAction", Action, nullptr, horizontalLaunchedAction);
	//vertical launch
	enemy->addChild("attackProperties", "verticalLaunch", Sequence, onVerticalLaunchCondition, nullptr);
	enemy->addChild("verticalLaunch", "onVerticalLaunchAction", Action, nullptr, onVerticalLaunchAction);
	enemy->addChild("verticalLaunch", "verticalLaunchedAction", Action, nullptr, verticalLaunchedAction);
	//grab
	enemy->addChild("attackProperties", "grab", Sequence, onGrabCondition, nullptr);
	enemy->addChild("grab", "onGrabAction", Action, nullptr, onGrabAction);
	enemy->addChild("grab", "grabAction", Action, nullptr, grabAction);
	//propel
	enemy->addChild("attackProperties", "propel", Sequence, onPropelCondition, nullptr);
	enemy->addChild("propel", "onPropelAction", Action, nullptr, onPropelAction);
	enemy->addChild("propel", "propelAction", Action, nullptr, propelAction);
	//hit stun
	enemy->addChild("attackProperties", "hitStun", Action, nullptr, hitStun);

	//death
	enemy->addChild("kippah", "death", Sequence, onDeathCondition, nullptr);
	enemy->addChild("death", "onDeathAction", Action, nullptr, onDeathAction);
	enemy->addChild("death", "deathAction", Action, nullptr, deathAction);
	//disappear
	enemy->addChild("death", "disappear", Sequence, nullptr, nullptr);
	enemy->addChild("disappear", "onDisappearAction", Action, nullptr, onDisappearAction);
	enemy->addChild("disappear", "disappearAction", Action, nullptr, disappearAction);

	//stun
	enemy->addChild("kippah", "stun", Sequence, onStunCondition, nullptr);
	enemy->addChild("stun", "onStunAction", Action, nullptr, onStunAction);
	enemy->addChild("stun", "stunAction", Action, nullptr, stunAction);

	//teleport to spawn
	enemy->addChild("kippah", "teleport", Sequence, onTeleportCondition, nullptr);
	enemy->addChild("teleport", "onTeleportAction", Action, nullptr, onTeleportAction);
	enemy->addChild("teleport", "teleportAction", Action, nullptr, teleportAction);
	//appear
	enemy->addChild("teleport", "appear", Sequence, nullptr, nullptr);
	enemy->addChild("appear", "onAppearAction", Action, nullptr, onAppearAction);
	enemy->addChild("appear", "appearAction", Action, nullptr, appearAction);

	//return to spawn walking
	enemy->addChild("kippah", "returnToSpawn", Sequence, onReturnToSpawnCondition, nullptr);
	enemy->addChild("returnToSpawn", "onReturnToSpawnAction", Action, nullptr, onReturnToSpawnAction);
	enemy->addChild("returnToSpawn", "returnToSpawnAction", Action, nullptr, returnToSpawnFlyingAction);

	//combat
	enemy->addChild("kippah", "combat", Priority, combatCondition, nullptr);
	//short distance combat
	enemy->addChild("combat", "shortDistanceCombat", Sequence, shortDistanceCombatCondition, nullptr);
	//spin attack
	enemy->addChild("shortDistanceCombat", "shortSpinAttack", Sequence, nullptr, nullptr);
	enemy->addChild("shortSpinAttack", "onShortSpinAttackAction", Action, nullptr, onSpinAttackAction);
	enemy->addChild("shortSpinAttack", "shortSpinAttackAction", Action, nullptr, spinAttackAction);
	//idle war
	enemy->addChild("shortDistanceCombat", "shortIdleWar", Sequence, nullptr, nullptr);
	enemy->addChild("shortIdleWar", "onShortIdleWar", Action, nullptr, onIdleWarAction);
	enemy->addChild("shortIdleWar", "shortIdleWarAction", Action, nullptr, idleWarActionFlying);
	//attack cool down
	enemy->addChild("combat", "attackCooldownCombat", Random, attackCoolDownCondition, nullptr);
	//idle war
	enemy->addChild("attackCooldownCombat", "CDIdleWar", Sequence, nullptr, nullptr);
	enemy->addChild("CDIdleWar", "onCDIdleWar", Action, nullptr, onIdleWarAction);
	enemy->addChild("CDIdleWar", "CDIdleWarAction", Action, nullptr, idleWarActionFlying);
	//long distance combat
	enemy->addChild("combat", "longDistanceCombat", Random, longDistanceCombatCondition, nullptr);
	//ranged attack
	enemy->addChild("longDistanceCombat", "rangedAttack", Sequence, nullptr, nullptr);
	enemy->addChild("rangedAttack", "onRangedAttackAction", Action, nullptr, onRangedAttackAction);
	enemy->addChild("rangedAttack", "rangedAttackAction", Action, nullptr, rangedAttackAction);
	//idle war
	enemy->addChild("longDistanceCombat", "longIdleWar", Sequence, nullptr, nullptr);
	enemy->addChild("longIdleWar", "onLongIdleWar", Action, nullptr, onIdleWarAction);
	enemy->addChild("longIdleWar", "longIdleWarAction", Action, nullptr, idleWarActionFlying);

	//idle
	enemy->addChild("kippah", "idle", Random, nullptr, nullptr);
	//idle action
	enemy->addChild("idle", "idleActionSeq", Sequence, nullptr, nullptr);
	enemy->addChild("idleActionSeq", "onIdleAction", Action, nullptr, onIdleAction);
	enemy->addChild("idleActionSeq", "idleAction", Action, nullptr, idleActionFlying);
	//idle loop
	enemy->addChild("idle", "idleLoopSeq", Sequence, nullptr, nullptr);
	enemy->addChild("idleLoopSeq", "onIdleLoop", Action, nullptr, onIdleLoop);
	enemy->addChild("idleLoopSeq", "idleLoop", Action, nullptr, idleLoopFlying);
}

void Kippah::load(const json& j, TEntityParseContext& ctx) {
	SAFE_DELETE(enemy);
	enemy = new Enemy();
	enemy->load(j, ctx);
}

void Kippah::debugInMenu() {
	enemy->debugInMenu();
}

void Kippah::registerMsgs() {
	DECL_MSG(Kippah, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(Kippah, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(Kippah, TMsgAttackHit, onAttackHit);
	DECL_MSG(Kippah, TMsgOutOfBounds, onOutOfBounds);
	DECL_MSG(Kippah, TMsgPerfectDodged, onPerfectDodged);
	DECL_MSG(Kippah, TMsgColliderDestroyed, onColliderDestroyed);
	DECL_MSG(Kippah, TMsgHitboxEnter, onHitboxEnter);
}

void Kippah::update(float delta) {
	if (!enemy->getCollider()->toDestroy) {
		enemy->deltaMovement = VEC3::Zero;
		enemy->recalc(delta);
		if (enemy->getCollider()->isCreated()) enemy->move(delta);
	}
}

void Kippah::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	initBehaviorTree();

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

	enemy->setCurrent("appear");
}

void Kippah::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	enemy->playerHandle = getEntityByName(PLAYER_NAME);
	if (enemy->playerHandle.isValid()) {
		CEntity* player = enemy->playerHandle;
		enemy->playerTransformHandle = player->get<TCompTransform>();
		enemy->playerModelHandle = player->get<TCompPlayerModel>();
	}
	enemy->spawnPosition = enemy->getTransform()->getPosition();
}

void Kippah::onAttackHit(const TMsgAttackHit& msg) {
	if (!enemy->getCollider()->toDestroy) {
		enemy->receivedAttack = msg.info;
		enemy->setCurrent("hit");
	}
}

void Kippah::onOutOfBounds(const TMsgOutOfBounds& msg) {
	if (!enemy->getCollider()->toDestroy) {
		enemy->setCurrent("disappear");
	}
}

void Kippah::onPerfectDodged(const TMsgPerfectDodged & msg) {
	if (!enemy->getCollider()->toDestroy) {
		dbg("Damn! I've been dodged.\n");
		if (hpGiven < maxHpToGive) {
			enemy->getPlayerModel()->setHp(enemy->getPlayerModel()->getHp() + 1);
			hpGiven++;
		}
	}
}

void Kippah::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	if (enemy->getCollider()->toDestroy) {
		CEntity* entity = CHandle(this).getOwner();
		((CModuleGame*)(EngineModules.getModule("game")))->getEnemyManager()->onDead(entity->getName());
		enemy->getSkeleton()->stop();
		CHandle(this).getOwner().destroy();
	}
}

void Kippah::onHitboxEnter(const TMsgHitboxEnter& msg) {
	if (enemy->currentAttack != "") {
		if (enemy->attacks[enemy->currentAttack].hitboxName == msg.hitbox) {
			TMsgAttackHit attackHit = {};
			attackHit.attacker = CHandle(this).getOwner();
			attackHit.info = enemy->attacks[enemy->currentAttack].attackInfo;
			((CEntity*)msg.h_other_entity)->sendMsg(attackHit);
		}
	}
}