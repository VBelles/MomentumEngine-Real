#include "mcv_platform.h"
#include "Dreidel.h"

#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"
#include "components/comp_give_power.h"
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

DECL_OBJ_MANAGER("dreidel", Dreidel);

Dreidel::~Dreidel() {
}

void Dreidel::initBehaviorTree() {
	clear();

	FalseCondition* falseCondition = new FalseCondition();
	AirborneAction* airborneAction = new AirborneAction(this);
	LongDistanceCombatCondition* longDistanceCombatCondition = new LongDistanceCombatCondition(this);
	MediumDistanceCombatCondition* mediumDistanceCombatCondition = new MediumDistanceCombatCondition(this);
	ShortDistanceCombatCondition* shortDistanceCombatCondition = new ShortDistanceCombatCondition(this);
	AirborneCondition* airborneCondition = new AirborneCondition(this);
	OnAirborneAction* onAirborneAction = new OnAirborneAction(this, "enemigo_bola_idle");
	OnAirborneCondition* onAirborneCondition = new OnAirborneCondition(this);
	DeathAction* deathAction = new DeathAction(this, "enemigo_bola_muerte");
	DisappearAction* disappearAction = new DisappearAction(this, "enemigo_bola_desaparecer");
	OnDeathAction* onDeathAction = new OnDeathAction(this, "enemigo_bola_muerte");
	OnDeathCondition* onDeathCondition = new OnDeathCondition(this);
	OnDisappearAction* onDisappearAction = new OnDisappearAction(this, "enemigo_bola_desaparecer");
	GrabAction* grabAction = new GrabAction(this);
	OnGrabAction* onGrabAction = new OnGrabAction(this);
	OnGrabCondition* onGrabCondition = new OnGrabCondition(this);
	HitStun* hitStun = new HitStun(this, "enemigo_bola_recibirdanio");
	OnHit* onHit = new OnHit(this, "enemigo_bola_recibirdanio");
	HorizontalLaunchedAction* horizontalLaunchedAction = new HorizontalLaunchedAction(this);
	OnHorizontalLaunchAction* onHorizontalLaunchAction = new OnHorizontalLaunchAction(this, "enemigo_bola_recibirdanio");
	OnHorizontalLaunchCondition* onHorizontalLaunchCondition = new OnHorizontalLaunchCondition(this);
	OnVerticalLaunchAction* onVerticalLaunchAction = new OnVerticalLaunchAction(this, "enemigo_bola_recibirdanio");
	OnVerticalLaunchCondition* onVerticalLaunchCondition = new OnVerticalLaunchCondition(this);
	VerticalLaunchedAction* verticalLaunchedAction = new VerticalLaunchedAction(this);
	OnPropelAction* onPropelAction = new OnPropelAction(this);
	OnPropelCondition * onPropelCondition = new OnPropelCondition(this);
	PropelAction* propelAction = new PropelAction(this);
	OnStunAction* onStunAction = new OnStunAction(this, "enemigo_bola_stun", "enemigo_bola_stun_loop");
	OnStunCondition* onStunCondition = new OnStunCondition(this);
	StunAction* stunAction = new StunAction(this);
	OnIdleAction* onIdleAction = new OnIdleAction(this, "enemigo_bola_idle");
	IdleAction* idleAction = new IdleAction(this, "enemigo_bola_idle");
	OnIdleLoop* onIdleLoop = new OnIdleLoop(this, "enemigo_bola_idle");
	IdleLoop* idleLoop = new IdleLoop(this, "enemigo_bola_idle");
	OnTeleportAction* onTeleportAction = new OnTeleportAction(this, "enemigo_bola_desaparecer");
	OnTeleportCondition* onTeleportCondition = new OnTeleportCondition(this);
	TeleportAction* teleportAction = new TeleportAction(this, "enemigo_bola_desaparecer");
	AppearAction* appearAction = new AppearAction(this, "enemigo_bola_aparecer");
	OnAppearAction* onAppearAction = new OnAppearAction(this, "enemigo_bola_aparecer");
	CombatCondition* combatCondition = new CombatCondition(this);
	AttackAction* spinAttackAction = new AttackAction(this, "enemigo_bola_ataquerapido", "spin_attack");
	OnAttackAction* onSpinAttackAction = new OnAttackAction(this, "enemigo_bola_ataquerapido", "spin_attack");
	AttackAction* areaAttackAction = new AttackAction(this, "enemigo_bola_ataquelento", "area_attack");
	OnAttackAction* onAreaAttackAction = new OnAttackAction(this, "enemigo_bola_ataquelento", "area_attack");
	ChaseAction* chaseAction = new ChaseAction(this);
	OnChaseAction* onChaseAction = new OnChaseAction(this, "enemigo_bola_chase");
	IdleWarAction* idleWarAction = new IdleWarAction(this, "enemigo_bola_idle_war");
	OnIdleWarAction* onIdleWarAction = new OnIdleWarAction(this, "enemigo_bola_idle_war");
	OnTackleAttackAction* onTackleAttackAction = new OnTackleAttackAction(this, "enemigo_bola_placaje_loop", "tackle");
	TackleAttackAction* tackleAttackAction = new TackleAttackAction(this, "enemigo_bola_placaje_loop", "tackle", movementSpeed * 3.0f, 1.0f);
	OnStepBackAction* onStepBackAction = new OnStepBackAction(this, "enemigo_bola_run");
	StepBackAction* stepBackAction = new StepBackAction(this, "enemigo_bola_run", movementSpeed);
	OnReturnToSpawnAction* onReturnToSpawnAction = new OnReturnToSpawnAction(this, "enemigo_bola_run");
	OnReturnToSpawnCondition* onReturnToSpawnCondition = new OnReturnToSpawnCondition(this);
	ReturnToSpawnAction* returnToSpawnAction = new ReturnToSpawnAction(this, combatCondition);
	OnStrollAction* onStrollAction = new OnStrollAction(this, "enemigo_bola_run");
	StrollAction* strollAction = new StrollAction(this, combatCondition);
	BlockingBreakAction* blockingBreakAction = new BlockingBreakAction(this, "enemigo_bola_recibirdanio");
	OnBlockingBreakAction* onBlockingBreakAction = new OnBlockingBreakAction(this, "enemigo_bola_recibirdanio");
	OnBlockingBreakCondition* onBlockingBreakCondition = new OnBlockingBreakCondition(this);
	BlockAction* blockAction = new BlockAction(this);
	OnBlockAction* onBlockAction = new OnBlockAction(this, "enemigo_bola_guardia", "enemigo_bola_guardia_loop");
	BlockCondition* blockCondition = new BlockCondition(this);

	//root
	createRoot("dreidel", Priority, nullptr, nullptr);

	//getting hit
	addChild("dreidel", "hit", Sequence, falseCondition, nullptr);
	//damage calculation
	addChild("hit", "onHit", Action, nullptr, onHit);
	//attack properties
	addChild("hit", "attackProperties", Priority, nullptr, nullptr);
	//horizontal launch
	addChild("attackProperties", "horizontalLaunch", Sequence, onHorizontalLaunchCondition, nullptr);
	addChild("horizontalLaunch", "onHorizontalLaunchAction", Action, nullptr, onHorizontalLaunchAction);
	addChild("horizontalLaunch", "horizontalLaunchedAction", Action, nullptr, horizontalLaunchedAction);
	//vertical launch
	addChild("attackProperties", "verticalLaunch", Sequence, onVerticalLaunchCondition, nullptr);
	addChild("verticalLaunch", "onVerticalLaunchAction", Action, nullptr, onVerticalLaunchAction);
	addChild("verticalLaunch", "verticalLaunchedAction", Action, nullptr, verticalLaunchedAction);
	//grab
	addChild("attackProperties", "grab", Sequence, onGrabCondition, nullptr);
	addChild("grab", "onGrabAction", Action, nullptr, onGrabAction);
	addChild("grab", "grabAction", Action, nullptr, grabAction);
	//propel
	addChild("attackProperties", "propel", Sequence, onPropelCondition, nullptr);
	addChild("propel", "onPropelAction", Action, nullptr, onPropelAction);
	addChild("propel", "propelAction", Action, nullptr, propelAction);
	//hit stun
	addChild("attackProperties", "hitStun", Action, nullptr, hitStun);

	//stun
	addChild("dreidel", "stun", Sequence, onStunCondition, nullptr);
	addChild("stun", "onStunAction", Action, nullptr, onStunAction);
	addChild("stun", "stunAction", Action, nullptr, stunAction);

	//airborne
	addChild("dreidel", "onAirborneAction", Action, onAirborneCondition, onAirborneAction);
	addChild("dreidel", "airborneAction", Action, airborneCondition, airborneAction);

	//death
	addChild("dreidel", "death", Sequence, onDeathCondition, nullptr);
	addChild("death", "onDeathAction", Action, nullptr, onDeathAction);
	addChild("death", "deathAction", Action, nullptr, deathAction);
	//disappear
	addChild("death", "disappear", Sequence, nullptr, nullptr);
	addChild("disappear", "onDisappearAction", Action, nullptr, onDisappearAction);
	addChild("disappear", "disappearAction", Action, nullptr, disappearAction);

	//block break
	addChild("dreidel", "blockingBreak", Sequence, onBlockingBreakCondition, nullptr);
	addChild("blockingBreak", "onBlockingBreakAction", Action, nullptr, onBlockingBreakAction);
	addChild("blockingBreak", "blockingBreakAction", Action, nullptr, blockingBreakAction);

	//block
	addChild("dreidel", "blockAction", Action, blockCondition, blockAction);


	//teleport to spawn
	addChild("dreidel", "teleport", Sequence, onTeleportCondition, nullptr);
	addChild("teleport", "onTeleportAction", Action, nullptr, onTeleportAction);
	addChild("teleport", "teleportAction", Action, nullptr, teleportAction);
	//appear
	addChild("teleport", "appear", Sequence, nullptr, nullptr);
	addChild("appear", "onAppearAction", Action, nullptr, onAppearAction);
	addChild("appear", "appearAction", Action, nullptr, appearAction);

	//combat
	addChild("dreidel", "combat", Priority, combatCondition, nullptr);
	//short distance combat
	addChild("combat", "shortDistanceCombat", Random, shortDistanceCombatCondition, nullptr);
	//idle war
	addChild("shortDistanceCombat", "shortIdleWar", Sequence, nullptr, nullptr);
	addChild("shortIdleWar", "onShortIdleWar", Action, nullptr, onIdleWarAction);
	addChild("shortIdleWar", "shortIdleWarAction", Action, nullptr, idleWarAction);
	//spin attack
	addChild("shortDistanceCombat", "shortSpinAttack", Sequence, nullptr, nullptr);
	addChild("shortSpinAttack", "onShortSpinAttackAction", Action, nullptr, onSpinAttackAction);
	addChild("shortSpinAttack", "shortSpinAttackAction", Action, nullptr, spinAttackAction);
	//step back
	addChild("shortDistanceCombat", "stepBack", Sequence, nullptr, nullptr);
	addChild("stepBack", "onStepBackAction", Action, nullptr, onStepBackAction);
	addChild("stepBack", "stepBackAction", Action, nullptr, stepBackAction);
	//block;
	addChild("shortDistanceCombat", "onBlockAction", Action, nullptr, onBlockAction);
	//medium distance combat
	addChild("combat", "mediumDistanceCombat", Random, mediumDistanceCombatCondition, nullptr);
	//idle war
	addChild("mediumDistanceCombat", "mediumIdleWar", Sequence, nullptr, nullptr);
	addChild("mediumIdleWar", "onMediumIdleWar", Action, nullptr, onIdleWarAction);
	addChild("mediumIdleWar", "mediumIdleWarAction", Action, nullptr, idleWarAction);
	//spin attack
	addChild("mediumDistanceCombat", "mediumSpinAttack", Sequence, nullptr, nullptr);
	addChild("mediumSpinAttack", "onMediumSpinAttackAction", Action, nullptr, onSpinAttackAction);
	addChild("mediumSpinAttack", "mediumSpinAttackAction", Action, nullptr, spinAttackAction);
	//area attack
	addChild("mediumDistanceCombat", "mediumAreaAttack", Sequence, nullptr, nullptr);
	addChild("mediumAreaAttack", "onMediumAreaAttackAction", Action, nullptr, onAreaAttackAction);
	addChild("mediumAreaAttack", "mediumAreaAttackAction", Action, nullptr, areaAttackAction);
	//tackle
	addChild("mediumDistanceCombat", "mediumTackle", Sequence, nullptr, nullptr);
	addChild("mediumTackle", "onMediumTackleAttackAction", Action, nullptr, onTackleAttackAction);
	addChild("mediumTackle", "mediumTackleAttackAction", Action, nullptr, tackleAttackAction);
	//long distance combat
	addChild("combat", "longDistanceCombat", Random, longDistanceCombatCondition, nullptr);
	//idle war
	addChild("longDistanceCombat", "longIdleWar", Sequence, nullptr, nullptr);
	addChild("longIdleWar", "onLongIdleWar", Action, nullptr, onIdleWarAction);
	addChild("longIdleWar", "longIdleWarAction", Action, nullptr, idleWarAction);
	//tackle
	addChild("longDistanceCombat", "longTackle", Sequence, nullptr, nullptr);
	addChild("longTackle", "onLongTackleAttackAction", Action, nullptr, onTackleAttackAction);
	addChild("longTackle", "longTackleAttackAction", Action, nullptr, tackleAttackAction);
	//chase
	addChild("combat", "chase", Sequence, nullptr, nullptr);
	addChild("chase", "onChaseAction", Action, nullptr, onChaseAction);
	addChild("chase", "chaseAction", Action, nullptr, chaseAction);

	//return to spawn walking
	addChild("dreidel", "returnToSpawn", Sequence, onReturnToSpawnCondition, nullptr);
	addChild("returnToSpawn", "onReturnToSpawnAction", Action, nullptr, onReturnToSpawnAction);
	addChild("returnToSpawn", "returnToSpawnAction", Action, nullptr, returnToSpawnAction);

	//idle
	addChild("dreidel", "idle", Random, nullptr, nullptr);
	//idle action
	addChild("idle", "idleActionSeq", Sequence, nullptr, nullptr);
	addChild("idleActionSeq", "onIdleAction", Action, nullptr, onIdleAction);
	addChild("idleActionSeq", "idleAction", Action, nullptr, idleAction);
	//idle loop
	addChild("idle", "idleLoopSeq", Sequence, nullptr, nullptr);
	addChild("idleLoopSeq", "onIdleLoop", Action, nullptr, onIdleLoop);
	addChild("idleLoopSeq", "idleLoop", Action, nullptr, idleLoop);
	//stroll
	addChild("idle", "stroll", Sequence, nullptr, nullptr);
	addChild("stroll", "onStrollAction", Action, nullptr, onStrollAction);
	addChild("stroll", "strollAction", Action, nullptr, strollAction);
}

void Dreidel::load(const json& j, TEntityParseContext& ctx) {
	Enemy::load(j, ctx);
}

void Dreidel::debugInMenu() {
	Enemy::debugInMenu();
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
	if (!getCollider()->toDestroy) {
		deltaMovement = VEC3::Zero;
		recalc(delta);
		if (getCollider()->isCreated()) move(delta);
	}
}

void Dreidel::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	initBehaviorTree();

	transformHandle = get<TCompTransform>();
	colliderHandle = get<TCompCollider>();
	skeletonHandle = get<TCompSkeleton>();
	hitboxesHandle = get<TCompHitboxes>();
	powerHandle = get<TCompGivePower>();

	spawnPosition = getTransform()->getPosition();

	current = tree["appear"];
}

void Dreidel::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	playerHandle = getEntityByName(PLAYER_NAME);
}

void Dreidel::onAttackHit(const TMsgAttackHit& msg) {
	if (!getCollider()->toDestroy) {
		receivedAttack = msg.info;
		current = tree["hit"];
	}
}

void Dreidel::onOutOfBounds(const TMsgOutOfBounds& msg) {
	if (!getCollider()->toDestroy) {
		current = tree["disappear"];
	}
}

void Dreidel::onPerfectDodged(const TMsgPerfectDodged & msg) {
	if (!getCollider()->toDestroy) {
		dbg("Damn! I've been dodged.\n");
	}
}

void Dreidel::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	if (getCollider()->toDestroy) {
		CEntity* entity = CHandle(this).getOwner();
		((CModuleGame*)(EngineModules.getModule("game")))->getEnemyManager()->onDead(entity->getName());
		getSkeleton()->stop();
		CHandle(this).getOwner().destroy();
	}
}

void Dreidel::onHitboxEnter(const TMsgHitboxEnter& msg) {
	if (currentAttack != "") {
		if (attacks[currentAttack].hitboxName == msg.hitbox) {
			TMsgAttackHit attackHit = {};
			attackHit.attacker = CHandle(this).getOwner();
			attackHit.info = attacks[currentAttack].attackInfo;
			((CEntity*)msg.h_other_entity)->sendMsg(attackHit);
		}
	}
}