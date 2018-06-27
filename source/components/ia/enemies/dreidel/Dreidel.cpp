#include "mcv_platform.h"
#include "Dreidel.h"

#include "components/ia/enemies/common/FalseCondition.h"
#include "components/ia/enemies/common/bloking_break/BlockingBreakAction.h"
#include "components/ia/enemies/common/bloking_break/OnBlockingBreakAction.h"
#include "components/ia/enemies/common/bloking_break/OnBlockingBreakCondition.h"
#include "components/ia/enemies/common/airborne/AirborneAction.h"
#include "components/ia/enemies/common/airborne/AirborneCondition.h"
#include "components/ia/enemies/common/airborne/OnAirborneAction.h"
#include "components/ia/enemies/common/airborne/OnAirborneCondition.h"
#include "components/ia/enemies/common/FalseCondition.h"
#include "components/ia/enemies/common/bloking_break/BlockingBreakAction.h"
#include "components/ia/enemies/common/bloking_break/OnBlockingBreakAction.h"
#include "components/ia/enemies/common/bloking_break/OnBlockingBreakCondition.h"
#include "components/ia/enemies/common/airborne/AirborneAction.h"
#include "components/ia/enemies/common/airborne/AirborneCondition.h"
#include "components/ia/enemies/common/airborne/OnAirborneAction.h"
#include "components/ia/enemies/common/airborne/OnAirborneCondition.h"
#include "components/ia/enemies/common/FalseCondition.h"
#include "components/ia/enemies/common/bloking_break/BlockingBreakAction.h"
#include "components/ia/enemies/common/bloking_break/OnBlockingBreakAction.h"
#include "components/ia/enemies/common/bloking_break/OnBlockingBreakCondition.h"
#include "components/ia/enemies/common/airborne/AirborneAction.h"
#include "components/ia/enemies/common/airborne/AirborneCondition.h"
#include "components/ia/enemies/common/airborne/OnAirborneAction.h"
#include "components/ia/enemies/common/airborne/OnAirborneCondition.h"
#include "components/ia/enemies/common/FalseCondition.h"
#include "components/ia/enemies/common/bloking_break/BlockingBreakAction.h"
#include "components/ia/enemies/common/bloking_break/OnBlockingBreakAction.h"
#include "components/ia/enemies/common/bloking_break/OnBlockingBreakCondition.h"
#include "components/ia/enemies/common/airborne/AirborneAction.h"
#include "components/ia/enemies/common/airborne/AirborneCondition.h"
#include "components/ia/enemies/common/airborne/OnAirborneAction.h"
#include "components/ia/enemies/common/airborne/OnAirborneCondition.h"
#include "components/ia/enemies/common/FalseCondition.h"
#include "components/ia/enemies/common/bloking_break/BlockingBreakAction.h"
#include "components/ia/enemies/common/bloking_break/OnBlockingBreakAction.h"
#include "components/ia/enemies/common/bloking_break/OnBlockingBreakCondition.h"
#include "components/ia/enemies/common/airborne/AirborneAction.h"
#include "components/ia/enemies/common/airborne/AirborneCondition.h"
#include "components/ia/enemies/common/airborne/OnAirborneAction.h"
#include "components/ia/enemies/common/airborne/OnAirborneCondition.h"
#include "components/ia/enemies/common/getting_hit/grab/GrabAction.h"
#include "components/ia/enemies/common/getting_hit/grab/OnGrabAction.h"
#include "components/ia/enemies/common/getting_hit/grab/OnGrabCondition.h"
#include "components/ia/enemies/common/getting_hit/hit/HitStun.h"
#include "components/ia/enemies/common/getting_hit/hit/OnHit.h"
#include "components/ia/enemies/common/FalseCondition.h"
#include "components/ia/enemies/common/bloking_break/BlockingBreakAction.h"
#include "components/ia/enemies/common/bloking_break/OnBlockingBreakAction.h"
#include "components/ia/enemies/common/bloking_break/OnBlockingBreakCondition.h"
#include "components/ia/enemies/common/airborne/AirborneAction.h"
#include "components/ia/enemies/common/airborne/AirborneCondition.h"
#include "components/ia/enemies/common/airborne/OnAirborneAction.h"
#include "components/ia/enemies/common/airborne/OnAirborneCondition.h"
#include "components/ia/enemies/common/getting_hit/grab/GrabAction.h"
#include "components/ia/enemies/common/getting_hit/grab/OnGrabAction.h"
#include "components/ia/enemies/common/getting_hit/grab/OnGrabCondition.h"
#include "components/ia/enemies/common/getting_hit/hit/HitStun.h"
#include "components/ia/enemies/common/getting_hit/hit/OnHit.h"

DECL_OBJ_MANAGER("dreidel", Dreidel);

void Dreidel::initBehaviorTree() {
	IBehaviorTreeCondition* falseCondition = new FalseCondition();
	AirborneAction* airborneAction = new AirborneAction(this);
	AirborneCondition* airborneCondition = new AirborneCondition(this);
	OnAirborneAction* onAirborneAction = new OnAirborneAction(this, AirborneAnimation);
	OnAirborneCondition* onAirborneCondition = new OnAirborneCondition(this);
	BlockingBreakAction* blockingBreakAction = new BlockingBreakAction(this, blockingBreakAnimation);
	OnBlockingBreakAction* nnBlockingBreakAction = new OnBlockingBreakAction(this, blockingBreakAnimation);
	OnBlockingBreakCondition* nnBlockingBreakCondition = new OnBlockingBreakCondition(this);
	DeathAction* deathAction = new DeathAction(this, deathAnimation);
	DisappearAction* disappearAction = new DisappearAction(this, DisapparAnimation);
	OnDeathAction* nnDeathAction = new OnDeathAction(this, DeathAnimation);
	OnDeathCondition* onDeathCondition = new OnDeathCondition(this);
	OnDisappearAction* onDisappearAction = new OnDisappearAction(this, DisapparAnimation);
	GrabAction* grabAction = new GrabAction(this);
	OnGrabAction* onGrabAction = new OnGrabAction(this);
	OnGrabCondition* onGrabCondition = new OnGrabCondition(this);
	HitStun* hitStun = new HitStun(this, HitAnimation);
	OnHit* onHit = new OnHit(this, HitAnimation);
	HorizontalLaunchedAction* horizontalLaunchedAction = new HorizontalLaunchedAction(this);
	OnHorizontalLaunchAction* onHorizontalLaunchAction = new OnHorizontalLaunchAction(this, HitAnimation);
	OnHorizontalLaunchCondition* onHorizontalLaunchCondition = new OnHorizontalLaunchCondition(this);
	OnVerticalLaunchAction* onVerticalLaunchAction = new OnVerticalLaunchAction(this, HitAnimation);
	OnVerticalLaunchCondition* onVerticalLaunchCondition = new OnVerticalLaunchCondition(this);
	VerticalLaunchedAction* verticalLaunchedAction = new VerticalLaunchedAction(this);
	OnPropelAction* onPropelAction = new OnPropelAction(this);
	OnPropelCondition * onPropelCondition = new OnPropelCondition(this);
	PropelAction* propelAction = new PropelAction(this);
	OnStunAction* onStunAction = new OnStunAction(this, stunAnimation);
	OnStunCondition* onStunCondition = new OnStunCondition(this);
	StunAction* stunAction = new StunAction(this);


	createRoot("meleeEnemy", Priority, nullptr, nullptr);

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
}

void Dreidel::load(const json& j, TEntityParseContext& ctx) {
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