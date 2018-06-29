#include "mcv_platform.h"
#include "Dreidel.h"

#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"

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

DECL_OBJ_MANAGER("dreidel", Dreidel);

void Dreidel::initBehaviorTree() {
	//TODO delete
	FalseCondition* falseCondition = new FalseCondition();
	AirborneAction* airborneAction = new AirborneAction(this);
	AirborneCondition* airborneCondition = new AirborneCondition(this);
	OnAirborneAction* onAirborneAction = new OnAirborneAction(this, "enemigo_bola_idle");
	OnAirborneCondition* onAirborneCondition = new OnAirborneCondition(this);
	BlockingBreakAction* blockingBreakAction = new BlockingBreakAction(this, "enemigo_bola_guardia");
	OnBlockingBreakAction* onBlockingBreakAction = new OnBlockingBreakAction(this, "enemigo_bola_guardia");
	OnBlockingBreakCondition* onBlockingBreakCondition = new OnBlockingBreakCondition(this);
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
	OnStunAction* onStunAction = new OnStunAction(this, "enemigo_bola_stun");
	OnStunCondition* onStunCondition = new OnStunCondition(this);
	StunAction* stunAction = new StunAction(this);
	OnIdleAction* onIdleAction = new OnIdleAction(this, "enemigo_bola_idle");
	IdleAction* idleAction = new IdleAction(this, "enemigo_bola_idle");
	OnIdleLoop* onIdleLoop = new OnIdleLoop(this, "enemigo_bola_idle");
	IdleLoop* idleLoop = new IdleLoop(this, "enemigo_bola_idle");
	OnStrollAction* onStrollAction = new OnStrollAction(this, "enemigo_bola_run");
	StrollAction* strollAction = new StrollAction(this);
	OnTeleportAction* onTeleportAction = new OnTeleportAction(this, "enemigo_bola_desaparecer");
	OnTeleportCondition* onTeleportCondition = new OnTeleportCondition(this);
	TeleportAction* teleportAction = new TeleportAction(this, "enemigo_bola_desaparecer");
	OnReturnToSpawnAction* onReturnToSpawnAction = new OnReturnToSpawnAction(this, "enemigo_bola_run");
	OnReturnToSpawnCondition* onReturnToSpawnCondition = new OnReturnToSpawnCondition(this);
	ReturnToSpawnAction* returnToSpawnAction = new ReturnToSpawnAction(this);
	AppearAction* appearAction = new AppearAction(this, "enemigo_bola_aparecer");
	OnAppearAction* onAppearAction = new OnAppearAction(this, "enemigo_bola_aparecer");

	createRoot("dreidel", Priority, nullptr, nullptr);

	addChild("dreidel", "hit", Sequence, falseCondition, nullptr);
	addChild("hit", "onHit", Action, nullptr, onHit);
	addChild("hit", "attackProperties", Priority, nullptr, nullptr);
	addChild("attackProperties", "horizontalLaunch", Sequence, onHorizontalLaunchCondition, nullptr);
	addChild("horizontalLaunch", "onHorizontalLaunchAction", Action, nullptr, onHorizontalLaunchAction);
	addChild("horizontalLaunch", "horizontalLaunchedAction", Action, nullptr, horizontalLaunchedAction);
	addChild("attackProperties", "verticalLaunch", Sequence, onVerticalLaunchCondition, nullptr);
	addChild("verticalLaunch", "onVerticalLaunchAction", Action, nullptr, onVerticalLaunchAction);
	addChild("verticalLaunch", "verticalLaunchedAction", Action, nullptr, verticalLaunchedAction);
	addChild("attackProperties", "grab", Sequence, onGrabCondition, nullptr);
	addChild("grab", "onGrabAction", Action, nullptr, onGrabAction);
	addChild("grab", "grabAction", Action, nullptr, grabAction);
	addChild("attackProperties", "propel", Sequence, onPropelCondition, nullptr);
	addChild("propel", "onPropelAction", Action, nullptr, onPropelAction);
	addChild("propel", "propelAction", Action, nullptr, propelAction);
	addChild("hit", "hitStun", Action, nullptr, hitStun);

	addChild("dreidel", "blockingBreak", Sequence, onBlockingBreakCondition, nullptr);
	addChild("blockingBreak", "onBlockingBreakAction", Action, nullptr, onBlockingBreakAction);
	addChild("blockingBreak", "blockingBreakAction", Action, nullptr, blockingBreakAction);

	addChild("dreidel", "death", Sequence, onDeathCondition, nullptr);
	addChild("death", "onDeathAction", Action, nullptr, onDeathAction);
	addChild("death", "deathAction", Action, nullptr, deathAction);
	addChild("death", "disappear", Sequence, nullptr, nullptr);
	addChild("disappear", "onDisappearAction", Action, nullptr, onDisappearAction);
	addChild("disappear", "disappearAction", Action, nullptr, disappearAction);

	addChild("dreidel", "stun", Sequence, onStunCondition, nullptr);
	addChild("stun", "onStunAction", Action, nullptr, onStunAction);
	addChild("stun", "stunAction", Action, nullptr, stunAction);

	addChild("dreidel", "onAirborneAction", Action, onAirborneCondition, onAirborneAction);
	addChild("dreidel", "airborneAction", Action, airborneCondition, airborneAction);

	addChild("dreidel", "teleport", Sequence, onTeleportCondition, nullptr);
	addChild("teleport", "onTeleportAction", Action, nullptr, onTeleportAction);
	addChild("teleport", "teleportAction", Action, nullptr, teleportAction);
	addChild("teleport", "appear", Sequence, nullptr, nullptr);
	addChild("appear", "onAppearAction", Action, nullptr, onAppearAction);
	addChild("appear", "appearAction", Action, nullptr, appearAction);

	addChild("dreidel", "returnToSpawn", Sequence, onReturnToSpawnCondition, nullptr);
	addChild("returnToSpawn", "onReturnToSpawnAction", Action, nullptr, onReturnToSpawnAction);
	addChild("returnToSpawn", "returnToSpawnAction", Action, nullptr, returnToSpawnAction);

	addChild("dreidel", "idle", Random, nullptr, nullptr);
	addChild("idle", "idleActionSeq", Sequence, nullptr, nullptr);
	addChild("idleActionSeq", "onIdleAction", Action, nullptr, onIdleAction);
	addChild("idleActionSeq", "idleAction", Action, nullptr, idleAction);
	addChild("idle", "idleLoopSeq", Sequence, nullptr, nullptr);
	addChild("idleLoopSeq", "onIdleLoop", Action, nullptr, onIdleLoop);
	addChild("idleLoopSeq", "idleLoop", Action, nullptr, idleLoop);
	addChild("idle", "stroll", Sequence, nullptr, nullptr);
	addChild("stroll", "onStrollAction", Action, nullptr, onStrollAction);
	addChild("stroll", "strollAction", Action, nullptr, strollAction);
}

void Dreidel::load(const json& j, TEntityParseContext& ctx) {
	Enemy::load(j, ctx);
}

void Dreidel::debugInMenu() {
}

void Dreidel::registerMsgs() {
	DECL_MSG(Dreidel, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(Dreidel, TMsgAttackHit, onAttackHit);
	DECL_MSG(Dreidel, TMsgOutOfBounds, onOutOfBounds);
	DECL_MSG(Dreidel, TMsgPerfectDodged, onPerfectDodged);
	DECL_MSG(Dreidel, TMsgColliderDestroyed, onColliderDestroyed);
	//DECL_MSG(Dreidel, TMsgHitboxEnter, onHitboxEnter);
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

	playerHandle = getEntityByName(PLAYER_NAME);
	transformHandle = get<TCompTransform>();
	colliderHandle = get<TCompCollider>();
	skeletonHandle = get<TCompSkeleton>();
	hitboxesHandle = get<TCompHitboxes>();

	spawnPosition = getTransform()->getPosition();

	current = tree["appear"];
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
		getSkeleton()->stop();
		CHandle(this).getOwner().destroy();
	}
}

//void Dreidel::onHitboxEnter(const TMsgHitboxEnter& msg) {
//	if (attackInfos.find(msg.hitbox) != attackInfos.end()) {
//		TMsgAttackHit attackHit = {};
//		attackHit.attacker = CHandle(this).getOwner();
//		attackHit.info = attackInfos[msg.hitbox];
//		((CEntity*)msg.h_other_entity)->sendMsg(attackHit);
//	}
//}