#include "mcv_platform.h"
#include "Kippah.h"

#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"
#include "components/comp_give_power.h"

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

DECL_OBJ_MANAGER("kippah", Kippah);

Kippah::~Kippah() {
}

void Kippah::initBehaviorTree() {
	FalseCondition* falseCondition = new FalseCondition();
	AirborneAction* airborneAction = new AirborneAction(this);
	AirborneCondition* airborneCondition = new AirborneCondition(this);
	OnAirborneAction* onAirborneAction = new OnAirborneAction(this, "medusa_idle");
	OnAirborneCondition* onAirborneCondition = new OnAirborneCondition(this);
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
	HorizontalLaunchedAction* horizontalLaunchedAction = new HorizontalLaunchedAction(this);
	OnHorizontalLaunchAction* onHorizontalLaunchAction = new OnHorizontalLaunchAction(this, "medusa_damage");
	OnHorizontalLaunchCondition* onHorizontalLaunchCondition = new OnHorizontalLaunchCondition(this);
	OnVerticalLaunchAction* onVerticalLaunchAction = new OnVerticalLaunchAction(this, "medusa_damage");
	OnVerticalLaunchCondition* onVerticalLaunchCondition = new OnVerticalLaunchCondition(this);
	VerticalLaunchedAction* verticalLaunchedAction = new VerticalLaunchedAction(this);
	OnPropelAction* onPropelAction = new OnPropelAction(this);
	OnPropelCondition * onPropelCondition = new OnPropelCondition(this);
	PropelAction* propelAction = new PropelAction(this);
	OnStunAction* onStunAction = new OnStunAction(this, "medusa_stun", "medusa_stun");
	OnStunCondition* onStunCondition = new OnStunCondition(this);
	StunAction* stunAction = new StunAction(this);
	OnIdleAction* onIdleAction = new OnIdleAction(this, "medusa_idle");
	IdleAction* idleAction = new IdleAction(this, "medusa_idle");
	OnIdleLoop* onIdleLoop = new OnIdleLoop(this, "medusa_idle");
	IdleLoop* idleLoop = new IdleLoop(this, "medusa_idle");
	OnTeleportAction* onTeleportAction = new OnTeleportAction(this, "medusa_desaparicion");
	OnTeleportCondition* onTeleportCondition = new OnTeleportCondition(this);
	TeleportAction* teleportAction = new TeleportAction(this, "medusa_desaparicion");
	AppearAction* appearAction = new AppearAction(this, "medusa_aparicion");
	OnAppearAction* onAppearAction = new OnAppearAction(this, "medusa_aparicion");
	CombatCondition* combatCondition = new CombatCondition(this);
	AttackAction* spinAttackAction = new AttackAction(this, "medusa_revolverse", "attack");
	OnAttackAction* onSpinAttackAction = new OnAttackAction(this, "medusa_revolverse", "attack");
	IdleWarAction* idleWarAction = new IdleWarAction(this, "medusa_idle_war");
	OnIdleWarAction* onIdleWarAction = new OnIdleWarAction(this, "medusa_idle_war");
	LongDistanceCombatCondition* longDistanceCombatCondition = new LongDistanceCombatCondition(this);
	MediumDistanceCombatCondition* mediumDistanceCombatCondition = new MediumDistanceCombatCondition(this);
	ShortDistanceCombatCondition* shortDistanceCombatCondition = new ShortDistanceCombatCondition(this);
	OnReturnToSpawnAction* onReturnToSpawnAction = new OnReturnToSpawnAction(this, "medusa_idle");
	OnReturnToSpawnCondition* onReturnToSpawnCondition = new OnReturnToSpawnCondition(this);
	ReturnToSpawnAction* returnToSpawnAction = new ReturnToSpawnAction(this, combatCondition);

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

	//combat
	addChild("kippah", "combat", Priority, combatCondition, nullptr);
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
	//long distance combat
	addChild("combat", "longDistanceCombat", Random, longDistanceCombatCondition, nullptr);
	//idle war
	addChild("longDistanceCombat", "longIdleWar", Sequence, nullptr, nullptr);
	addChild("longIdleWar", "onLongIdleWar", Action, nullptr, onIdleWarAction);
	addChild("longIdleWar", "longIdleWarAction", Action, nullptr, idleWarAction);

	//return to spawn walking
	addChild("kippah", "returnToSpawn", Sequence, onReturnToSpawnCondition, nullptr);
	addChild("returnToSpawn", "onReturnToSpawnAction", Action, nullptr, onReturnToSpawnAction);
	addChild("returnToSpawn", "returnToSpawnAction", Action, nullptr, returnToSpawnAction);

	//idle
	addChild("kippah", "idle", Random, nullptr, nullptr);
	//idle action
	addChild("idle", "idleActionSeq", Sequence, nullptr, nullptr);
	addChild("idleActionSeq", "onIdleAction", Action, nullptr, onIdleAction);
	addChild("idleActionSeq", "idleAction", Action, nullptr, idleAction);
	//idle loop
	addChild("idle", "idleLoopSeq", Sequence, nullptr, nullptr);
	addChild("idleLoopSeq", "onIdleLoop", Action, nullptr, onIdleLoop);
	addChild("idleLoopSeq", "idleLoop", Action, nullptr, idleLoop);
}

void Kippah::load(const json& j, TEntityParseContext& ctx) {
	Enemy::load(j, ctx);
}

void Kippah::debugInMenu() {
}

void Kippah::registerMsgs() {
	DECL_MSG(Kippah, TMsgEntitiesGroupCreated, onGroupCreated);
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
	}
}

void Kippah::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	if (getCollider()->toDestroy) {
		getSkeleton()->stop();
		CHandle(this).getOwner().destroy();
	}
}

void Kippah::onHitboxEnter(const TMsgHitboxEnter& msg) {
	if (currentAttack != "") {
		if (attacks[currentAttack].hitboxName == msg.hitbox) {
			TMsgAttackHit attackHit = {};
			attackHit.attacker = CHandle(this).getOwner();
			attackHit.info = attacks[currentAttack].attackInfo;
			((CEntity*)msg.h_other_entity)->sendMsg(attackHit);
		}
	}
}