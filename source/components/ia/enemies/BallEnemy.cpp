#include "mcv_platform.h"
#include "BallEnemy.h"
#include "entity/common_msgs.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_render.h"
#include "components/comp_respawner.h"
#include "components/comp_give_power.h"
#include "components/player/power_stats.h"
#include "skeleton/comp_skeleton.h"

DECL_OBJ_MANAGER("behaviorTree_ball_enemy", CBehaviorTreeBallEnemy);

CBehaviorTreeBallEnemy::CBehaviorTreeBallEnemy()
	: IBehaviorTree::IBehaviorTree() {
	createRoot("meleeEnemy", Priority, nullptr, nullptr);

	addChild("meleeEnemy", "onAttackHit", Sequence, &CBehaviorTreeBallEnemy::falseCondition, nullptr);
	addChild("onAttackHit", "damageCalc", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::damageCalc);
	addChild("onAttackHit", "deathCheck", Priority, nullptr, nullptr);
	addChild("deathCheck", "onDeath", Action, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::deathCondition, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::onDeath);
	addChild("deathCheck", "attackProperties", Priority, nullptr, nullptr);
	addChild("attackProperties", "grabProperty", Sequence, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::grabCondition, nullptr);
	addChild("grabProperty", "onGrab", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::onGrab);
	addChild("grabProperty", "grabbed", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::grabbed);
	addChild("attackProperties", "propelProperty", Sequence, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::propelCondition, nullptr);
	addChild("propelProperty", "onPropel", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::onPropel);
	addChild("propelProperty", "propelled", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::propelled);
	addChild("attackProperties", "horizontalLaunchProperty", Sequence, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::horizontalLaunchCondition, nullptr);
	addChild("horizontalLaunchProperty", "onHorizontalLaunch", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::onHorizontalLaunch);
	addChild("horizontalLaunchProperty", "horizontalLaunched", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::horizontalLaunched);
	addChild("horizontalLaunchProperty", "horizontalLaunchedFloat", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::floating);
	addChild("attackProperties", "verticalLaunchProperty", Sequence, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::verticalLaunchCondition, nullptr);
	addChild("verticalLaunchProperty", "onVerticalLaunch", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::onVerticalLaunch);
	addChild("verticalLaunchProperty", "verticalLaunched", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::verticalLaunched);
	addChild("verticalLaunchProperty", "verticalLaunchedFloat", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::floating);
	addChild("attackProperties", "onStun", Action, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::onStunCondition, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::onStun);

	addChild("meleeEnemy", "onRespawn", Action, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::falseCondition, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::respawn);

	addChild("meleeEnemy", "dead", Action, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::deadCondition, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::dead);

	addChild("meleeEnemy", "stunned", Action, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::stunCondition, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::stunned);

	addChild("meleeEnemy", "airborne", Action, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::airborneCondition, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::airborne);

	addChild("meleeEnemy", "returnToSpawn", Action, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::returnToSpawnCondition, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::returnToSpawn);

	addChild("meleeEnemy", "stepBack", Action, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::stepBackCondition, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::stepBack);

	CBehaviorTreeNodeRandom *node = (CBehaviorTreeNodeRandom*)addChild("meleeEnemy", "combat", Random, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::combatCondition, nullptr);
	addChild("combat", "idleWar", Sequence, nullptr, nullptr);
	addChild("idleWar", "onIdleWar", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::onIdleWar);
	addChild("idleWar", "idleWarAction", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::idleWar);
	addChild("combat", "attack", Sequence, nullptr, nullptr);
	addChild("attack", "onAttack", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::onAttack);
	addChild("attack", "attackAction", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::attack);
	node->setProbability(std::vector<float>{ 0.75f, 0.25f });

	addChild("meleeEnemy", "chase", Action, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::chaseCondition, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::chase);

	addChild("meleeEnemy", "idle", Action, (BehaviorTreeCondition)&CBehaviorTreeBallEnemy::trueCondition, (BehaviorTreeAction)&CBehaviorTreeBallEnemy::idle);
}

void CBehaviorTreeBallEnemy::load(const json& j, TEntityParseContext& ctx) {
	maxHealth = j.value("maxHealth", 5.0f);
	health = maxHealth;
	movementSpeed = j.value("movementSpeed", 2.5f);
	stepBackSpeed = j.value("stepBackSpeed", 3.5f);
	rotationSpeed = j.value("rotationSpeed", 20.f);
	recallDistanceSqrd = j.value("recallDistanceSqrd", 784.f);
	chaseFov = deg2rad(j.value("chaseFov", 60.f));
	fovChaseDistanceSqrd = j.value("fovChaseDistanceSqrd", 625.f);
	smallChaseRadiusSqrd = j.value("smallChaseRadiusSqrd", 100.f);
	attackFov = deg2rad(j.value("attackFov", 60.f));
	minCombatDistanceSqrd = j.value("minCombatDistanceSqrd", 1.f);
	maxCombatDistanceSqrd = j.value("maxCombatDistanceSqrd", 6.25f);
	attackCooldown = j.value("attackCooldown", 2.f);
	attackDamage = j.value("attackDamage", 1.f);
	propelDuration = j.value("propelDuration", 1.5f);
	gravity = j.value("gravity", -55.f);
	if (j.count("maxVelocity")) {
		maxVelocity = loadVEC3(j["maxVelocity"]);
	}
}

void CBehaviorTreeBallEnemy::debugInMenu() {
}

void CBehaviorTreeBallEnemy::registerMsgs() {
	DECL_MSG(CBehaviorTreeBallEnemy, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(CBehaviorTreeBallEnemy, TMsgAttackHit, onAttackHit);
	DECL_MSG(CBehaviorTreeBallEnemy, TMsgRespawn, onRespawn);
	DECL_MSG(CBehaviorTreeBallEnemy, TMsgOutOfBounds, onOutOfBounds);
}

void CBehaviorTreeBallEnemy::update(float delta) {
	recalc(delta);
}

int CBehaviorTreeBallEnemy::damageCalc(float delta) {
	health -= receivedAttack.damage;
	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
	return Leave;
}

int CBehaviorTreeBallEnemy::onDeath(float delta) {
	health = 0.f;
	isDead = true;

	getCollider()->destroy();

	TCompRender *render = get<TCompRender>();
	render->disable();

	TCompRespawner* spawner = get<TCompRespawner>();
	spawner->onDead();

    EngineScripting.throwEvent(onEnemyKilled, ((CEntity*)CHandle(this).getOwner())->getName());

	return Leave;
}

int CBehaviorTreeBallEnemy::dead(float delta) {
	return Leave;
}

int CBehaviorTreeBallEnemy::onGrab(float delta) {
	getSkeleton()->setTimeFactor(0);
	getCollider()->destroy();
	timer.reset();
	grabbedDuration = receivedAttack.grab->duration;
	return Leave;
}

int CBehaviorTreeBallEnemy::grabbed(float delta) {
	if (timer.elapsed() >= grabbedDuration) {
		getCollider()->create();
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeBallEnemy::onPropel(float delta) {
	getCollider()->create();
	velocityVector = receivedAttack.propel->velocity;

	getSkeleton()->setTimeFactor(0);

	timer.reset();

	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
	return Leave;
}

int CBehaviorTreeBallEnemy::propelled(float delta) {
	if (timer.elapsed() < propelDuration) {
		VEC3 deltaMovement = velocityVector * delta;
		getCollider()->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
		return Stay;
	}
	else {
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
}

int CBehaviorTreeBallEnemy::onHorizontalLaunch(float delta) {
	getSkeleton()->setTimeFactor(0);
	floatingDuration = receivedAttack.horizontalLauncher->suspensionDuration;
	initialLaunchPos = getTransform()->getPosition();
	velocityVector = receivedAttack.horizontalLauncher->velocity;
	return Leave;
}

int CBehaviorTreeBallEnemy::horizontalLaunched(float delta) {
	updateGravity(delta);
	VEC3 deltaMovement = velocityVector * delta;
	getCollider()->controller->move(physx::PxVec3(deltaMovement.x, 0, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	if (getTransform()->getPosition().y + deltaMovement.y - initialLaunchPos.y <= 0.001 || grounded) {
		velocityVector.x = 0;
		velocityVector.z = 0;
		timer.reset();
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeBallEnemy::onVerticalLaunch(float delta) {
	getSkeleton()->setTimeFactor(0);
	floatingDuration = receivedAttack.verticalLauncher->suspensionDuration;
	velocityVector = receivedAttack.verticalLauncher->velocity;
	return Leave;
}

int CBehaviorTreeBallEnemy::verticalLaunched(float delta) {
	updateGravity(delta);
	if (velocityVector.y <= 0) {
		timer.reset();
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeBallEnemy::floating(float delta) {
	getSkeleton()->setTimeFactor(0);
	if (timer.elapsed() > floatingDuration) {
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeBallEnemy::onStun(float delta) {
	getSkeleton()->setTimeFactor(0);
	stunDuration = receivedAttack.stun->duration;
	stunTimer.reset();
	isStunned = true;
	return Leave;
}

int CBehaviorTreeBallEnemy::stunned(float delta) {
	updateGravity(delta);
	if (stunTimer.elapsed() > stunDuration) {
		isStunned = false;
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeBallEnemy::airborne(float delta) {
	getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	updateGravity(delta);
	return Leave;
}

int CBehaviorTreeBallEnemy::respawn(float delta) {
	health = maxHealth;
	isDead = false;

	getCollider()->create();
	getTransform()->setPosition(spawnPosition);
	getCollider()->controller->setFootPosition(PxExtendedVec3(spawnPosition.x, spawnPosition.y, spawnPosition.z));

	TCompRender *render = get<TCompRender>();
	render->enable();

	TCompGivePower *power = get<TCompGivePower>();
	power->reset();

	getSkeleton()->blendCycle(0);
	getSkeleton()->setTimeFactor(1);

	return Leave;
}

int CBehaviorTreeBallEnemy::returnToSpawn(float delta) {
	getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	updateGravity(delta);

	rotateTowards(delta, spawnPosition, rotationSpeed);

	//Move forward
	VEC3 myPosition = getTransform()->getPosition();
	myPosition.y = 0;
	VEC3 targetPos = spawnPosition;
	targetPos.y = 0;
	VEC3 myFront = getTransform()->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = myFront * movementSpeed * delta;
	getCollider()->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());

	float distanceSquared = VEC3::DistanceSquared(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	if (VEC3::DistanceSquared(myPosition, targetPos) < minCombatDistanceSqrd
		|| (distanceSquared < fovChaseDistanceSqrd + minCombatDistanceSqrd && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov))) {
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeBallEnemy::chase(float delta) {
	getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	updateGravity(delta);

	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);

	//Move forward
	VEC3 myPosition = getTransform()->getPosition();
	VEC3 myFront = getTransform()->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = myFront * movementSpeed * delta;
	getCollider()->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	return Leave;
}

int CBehaviorTreeBallEnemy::stepBack(float delta) {
	getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	updateGravity(delta);

	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);

	VEC3 myPosition = getTransform()->getPosition();
	VEC3 myFront = getTransform()->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = -myFront * stepBackSpeed * delta;
	getCollider()->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	return Leave;
}

int CBehaviorTreeBallEnemy::onIdleWar(float delta) {
	getSkeleton()->blendCycle(0, 0.2f, 0.2f);
	updateGravity(delta);
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	idleWarTimer.reset();
	return Leave;
}

int CBehaviorTreeBallEnemy::idleWar(float delta) {
	updateGravity(delta);
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (idleWarTimer.elapsed() > getSkeleton()->getAnimationDuration(0)) {
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeBallEnemy::onAttack(float delta) {
	updateGravity(delta);
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (attackTimer.elapsed() < attackCooldown) {
		current = nullptr;
	}
	else {
		attackTimer.reset();
		getSkeleton()->executeAction(2, 0.2f, 0.2f);
	}
	return Leave;
}

int CBehaviorTreeBallEnemy::attack(float delta) {
	updateGravity(delta);
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (attackTimer.elapsed() < (getSkeleton()->getAnimationDuration(2))) {
		return Stay;
	}
	else {
		TMsgAttackHit msg = {};
		msg.attacker = CHandle(this);
		msg.info.damage = attackDamage;
		getPlayerEntity()->sendMsg(msg);
		attackTimer.reset();
		return Leave;
	}
}

int CBehaviorTreeBallEnemy::idle(float delta) {
	getSkeleton()->blendCycle(0, 0.2f, 0.2f);
	updateGravity(delta);
	return Leave;
}

bool CBehaviorTreeBallEnemy::deathCondition(float delta) {
	return health <= 0;
}

bool CBehaviorTreeBallEnemy::deadCondition(float delta) {
	return isDead;
}

bool CBehaviorTreeBallEnemy::grabCondition(float delta) {
	return receivedAttack.grab;
}

bool CBehaviorTreeBallEnemy::propelCondition(float delta) {
	return receivedAttack.propel;
}

bool CBehaviorTreeBallEnemy::horizontalLaunchCondition(float delta) {
	return receivedAttack.horizontalLauncher;
}

bool CBehaviorTreeBallEnemy::verticalLaunchCondition(float delta) {
	return receivedAttack.verticalLauncher;
}

bool CBehaviorTreeBallEnemy::onStunCondition(float delta) {
	return receivedAttack.stun;
}

bool CBehaviorTreeBallEnemy::stunCondition(float delta) {
	return isStunned;
}

bool CBehaviorTreeBallEnemy::airborneCondition(float delta) {
	return !grounded;
}

bool CBehaviorTreeBallEnemy::returnToSpawnCondition(float delta) {
	return VEC3::DistanceSquared(getTransform()->getPosition(), spawnPosition) > recallDistanceSqrd;
}

bool CBehaviorTreeBallEnemy::chaseCondition(float delta) {
	float distanceSqrd = VEC3::DistanceSquared(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	bool isPlayerInFov = getTransform()->isInFov(getPlayerTransform()->getPosition(), chaseFov);
	return distanceSqrd > minCombatDistanceSqrd && (distanceSqrd < smallChaseRadiusSqrd || (distanceSqrd < fovChaseDistanceSqrd && isPlayerInFov));
}

bool CBehaviorTreeBallEnemy::combatCondition(float delta) {
	float distanceSqrd = VEC3::DistanceSquared(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	return distanceSqrd < maxCombatDistanceSqrd && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov);
}

bool CBehaviorTreeBallEnemy::stepBackCondition(float delta) {
	VEC3 myPos = getTransform()->getPosition();
	VEC3 playerPos = getPlayerTransform()->getPosition();
	myPos.y = 0;
	playerPos.y = 0;
	//float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	float distanceSqrd = VEC3::DistanceSquared(myPos, playerPos);
	return distanceSqrd < minCombatDistanceSqrd;
}

void CBehaviorTreeBallEnemy::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	spawnPosition = getTransform()->getPosition();
	playerHandle = getEntityByName(PLAYER_NAME);
}

void CBehaviorTreeBallEnemy::onAttackHit(const TMsgAttackHit& msg) {
	getSkeleton()->blendCycle(0);
	getSkeleton()->setTimeFactor(1);
	isStunned = false;
	receivedAttack = msg.info;
	current = tree["onAttackHit"];
}

void CBehaviorTreeBallEnemy::onRespawn(const TMsgRespawn& msg) {
	current = tree["onRespawn"];
}

void CBehaviorTreeBallEnemy::onOutOfBounds(const TMsgOutOfBounds& msg) {
	current = tree["onDeath"];
}

void CBehaviorTreeBallEnemy::updateGravity(float delta) {
	float deltaY = calculateVerticalDeltaMovement(delta, gravity, maxVelocity.y);
	physx::PxControllerCollisionFlags myFlags = getCollider()->controller->move(physx::PxVec3(0, deltaY, 0), 0.f, delta, physx::PxControllerFilters());
	grounded = myFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_DOWN);
	if (grounded) {
		velocityVector.y = 0;
	}
	else {
		velocityVector.y += gravity * delta;
	}
}

float CBehaviorTreeBallEnemy::calculateVerticalDeltaMovement(float delta, float acceleration, float maxVelocityVertical) {
	float resultingDeltaMovement;
	resultingDeltaMovement = velocityVector.y * delta + 0.5f * acceleration * delta * delta;
	//clampear distancia vertical
	resultingDeltaMovement = resultingDeltaMovement > maxVelocityVertical * delta ? maxVelocityVertical * delta : resultingDeltaMovement;
	return resultingDeltaMovement;
}

void CBehaviorTreeBallEnemy::rotateTowards(float delta, VEC3 targetPos, float rotationSpeed) {
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

TCompTransform* CBehaviorTreeBallEnemy::getTransform() {
	return get<TCompTransform>();
}

TCompCollider* CBehaviorTreeBallEnemy::getCollider() {
	return get<TCompCollider>();
}

TCompSkeleton* CBehaviorTreeBallEnemy::getSkeleton() {
	return get<TCompSkeleton>();
}

CEntity* CBehaviorTreeBallEnemy::getPlayerEntity() {
	return playerHandle;
}

TCompTransform* CBehaviorTreeBallEnemy::getPlayerTransform() {
	return getPlayerEntity()->get<TCompTransform>();
}