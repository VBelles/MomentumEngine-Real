#include "mcv_platform.h"
#include "Dreidel.h"

#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"
#include "components/comp_give_power.h"
#include "components/player/comp_player_model.h"
#include "modules/game_modules/game/module_game.h"
#include "modules/game_modules/game/enemy_manager.h"

#include "components/ia/enemies/common/FalseCondition.h"
#include "components/ia/enemies/common/airborne/AirborneAction.h"
#include "components/ia/enemies/common/airborne/AirborneCondition.h"
#include "components/ia/enemies/common/airborne/OnAirborneAction.h"
#include "components/ia/enemies/common/airborne/OnAirborneCondition.h"
#include "components/ia/enemies/common/bloking_break/BlockingBreakAction.h"
#include "components/ia/enemies/common/bloking_break/OnBlockingBreakAction.h"
#include "components/ia/enemies/common/bloking_break/OnBlockingBreakCondition.h"
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
#include "components/ia/enemies/common/idle/IdleLoop.h"
#include "components/ia/enemies/common/idle/OnIdleLoop.h"
#include "components/ia/enemies/common/idle/IdleAction.h"
#include "components/ia/enemies/common/idle/OnStrollAction.h"
#include "components/ia/enemies/common/idle/StrollAction.h"
#include "components/ia/enemies/common/teleport/OnTeleportAction.h"
#include "components/ia/enemies/common/teleport/OnTeleportCondition.h"
#include "components/ia/enemies/common/teleport/TeleportAction.h"
#include "components/ia/enemies/common/return_to_spawn/OnReturnToSpawnAction.h"
#include "components/ia/enemies/common/return_to_spawn/OnReturnToSpawnCondition.h"
#include "components/ia/enemies/common/return_to_spawn/ReturnToSpawnAction.h"
#include "components/ia/enemies/common/appear/AppearAction.h"
#include "components/ia/enemies/common/appear/OnAppearAction.h"
#include "components/ia/enemies/common/combat/CombatCondition.h"
#include "components/ia/enemies/common/combat/attack/AttackAction.h"
#include "components/ia/enemies/common/combat/attack/OnAttackAction.h"
#include "components/ia/enemies/common/combat/chase/ChaseAction.h"
#include "components/ia/enemies/common/combat/chase/OnChaseAction.h"
#include "components/ia/enemies/common/combat/idle_war/IdleWarAction.h"
#include "components/ia/enemies/common/combat/idle_war/OnIdleWarAction.h"
#include "components/ia/enemies/common/combat/long_distance/LongDistanceCombatCondition.h"
#include "components/ia/enemies/common/combat/medium_distance/MediumDistanceCombatCondition.h"
#include "components/ia/enemies/common/combat/short_distance/ShortDistanceCombatCondition.h"
#include "components/ia/enemies/common/combat/tackle_attack/OnTackleAttackAction.h"
#include "components/ia/enemies/common/combat/tackle_attack/TackleAttackAction.h"
#include "components/ia/enemies/common/combat/step_back/OnStepBackAction.h"
#include "components/ia/enemies/common/combat/step_back/StepBackAction.h"
#include "components/ia/enemies/common/combat/block/BlockAction.h"
#include "components/ia/enemies/common/combat/block/OnBlockAction.h"
#include "components/ia/enemies/common/combat/block/BlockCondition.h"
#include "components/ia/enemies/common/combat/AttackCoolDownCondition.h"

DECL_OBJ_MANAGER("dreidel", Dreidel);

Dreidel::~Dreidel() {
	SAFE_DELETE(enemy);
}

void Dreidel::initBehaviorTree() {
	enemy->clear();

	FalseCondition* falseCondition = new FalseCondition();
	AirborneAction* airborneAction = new AirborneAction(enemy);
	LongDistanceCombatCondition* longDistanceCombatCondition = new LongDistanceCombatCondition(enemy);
	MediumDistanceCombatCondition* mediumDistanceCombatCondition = new MediumDistanceCombatCondition(enemy);
	ShortDistanceCombatCondition* shortDistanceCombatCondition = new ShortDistanceCombatCondition(enemy);
	AirborneCondition* airborneCondition = new AirborneCondition(enemy);
	OnAirborneAction* onAirborneAction = new OnAirborneAction(enemy, "enemigo_bola_idle");
	OnAirborneCondition* onAirborneCondition = new OnAirborneCondition(enemy);
	DeathAction* deathAction = new DeathAction(enemy, "enemigo_bola_muerte");
	DisappearAction* disappearAction = new DisappearAction(enemy, "enemigo_bola_desaparecer");
	OnDeathAction* onDeathAction = new OnDeathAction(enemy, "enemigo_bola_muerte");
	OnDeathCondition* onDeathCondition = new OnDeathCondition(enemy);
	OnDisappearAction* onDisappearAction = new OnDisappearAction(enemy, "enemigo_bola_desaparecer");
	GrabAction* grabAction = new GrabAction(enemy);
	OnGrabAction* onGrabAction = new OnGrabAction(enemy);
	OnGrabCondition* onGrabCondition = new OnGrabCondition(enemy);
	HitStun* hitStun = new HitStun(enemy, "enemigo_bola_recibirdanio");
	OnHit* onHit = new OnHit(enemy, "enemigo_bola_recibirdanio");
	HorizontalLaunchedAction* horizontalLaunchedAction = new HorizontalLaunchedAction(enemy);
	OnHorizontalLaunchAction* onHorizontalLaunchAction = new OnHorizontalLaunchAction(enemy, "enemigo_bola_recibirdanio");
	OnHorizontalLaunchCondition* onHorizontalLaunchCondition = new OnHorizontalLaunchCondition(enemy);
	OnVerticalLaunchAction* onVerticalLaunchAction = new OnVerticalLaunchAction(enemy, "enemigo_bola_recibirdanio");
	OnVerticalLaunchCondition* onVerticalLaunchCondition = new OnVerticalLaunchCondition(enemy);
	VerticalLaunchedAction* verticalLaunchedAction = new VerticalLaunchedAction(enemy);
	OnPropelAction* onPropelAction = new OnPropelAction(enemy);
	OnPropelCondition * onPropelCondition = new OnPropelCondition(enemy);
	PropelAction* propelAction = new PropelAction(enemy);
	OnStunAction* onStunAction = new OnStunAction(enemy, "enemigo_bola_stun", "enemigo_bola_stun_loop");
	OnStunCondition* onStunCondition = new OnStunCondition(enemy);
	StunAction* stunAction = new StunAction(enemy);
	OnIdleAction* onIdleAction = new OnIdleAction(enemy, "enemigo_bola_idle");
	IdleAction* idleAction = new IdleAction(enemy, "enemigo_bola_idle");
	OnIdleLoop* onIdleLoop = new OnIdleLoop(enemy, "enemigo_bola_idle");
	IdleLoop* idleLoop = new IdleLoop(enemy, "enemigo_bola_idle");
	OnTeleportAction* onTeleportAction = new OnTeleportAction(enemy, "enemigo_bola_desaparecer");
	OnTeleportCondition* onTeleportCondition = new OnTeleportCondition(enemy);
	TeleportAction* teleportAction = new TeleportAction(enemy, "enemigo_bola_desaparecer");
	AppearAction* appearAction = new AppearAction(enemy, "enemigo_bola_aparecer");
	OnAppearAction* onAppearAction = new OnAppearAction(enemy, "enemigo_bola_aparecer");
	CombatCondition* combatCondition = new CombatCondition(enemy);
	AttackAction* spinAttackAction = new AttackAction(enemy, "enemigo_bola_ataquerapido", "spin_attack");
	OnAttackAction* onSpinAttackAction = new OnAttackAction(enemy, "enemigo_bola_ataquerapido", "spin_attack");
	AttackAction* areaAttackAction = new AttackAction(enemy, "enemigo_bola_ataquelento", "area_attack");
	OnAttackAction* onAreaAttackAction = new OnAttackAction(enemy, "enemigo_bola_ataquelento", "area_attack");
	ChaseAction* chaseAction = new ChaseAction(enemy);
	OnChaseAction* onChaseAction = new OnChaseAction(enemy, "enemigo_bola_chase");
	IdleWarAction* idleWarAction = new IdleWarAction(enemy, "enemigo_bola_idle_war");
	OnIdleWarAction* onIdleWarAction = new OnIdleWarAction(enemy, "enemigo_bola_idle_war");
	OnTackleAttackAction* onTackleAttackAction = new OnTackleAttackAction(enemy, "enemigo_bola_placaje_loop", "tackle");
	TackleAttackAction* tackleAttackAction = new TackleAttackAction(enemy, "enemigo_bola_placaje_loop", "tackle", enemy->movementSpeed * 3.0f, 1.0f);
	OnStepBackAction* onStepBackAction = new OnStepBackAction(enemy, "enemigo_bola_run");
	StepBackAction* stepBackAction = new StepBackAction(enemy, "enemigo_bola_run", enemy->movementSpeed);
	OnReturnToSpawnAction* onReturnToSpawnAction = new OnReturnToSpawnAction(enemy, "enemigo_bola_run");
	OnReturnToSpawnCondition* onReturnToSpawnCondition = new OnReturnToSpawnCondition(enemy);
	ReturnToSpawnAction* returnToSpawnAction = new ReturnToSpawnAction(enemy, "combat");
	OnStrollAction* onStrollAction = new OnStrollAction(enemy, "enemigo_bola_run");
	StrollAction* strollAction = new StrollAction(enemy, "combat");
	BlockingBreakAction* blockingBreakAction = new BlockingBreakAction(enemy, "enemigo_bola_recibirdanio");
	OnBlockingBreakAction* onBlockingBreakAction = new OnBlockingBreakAction(enemy, "enemigo_bola_recibirdanio");
	OnBlockingBreakCondition* onBlockingBreakCondition = new OnBlockingBreakCondition(enemy);
	BlockAction* blockAction = new BlockAction(enemy);
	OnBlockAction* onBlockAction = new OnBlockAction(enemy, "enemigo_bola_guardia", "enemigo_bola_guardia_loop");
	BlockCondition* blockCondition = new BlockCondition(enemy);
	AttackCoolDownCondition* attackCoolDownCondition = new AttackCoolDownCondition(enemy);

	//root
	enemy->createRoot("dreidel", Priority, nullptr, nullptr);

	//getting hit
	enemy->addChild("dreidel", "hit", Sequence, falseCondition, nullptr);
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

	//stun
	enemy->addChild("dreidel", "stun", Sequence, onStunCondition, nullptr);
	enemy->addChild("stun", "onStunAction", Action, nullptr, onStunAction);
	enemy->addChild("stun", "stunAction", Action, nullptr, stunAction);

	//airborne
	enemy->addChild("dreidel", "onAirborneAction", Action, onAirborneCondition, onAirborneAction);
	enemy->addChild("dreidel", "airborneAction", Action, airborneCondition, airborneAction);

	//death
	enemy->addChild("dreidel", "death", Sequence, onDeathCondition, nullptr);
	enemy->addChild("death", "onDeathAction", Action, nullptr, onDeathAction);
	enemy->addChild("death", "deathAction", Action, nullptr, deathAction);
	//disappear
	enemy->addChild("death", "disappear", Sequence, nullptr, nullptr);
	enemy->addChild("disappear", "onDisappearAction", Action, nullptr, onDisappearAction);
	enemy->addChild("disappear", "disappearAction", Action, nullptr, disappearAction);

	//block break
	enemy->addChild("dreidel", "blockingBreak", Sequence, onBlockingBreakCondition, nullptr);
	enemy->addChild("blockingBreak", "onBlockingBreakAction", Action, nullptr, onBlockingBreakAction);
	enemy->addChild("blockingBreak", "blockingBreakAction", Action, nullptr, blockingBreakAction);

	//block
	enemy->addChild("dreidel", "blockAction", Action, blockCondition, blockAction);


	//teleport to spawn
	enemy->addChild("dreidel", "teleport", Sequence, onTeleportCondition, nullptr);
	enemy->addChild("teleport", "onTeleportAction", Action, nullptr, onTeleportAction);
	enemy->addChild("teleport", "teleportAction", Action, nullptr, teleportAction);
	//appear
	enemy->addChild("teleport", "appear", Sequence, nullptr, nullptr);
	enemy->addChild("appear", "onAppearAction", Action, nullptr, onAppearAction);
	enemy->addChild("appear", "appearAction", Action, nullptr, appearAction);

	//combat
	enemy->addChild("dreidel", "combat", Priority, combatCondition, nullptr);
	//attack cool down
	enemy->addChild("combat", "attackCooldownCombat", Random, attackCoolDownCondition, nullptr);
	//idle war
	enemy->addChild("attackCooldownCombat", "CDIdleWar", Sequence, nullptr, nullptr);
	enemy->addChild("CDIdleWar", "onCDIdleWar", Action, nullptr, onIdleWarAction);
	enemy->addChild("CDIdleWar", "CDIdleWarAction", Action, nullptr, idleWarAction);
	//short distance combat
	enemy->addChild("combat", "shortDistanceCombat", Random, shortDistanceCombatCondition, nullptr);
	//idle war
	enemy->addChild("shortDistanceCombat", "shortIdleWar", Sequence, nullptr, nullptr);
	enemy->addChild("shortIdleWar", "onShortIdleWar", Action, nullptr, onIdleWarAction);
	enemy->addChild("shortIdleWar", "shortIdleWarAction", Action, nullptr, idleWarAction);
	//spin attack
	enemy->addChild("shortDistanceCombat", "shortSpinAttack", Sequence, nullptr, nullptr);
	enemy->addChild("shortSpinAttack", "onShortSpinAttackAction", Action, nullptr, onSpinAttackAction);
	enemy->addChild("shortSpinAttack", "shortSpinAttackAction", Action, nullptr, spinAttackAction);
	//step back
	enemy->addChild("shortDistanceCombat", "stepBack", Sequence, nullptr, nullptr);
	enemy->addChild("stepBack", "onStepBackAction", Action, nullptr, onStepBackAction);
	enemy->addChild("stepBack", "stepBackAction", Action, nullptr, stepBackAction);
	//block;
	enemy->addChild("shortDistanceCombat", "onBlockAction", Action, nullptr, onBlockAction);
	//medium distance combat
	enemy->addChild("combat", "mediumDistanceCombat", Random, mediumDistanceCombatCondition, nullptr);
	//idle war
	enemy->addChild("mediumDistanceCombat", "mediumIdleWar", Sequence, nullptr, nullptr);
	enemy->addChild("mediumIdleWar", "onMediumIdleWar", Action, nullptr, onIdleWarAction);
	enemy->addChild("mediumIdleWar", "mediumIdleWarAction", Action, nullptr, idleWarAction);
	//spin attack
	enemy->addChild("mediumDistanceCombat", "mediumSpinAttack", Sequence, nullptr, nullptr);
	enemy->addChild("mediumSpinAttack", "onMediumSpinAttackAction", Action, nullptr, onSpinAttackAction);
	enemy->addChild("mediumSpinAttack", "mediumSpinAttackAction", Action, nullptr, spinAttackAction);
	//area attack
	enemy->addChild("mediumDistanceCombat", "mediumAreaAttack", Sequence, nullptr, nullptr);
	enemy->addChild("mediumAreaAttack", "onMediumAreaAttackAction", Action, nullptr, onAreaAttackAction);
	enemy->addChild("mediumAreaAttack", "mediumAreaAttackAction", Action, nullptr, areaAttackAction);
	//tackle
	enemy->addChild("mediumDistanceCombat", "mediumTackle", Sequence, nullptr, nullptr);
	enemy->addChild("mediumTackle", "onMediumTackleAttackAction", Action, nullptr, onTackleAttackAction);
	enemy->addChild("mediumTackle", "mediumTackleAttackAction", Action, nullptr, tackleAttackAction);
	//long distance combat
	enemy->addChild("combat", "longDistanceCombat", Random, longDistanceCombatCondition, nullptr);
	//idle war
	enemy->addChild("longDistanceCombat", "longIdleWar", Sequence, nullptr, nullptr);
	enemy->addChild("longIdleWar", "onLongIdleWar", Action, nullptr, onIdleWarAction);
	enemy->addChild("longIdleWar", "longIdleWarAction", Action, nullptr, idleWarAction);
	//tackle
	enemy->addChild("longDistanceCombat", "longTackle", Sequence, nullptr, nullptr);
	enemy->addChild("longTackle", "onLongTackleAttackAction", Action, nullptr, onTackleAttackAction);
	enemy->addChild("longTackle", "longTackleAttackAction", Action, nullptr, tackleAttackAction);
	//chase
	enemy->addChild("combat", "chase", Sequence, nullptr, nullptr);
	enemy->addChild("chase", "onChaseAction", Action, nullptr, onChaseAction);
	enemy->addChild("chase", "chaseAction", Action, nullptr, chaseAction);

	//return to spawn walking
	enemy->addChild("dreidel", "returnToSpawn", Sequence, onReturnToSpawnCondition, nullptr);
	enemy->addChild("returnToSpawn", "onReturnToSpawnAction", Action, nullptr, onReturnToSpawnAction);
	enemy->addChild("returnToSpawn", "returnToSpawnAction", Action, nullptr, returnToSpawnAction);

	//idle
	enemy->addChild("dreidel", "idle", Random, nullptr, nullptr);
	//idle action
	enemy->addChild("idle", "idleActionSeq", Sequence, nullptr, nullptr);
	enemy->addChild("idleActionSeq", "onIdleAction", Action, nullptr, onIdleAction);
	enemy->addChild("idleActionSeq", "idleAction", Action, nullptr, idleAction);
	//idle loop
	enemy->addChild("idle", "idleLoopSeq", Sequence, nullptr, nullptr);
	enemy->addChild("idleLoopSeq", "onIdleLoop", Action, nullptr, onIdleLoop);
	enemy->addChild("idleLoopSeq", "idleLoop", Action, nullptr, idleLoop);
	//stroll
	enemy->addChild("idle", "stroll", Sequence, nullptr, nullptr);
	enemy->addChild("stroll", "onStrollAction", Action, nullptr, onStrollAction);
	enemy->addChild("stroll", "strollAction", Action, nullptr, strollAction);
}

void Dreidel::load(const json& j, TEntityParseContext& ctx) {
	SAFE_DELETE(enemy);
	enemy = new Enemy();
	enemy->load(j, ctx);
}

void Dreidel::debugInMenu() {
	enemy->debugInMenu();
}

void Dreidel::registerMsgs() {
	DECL_MSG(Dreidel, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(Dreidel, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(Dreidel, TMsgAttackHit, onAttackHit);
	DECL_MSG(Dreidel, TMsgOutOfBounds, onOutOfBounds);
	DECL_MSG(Dreidel, TMsgPerfectDodged, onPerfectDodged);
	DECL_MSG(Dreidel, TMsgColliderDestroyed, onColliderDestroyed);
	DECL_MSG(Dreidel, TMsgHitboxEnter, onHitboxEnter);
}

void Dreidel::update(float delta) {
	if (!enemy->getCollider()->toDestroy) {
		enemy->deltaMovement = VEC3::Zero;
		enemy->recalc(delta);
		if (enemy->getCollider()->isCreated()) enemy->move(delta);
	}
}

void Dreidel::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
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

void Dreidel::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	enemy->playerHandle = getEntityByName(PLAYER_NAME);
	if (enemy->playerHandle.isValid()) {
		CEntity* player = enemy->playerHandle;
		enemy->playerTransformHandle = player->get<TCompTransform>();
		enemy->playerModelHandle = player->get<TCompPlayerModel>();
	}
	enemy->spawnPosition = enemy->getTransform()->getPosition();
}

void Dreidel::onAttackHit(const TMsgAttackHit& msg) {
	if (!enemy->getCollider()->toDestroy) {
		enemy->receivedAttack = msg.info;
		enemy->setCurrent("hit");
	}
}

void Dreidel::onOutOfBounds(const TMsgOutOfBounds& msg) {
	if (!enemy->getCollider()->toDestroy) {
		enemy->setCurrent("disappear");
	}
}

void Dreidel::onPerfectDodged(const TMsgPerfectDodged & msg) {
	if (!enemy->getCollider()->toDestroy) {
		dbg("Damn! I've been dodged.\n");
		if (hpGiven < maxHpToGive) {
			enemy->getPlayerModel()->setHp(enemy->getPlayerModel()->getHp() + 1);
			hpGiven++;
		}
	}
}

void Dreidel::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	if (enemy->getCollider()->toDestroy) {
		CEntity* entity = CHandle(this).getOwner();
		((CModuleGame*)(EngineModules.getModule("game")))->getEnemyManager()->onDead(entity->getName());
		enemy->getSkeleton()->stop();
		CHandle(this).getOwner().destroy();
	}
}

void Dreidel::onHitboxEnter(const TMsgHitboxEnter& msg) {
	if (enemy->currentAttack != "") {
		if (enemy->attacks[enemy->currentAttack].hitboxName == msg.hitbox) {
			TMsgAttackHit attackHit = {};
			attackHit.attacker = CHandle(this).getOwner();
			attackHit.info = enemy->attacks[enemy->currentAttack].attackInfo;
			((CEntity*)msg.h_other_entity)->sendMsg(attackHit);
		}
	}
}