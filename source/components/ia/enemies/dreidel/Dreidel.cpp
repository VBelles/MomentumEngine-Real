#include "mcv_platform.h"
#include "Dreidel.h"
#include "entity/common_msgs.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_render.h"
#include "components/comp_respawner.h"
#include "components/comp_give_power.h"
#include "components/comp_hitboxes.h"
#include "components/player/power_stats.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/attack_info.h"
#include "components/ia/enemies/common/conditions/FalseCondition.h"
#include "actions/DamageCalcAction.h"
#include "conditions/DeathCondition.h"
#include "actions/OnDeathAction.h"
#include "conditions/GrabCondition.h"
#include "actions/OnGrabAction.h"
#include "actions/GrabbedAction.h"
#include "conditions/PropelCondition.h"
#include "actions/OnPropelAction.h"
#include "actions/PropelledAction.h"
#include "conditions/HorizontalLaunchCondition.h"
#include "actions/OnHorizontalLaunchAction.h"
#include "actions/HorizontalLaunchedAction.h"
#include "actions/FloatingAction.h"
#include "conditions/HorizontalLaunchCondition.h"
#include "actions/FloatingAction.h"
#include "conditions/VerticalLaunchCondition.h"
#include "actions/OnVerticalLaunchAction.h"
#include "actions/VerticalLaunchedAction.h"
#include "conditions/OnStunCondition.h"
#include "actions/OnStunAction.h"
#include "actions/RespawnAction.h"
#include "conditions/DeadCondition.h"
#include "actions/DeadAction.h"
#include "conditions/StunCondition.h"
#include "actions/StunnedAction.h"
#include "conditions/AirborneCondition.h"
#include "actions/AirborneAction.h"
#include "conditions/ReturnToSpawnCondition.h"
#include "actions/ReturnToSpawnAction.h"
#include "conditions/StepBackCondition.h"
#include "actions/StepBackAction.h"

#include "conditions/CombatCondition.h"
#include "actions/OnIdleWarAction.h"
#include "actions/IdleWarAction.h"
#include "actions/OnAttackAction.h"
#include "actions/AttackAction.h"
#include "conditions/ChaseCondition.h"
#include "actions/ChaseAction.h"
#include "actions/IdleAction.h"


DECL_OBJ_MANAGER("dreidel", Dreidel);

Dreidel::Dreidel()
	: IBehaviorTreeNew::IBehaviorTreeNew() {
	
}

void Dreidel::load(const json& j, TEntityParseContext& ctx) {
	createRoot("meleeEnemy", Priority, nullptr, nullptr);

	IBehaviorTreeCondition* falseCondition = new FalseCondition();
	IBehaviorTreeAction* damageCalcAction = new DamageCalcAction(this);
	IBehaviorTreeCondition* deathCondition = new DeathCondition(this);
	IBehaviorTreeAction* onDeathAction = new OnDeathAction(this);
	IBehaviorTreeCondition* grabCondition = new GrabCondition(this);
	IBehaviorTreeAction* onGrabAction = new OnGrabAction(this);
	IBehaviorTreeAction* grabbedAction = new GrabbedAction(this);
	IBehaviorTreeCondition* propelCondition = new PropelCondition(this);
	IBehaviorTreeAction* onPropelAction = new OnPropelAction(this);
	IBehaviorTreeAction* propelledAction = new PropelledAction(this);
	IBehaviorTreeCondition* horizontalLaunchCondition = new HorizontalLaunchCondition(this);
	IBehaviorTreeAction* onHorizontalLaunchAction = new OnHorizontalLaunchAction(this);
	IBehaviorTreeAction* horizontalLaunchedAction = new HorizontalLaunchedAction(this);
	IBehaviorTreeAction* floatingAction = new FloatingAction(this);
	IBehaviorTreeCondition* verticalLaunchCondition = new VerticalLaunchCondition(this);
	IBehaviorTreeAction* onVerticalLaunchAction = new OnVerticalLaunchAction(this);
	IBehaviorTreeAction* verticalLaunchedAction = new VerticalLaunchedAction(this);
	IBehaviorTreeCondition* onStunCondition = new OnStunCondition(this);
	IBehaviorTreeAction* onStunAction = new OnStunAction(this);
	IBehaviorTreeAction* respawnAction = new RespawnAction(this);
	IBehaviorTreeCondition* deadCondition = new DeadCondition(this);
	IBehaviorTreeAction* deadAction = new DeadAction(this);
	IBehaviorTreeCondition* stunCondition = new StunCondition(this);
	IBehaviorTreeAction* stunnedAction = new StunnedAction(this);
	IBehaviorTreeCondition* airborneCondition = new AirborneCondition(this);
	IBehaviorTreeAction* airborneAction = new AirborneAction(this);
	IBehaviorTreeCondition* returnToSpawnCondition = new ReturnToSpawnCondition(this);
	IBehaviorTreeAction* returnToSpawnAction = new ReturnToSpawnAction(this);
	IBehaviorTreeCondition* stepBackCondition = new StepBackCondition(this);
	IBehaviorTreeAction* stepBackAction = new StepBackAction(this);

	IBehaviorTreeCondition* combatCondition = new CombatCondition(this);
	IBehaviorTreeAction* onIdleWarAction = new OnIdleWarAction(this);
	IBehaviorTreeAction* idleWarAction = new IdleWarAction(this);
	IBehaviorTreeAction* onAttackAction = new OnAttackAction(this);
	IBehaviorTreeAction* attackAction = new AttackAction(this);
	IBehaviorTreeCondition* chaseCondition = new ChaseCondition(this);
	IBehaviorTreeAction* chaseAction = new ChaseAction(this);
	IBehaviorTreeAction* idleAction = new IdleAction(this);

	addChild("meleeEnemy", "onAttackHit", Sequence, falseCondition, nullptr);
	addChild("onAttackHit", "damageCalc", Action, nullptr, damageCalcAction);
	addChild("onAttackHit", "deathCheck", Priority, nullptr, nullptr);
	addChild("deathCheck", "onDeath", Action, deathCondition, onDeathAction);
	addChild("deathCheck", "attackProperties", Priority, nullptr, nullptr);
	addChild("attackProperties", "grabProperty", Sequence, grabCondition, nullptr);
	addChild("grabProperty", "onGrab", Action, nullptr, onGrabAction);
	addChild("grabProperty", "grabbed", Action, nullptr, grabbedAction);
	addChild("attackProperties", "propelProperty", Sequence, propelCondition, nullptr);
	addChild("propelProperty", "onPropel", Action, nullptr, onPropelAction);
	addChild("propelProperty", "propelled", Action, nullptr, propelledAction);
	addChild("attackProperties", "horizontalLaunchProperty", Sequence, horizontalLaunchCondition, nullptr);
	addChild("horizontalLaunchProperty", "onHorizontalLaunch", Action, nullptr, onHorizontalLaunchAction);
	addChild("horizontalLaunchProperty", "horizontalLaunched", Action, nullptr, horizontalLaunchedAction);
	addChild("horizontalLaunchProperty", "horizontalLaunchedFloat", Action, nullptr, floatingAction);
	addChild("attackProperties", "verticalLaunchProperty", Sequence, verticalLaunchCondition, nullptr);
	addChild("verticalLaunchProperty", "onVerticalLaunch", Action, nullptr, onVerticalLaunchAction);
	addChild("verticalLaunchProperty", "verticalLaunched", Action, nullptr, verticalLaunchedAction);
	addChild("verticalLaunchProperty", "verticalLaunchedFloat", Action, nullptr, floatingAction);
	addChild("attackProperties", "onStun", Action, onStunCondition, onStunAction);
	addChild("meleeEnemy", "onRespawn", Action, falseCondition, respawnAction);
	addChild("meleeEnemy", "dead", Action, deadCondition, deadAction);
	addChild("meleeEnemy", "stunned", Action, stunCondition, stunnedAction);
	addChild("meleeEnemy", "airborne", Action, airborneCondition, airborneAction);
	addChild("meleeEnemy", "returnToSpawn", Action, returnToSpawnCondition, returnToSpawnAction);
	addChild("meleeEnemy", "stepBack", Action, stepBackCondition, stepBackAction);

	CBehaviorTreeNodeRandom *node = (CBehaviorTreeNodeRandom*)addChild("meleeEnemy", "combat", Random, combatCondition, nullptr);
	addChild("combat", "idleWar", Sequence, nullptr, nullptr);
	addChild("idleWar", "onIdleWar", Action, nullptr, onIdleWarAction);
	addChild("idleWar", "idleWarAction", Action, nullptr, idleWarAction);
	addChild("combat", "attack", Sequence, nullptr, nullptr);
	addChild("attack", "onAttack", Action, nullptr, onAttackAction);
	addChild("attack", "attackAction", Action, nullptr, attackAction);
	node->setProbability(std::vector<float>{ 0.75f, 0.25f });

	addChild("meleeEnemy", "chase", Action, chaseCondition, chaseAction);

	addChild("meleeEnemy", "idle", Action, nullptr, idleAction);

	attackInfos["attack"].damage = attackDamage;
	attackInfos["attack"].stun = new AttackInfo::Stun{ 1.f };
	attackInfos["attack"].invulnerabilityTime = 1.f;

	maxHealth = j.value("maxHealth", maxHealth);
	health = maxHealth;
	movementSpeed = j.value("movementSpeed", movementSpeed);
	stepBackSpeed = j.value("stepBackSpeed", stepBackSpeed);
	rotationSpeed = j.value("rotationSpeed", rotationSpeed);
	recallDistanceSqrd = j.value("recallDistanceSqrd", recallDistanceSqrd);
	chaseFov = deg2rad(j.value("chaseFov", 60.f));
	fovChaseDistanceSqrd = j.value("fovChaseDistanceSqrd", 625.f);
	smallChaseRadiusSqrd = j.value("smallChaseRadiusSqrd", 100.f);
	attackFov = deg2rad(j.value("attackFov", 60.f));
	minCombatDistanceSqrd = j.value("minCombatDistanceSqrd", 1.f);
	maxCombatDistanceSqrd = j.value("maxCombatDistanceSqrd", 6.25f);
	attackCooldown = j.value("attackCooldown", 2.f);
	attackDamage = j.value("attackDamage", 1.f);
	propelDuration = j.value("propelDuration", propelDuration);
	gravity = j.value("gravity", -55.f);
	if (j.count("maxVelocity")) {
		maxVelocity = loadVEC3(j["maxVelocity"]);
	}
	if (j.count("attacks")) {
		for (int i = 0; i < j["attacks"].size(); ++i) {
			const json& jAttack = j["attacks"][i];
			std::string attackName = jAttack.value("name", "attack" + std::to_string(i));

			attackInfos[attackName].load(jAttack);

			if (jAttack.count("frameData")) {
				attacksFrameData[attackName] = loadVEC2(jAttack["frameData"]);
			}
		}
	}
}

void Dreidel::debugInMenu() {
}

void Dreidel::registerMsgs() {
	DECL_MSG(Dreidel, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(Dreidel, TMsgAttackHit, onAttackHit);
	DECL_MSG(Dreidel, TMsgRespawn, onRespawn);
	DECL_MSG(Dreidel, TMsgOutOfBounds, onOutOfBounds);
	DECL_MSG(Dreidel, TMsgPerfectDodged, onPerfectDodged);
	DECL_MSG(Dreidel, TMsgHitboxEnter, onHitboxEnter);
}

void Dreidel::update(float delta) {
	recalc(delta);
}

void Dreidel::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	spawnPosition = getTransform()->getPosition();
	playerHandle = getEntityByName(PLAYER_NAME);
}

void Dreidel::onAttackHit(const TMsgAttackHit& msg) {
	getSkeleton()->blendCycle(0);
	getSkeleton()->setTimeFactor(1);
	isStunned = false;
	receivedAttack = msg.info;
	current = tree["onAttackHit"];
}

void Dreidel::onRespawn(const TMsgRespawn& msg) {
	current = tree["onRespawn"];
}

void Dreidel::onOutOfBounds(const TMsgOutOfBounds& msg) {
	current = tree["onDeath"];
}

void Dreidel::onPerfectDodged(const TMsgPerfectDodged & msg) {
	dbg("Damn! I've been dodged.\n");
}

void Dreidel::updateGravity(float delta) {
	float deltaY = calculateVerticalDeltaMovement(delta, gravity, maxVelocity.y);
	physx::PxControllerCollisionFlags myFlags = EnginePhysics.move(getCollider()->controller, PxVec3(0, deltaY, 0), delta);
	grounded = myFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_DOWN);
	if (grounded) {
		velocityVector.y = 0;
	}
	else {
		velocityVector.y += gravity * delta;
	}
}

void Dreidel::onHitboxEnter(const TMsgHitboxEnter& msg) {
	if (attackInfos.find(msg.hitbox) != attackInfos.end()) {
		TMsgAttackHit attackHit = {};
		attackHit.attacker = CHandle(this).getOwner();
		attackHit.info = attackInfos[msg.hitbox];
		((CEntity*)msg.h_other_entity)->sendMsg(attackHit);
	}
}

float Dreidel::calculateVerticalDeltaMovement(float delta, float acceleration, float maxVelocityVertical) {
	float resultingDeltaMovement;
	resultingDeltaMovement = velocityVector.y * delta + 0.5f * acceleration * delta * delta;
	//clampear distancia vertical
	resultingDeltaMovement = resultingDeltaMovement > maxVelocityVertical * delta ? maxVelocityVertical * delta : resultingDeltaMovement;
	return resultingDeltaMovement;
}

void Dreidel::rotateTowards(float delta, VEC3 targetPos, float rotationSpeed) {
	float deltaYaw = getTransform()->getDeltaYawToAimTo(targetPos);
	float y, p, r;
	getTransform()->getYawPitchRoll(&y, &p, &r);
	float rotationIncrement = rotationSpeed * delta;
	if (abs(deltaYaw) >= rotationIncrement) {
		y += (deltaYaw > 0) ? (rotationIncrement) : (-rotationIncrement);
	}
	else {
		y += deltaYaw;
	}
	getTransform()->setYawPitchRoll(y, p, r);
}

TCompTransform* Dreidel::getTransform() {
	return get<TCompTransform>();
}

TCompCollider* Dreidel::getCollider() {
	return get<TCompCollider>();
}

TCompSkeleton* Dreidel::getSkeleton() {
	return get<TCompSkeleton>();
}

TCompHitboxes* Dreidel::getHitboxes() {
	return get<TCompHitboxes>();
}

CEntity* Dreidel::getPlayerEntity() {
	return playerHandle;
}

TCompTransform* Dreidel::getPlayerTransform() {
	return getPlayerEntity()->get<TCompTransform>();
}