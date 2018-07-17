#include "mcv_platform.h"
#include "Kippah.h"

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
#include "components/ia/enemies/kippah/ranged_attack/RangedAttackAction.h"
#include "components/ia/enemies/common/combat/AttackCoolDownCondition.h"

DECL_OBJ_MANAGER("kippah", Kippah);

Kippah::~Kippah() {
}

void Kippah::initBehaviorTree() {
	clear();

	FalseCondition* falseCondition = new FalseCondition();
	LongDistanceCombatCondition* longDistanceCombatCondition = new LongDistanceCombatCondition(this);
	MediumDistanceCombatCondition* mediumDistanceCombatCondition = new MediumDistanceCombatCondition(this);
	ShortDistanceCombatCondition* shortDistanceCombatCondition = new ShortDistanceCombatCondition(this);
	DeathAction* deathAction = new DeathAction(this, "medusa_death");
	DisappearAction* disappearAction = new DisappearAction(this, "medusa_desaparicion");
	OnDeathAction* onDeathAction = new OnDeathAction(this, "medusa_death");
	OnDeathCondition* onDeathCondition = new OnDeathCondition(this);
	OnDisappearAction* onDisappearAction = new OnDisappearAction(this, "medusa_desaparicion");
	GrabAction* grabAction = new GrabAction(this);
	OnGrabAction* onGrabAction = new OnGrabAction(this);
	OnGrabCondition* onGrabCondition = new OnGrabCondition(this);
	HitStun* hitStun = new HitStun(this, "medusa_damage");
	OnHit* onHit = new OnHit(this, "medusa_damage");
	HorizontalLaunchedAction* horizontalLaunchedAction = new HorizontalLaunchedAction(this, "launched");
	OnHorizontalLaunchAction* onHorizontalLaunchAction = new OnHorizontalLaunchAction(this, "medusa_damage", "launched");
	OnHorizontalLaunchCondition* onHorizontalLaunchCondition = new OnHorizontalLaunchCondition(this);
	OnVerticalLaunchAction* onVerticalLaunchAction = new OnVerticalLaunchAction(this, "medusa_damage");
	OnVerticalLaunchCondition* onVerticalLaunchCondition = new OnVerticalLaunchCondition(this);
	VerticalLaunchedAction* verticalLaunchedAction = new VerticalLaunchedAction(this);
	OnPropelAction* onPropelAction = new OnPropelAction(this);
	OnPropelCondition * onPropelCondition = new OnPropelCondition(this);
	PropelAction* propelAction = new PropelAction(this);
	OnStunAction* onStunAction = new OnStunAction(this, "", "medusa_stun");
	OnStunCondition* onStunCondition = new OnStunCondition(this);
	StunAction* stunAction = new StunAction(this);
	OnIdleAction* onIdleAction = new OnIdleAction(this, "medusa_idle");
	IdleActionFlying* idleActionFlying = new IdleActionFlying(this, "medusa_idle", shortDistanceCombatCondition);
	OnIdleLoop* onIdleLoop = new OnIdleLoop(this, "medusa_idle");
	IdleLoopFlying* idleLoopFlying = new IdleLoopFlying(this, "medusa_idle", shortDistanceCombatCondition);
	OnTeleportAction* onTeleportAction = new OnTeleportAction(this, "medusa_desaparicion");
	OnTeleportCondition* onTeleportCondition = new OnTeleportCondition(this);
	TeleportAction* teleportAction = new TeleportAction(this, "medusa_desaparicion");
	AppearAction* appearAction = new AppearAction(this, "medusa_aparicion");
	OnAppearAction* onAppearAction = new OnAppearAction(this, "medusa_aparicion");
	CombatCondition* combatCondition = new CombatCondition(this);
	AttackActionFlying* spinAttackAction = new AttackActionFlying(this, "medusa_revolverse", "spinAttack");
	OnAttackAction* onSpinAttackAction = new OnAttackAction(this, "medusa_revolverse", "spinAttack");
	IdleWarActionFlying* idleWarActionFlying = new IdleWarActionFlying(this, "medusa_idle_war");
	OnIdleWarAction* onIdleWarAction = new OnIdleWarAction(this, "medusa_idle_war");
	OnReturnToSpawnAction* onReturnToSpawnAction = new OnReturnToSpawnAction(this, "medusa_idle");
	OnReturnToSpawnCondition* onReturnToSpawnCondition = new OnReturnToSpawnCondition(this);
	ReturnToSpawnFlyingAction* returnToSpawnFlyingAction = new ReturnToSpawnFlyingAction(this, shortDistanceCombatCondition);
	OnAttackAction* onRangedAttackAction = new OnAttackAction(this, "medusa_shot", "rangedAttack");
	RangedAttackAction* rangedAttackAction = new RangedAttackAction(this, "medusa_shot", "rangedAttack", shortDistanceCombatCondition);
	AttackCoolDownCondition* attackCoolDownCondition = new AttackCoolDownCondition(this);

	//root
	createRoot("kippah", Priority, nullptr, nullptr);

	//getting hit
	addChild("kippah", "hit", Sequence, falseCondition, nullptr);
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

	//death
	addChild("kippah", "death", Sequence, onDeathCondition, nullptr);
	addChild("death", "onDeathAction", Action, nullptr, onDeathAction);
	addChild("death", "deathAction", Action, nullptr, deathAction);
	//disappear
	addChild("death", "disappear", Sequence, nullptr, nullptr);
	addChild("disappear", "onDisappearAction", Action, nullptr, onDisappearAction);
	addChild("disappear", "disappearAction", Action, nullptr, disappearAction);

	//stun
	addChild("kippah", "stun", Sequence, onStunCondition, nullptr);
	addChild("stun", "onStunAction", Action, nullptr, onStunAction);
	addChild("stun", "stunAction", Action, nullptr, stunAction);

	//teleport to spawn
	addChild("kippah", "teleport", Sequence, onTeleportCondition, nullptr);
	addChild("teleport", "onTeleportAction", Action, nullptr, onTeleportAction);
	addChild("teleport", "teleportAction", Action, nullptr, teleportAction);
	//appear
	addChild("teleport", "appear", Sequence, nullptr, nullptr);
	addChild("appear", "onAppearAction", Action, nullptr, onAppearAction);
	addChild("appear", "appearAction", Action, nullptr, appearAction);

	//return to spawn walking
	addChild("kippah", "returnToSpawn", Sequence, onReturnToSpawnCondition, nullptr);
	addChild("returnToSpawn", "onReturnToSpawnAction", Action, nullptr, onReturnToSpawnAction);
	addChild("returnToSpawn", "returnToSpawnAction", Action, nullptr, returnToSpawnFlyingAction);

	//combat
	addChild("kippah", "combat", Priority, combatCondition, nullptr);
	//short distance combat
	addChild("combat", "shortDistanceCombat", Sequence, shortDistanceCombatCondition, nullptr);
	//spin attack
	addChild("shortDistanceCombat", "shortSpinAttack", Sequence, nullptr, nullptr);
	addChild("shortSpinAttack", "onShortSpinAttackAction", Action, nullptr, onSpinAttackAction);
	addChild("shortSpinAttack", "shortSpinAttackAction", Action, nullptr, spinAttackAction);
	//idle war
	addChild("shortDistanceCombat", "shortIdleWar", Sequence, nullptr, nullptr);
	addChild("shortIdleWar", "onShortIdleWar", Action, nullptr, onIdleWarAction);
	addChild("shortIdleWar", "shortIdleWarAction", Action, nullptr, idleWarActionFlying);
	//attack cool down
	addChild("combat", "attackCooldownCombat", Random, attackCoolDownCondition, nullptr);
	//idle war
	addChild("attackCooldownCombat", "CDIdleWar", Sequence, nullptr, nullptr);
	addChild("CDIdleWar", "onCDIdleWar", Action, nullptr, onIdleWarAction);
	addChild("CDIdleWar", "CDIdleWarAction", Action, nullptr, idleWarActionFlying);
	//long distance combat
	addChild("combat", "longDistanceCombat", Random, longDistanceCombatCondition, nullptr);
	//ranged attack
	addChild("longDistanceCombat", "rangedAttack", Sequence, nullptr, nullptr);
	addChild("rangedAttack", "onRangedAttackAction", Action, nullptr, onRangedAttackAction);
	addChild("rangedAttack", "rangedAttackAction", Action, nullptr, rangedAttackAction);
	//idle war
	addChild("longDistanceCombat", "longIdleWar", Sequence, nullptr, nullptr);
	addChild("longIdleWar", "onLongIdleWar", Action, nullptr, onIdleWarAction);
	addChild("longIdleWar", "longIdleWarAction", Action, nullptr, idleWarActionFlying);

	//idle
	addChild("kippah", "idle", Random, nullptr, nullptr);
	//idle action
	addChild("idle", "idleActionSeq", Sequence, nullptr, nullptr);
	addChild("idleActionSeq", "onIdleAction", Action, nullptr, onIdleAction);
	addChild("idleActionSeq", "idleAction", Action, nullptr, idleActionFlying);
	//idle loop
	addChild("idle", "idleLoopSeq", Sequence, nullptr, nullptr);
	addChild("idleLoopSeq", "onIdleLoop", Action, nullptr, onIdleLoop);
	addChild("idleLoopSeq", "idleLoop", Action, nullptr, idleLoopFlying);
}

void Kippah::load(const json& j, TEntityParseContext& ctx) {
	Enemy::load(j, ctx);
}

void Kippah::debugInMenu() {
	Enemy::debugInMenu();
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
	if (!getCollider()->toDestroy) {
		deltaMovement = VEC3::Zero;
		recalc(delta);
		if (getCollider()->isCreated()) move(delta);
	}
}

void Kippah::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	initBehaviorTree();

	playerHandle = getEntityByName(PLAYER_NAME);
	transformHandle = get<TCompTransform>();
	colliderHandle = get<TCompCollider>();
	skeletonHandle = get<TCompSkeleton>();
	hitboxesHandle = get<TCompHitboxes>();
	powerHandle = get<TCompGivePower>();

	spawnPosition = getTransform()->getPosition();

	current = tree["appear"];
}

void Kippah::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	playerHandle = getEntityByName(PLAYER_NAME);
	spawnPosition = getTransform()->getPosition();
}

void Kippah::onAttackHit(const TMsgAttackHit& msg) {
	if (!getCollider()->toDestroy) {
		receivedAttack = msg.info;
		current = tree["hit"];
	}
}

void Kippah::onOutOfBounds(const TMsgOutOfBounds& msg) {
	if (!getCollider()->toDestroy) {
		current = tree["disappear"];
	}
}

void Kippah::onPerfectDodged(const TMsgPerfectDodged & msg) {
	if (!getCollider()->toDestroy) {
		dbg("Damn! I've been dodged.\n");
		if (hpGiven < maxHpToGive) {
			getPlayerModel()->setHp(getPlayerModel()->getHp() + 1);
			hpGiven++;
		}
	}
}

void Kippah::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	if (getCollider()->toDestroy) {
		CEntity* entity = CHandle(this).getOwner();
		((CModuleGame*)(EngineModules.getModule("game")))->getEnemyManager()->onDead(entity->getName());
		getSkeleton()->stop();
		CHandle(this).getOwner().destroy();
	}
}

void Kippah::onHitboxEnter(const TMsgHitboxEnter& msg) {
	if (currentAttack != "") {
		if (msg.h_other_entity != CHandle(this).getOwner()) {
			if (attacks[currentAttack].hitboxName == msg.hitbox) {
				TMsgAttackHit attackHit = {};
				attackHit.attacker = CHandle(this).getOwner();
				attackHit.info = attacks[currentAttack].attackInfo;
				((CEntity*)msg.h_other_entity)->sendMsg(attackHit);
			}
		}
	}
}