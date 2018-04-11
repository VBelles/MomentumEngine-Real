#include "mcv_platform.h"
#include "MeleeEnemy.h"
#include "entity/common_msgs.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_render.h"
#include "components/comp_respawner.h"
#include "components/comp_shadow.h"
#include "components/comp_give_power.h"
#include "components/player/comp_player_model.h"

DECL_OBJ_MANAGER("behaviorTree_melee_enemy", CBehaviorTreeMeleeEnemy);

CBehaviorTreeMeleeEnemy::CBehaviorTreeMeleeEnemy()
	: IBehaviorTree::IBehaviorTree() {
	createRoot("meleeEnemy", Priority, nullptr, nullptr);

	addChild("meleeEnemy", "onAttackHit", Sequence, &CBehaviorTreeMeleeEnemy::falseCondition, nullptr);
	addChild("onAttackHit", "damageCalc", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::damageCalc);
	addChild("onAttackHit", "deathCheck", Priority, nullptr, nullptr);
	addChild("deathCheck", "onDeath", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::deathCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::onDeath);
	addChild("deathCheck", "attackProperties", Priority, nullptr, nullptr);
	addChild("attackProperties", "grabProperty", Sequence, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::grabCondition, nullptr);
	addChild("grabProperty", "onGrab", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::onGrab);
	addChild("grabProperty", "grabbed", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::grabbed);
	addChild("attackProperties", "propelProperty", Sequence, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::propelCondition, nullptr);
	addChild("propelProperty", "onPropel", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::onPropel);
	addChild("propelProperty", "propelled", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::propelled);
	addChild("attackProperties", "horizontalLaunchProperty", Sequence, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::horizontalLaunchCondition, nullptr);
	addChild("horizontalLaunchProperty", "onHorizontalLaunch", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::onHorizontalLaunch);
	addChild("horizontalLaunchProperty", "horizontalLaunched", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::horizontalLaunched);
	addChild("horizontalLaunchProperty", "horizontalLaunchedFloat", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::floating);
	addChild("attackProperties", "verticalLaunchProperty", Sequence, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::verticalLaunchCondition, nullptr);
	addChild("verticalLaunchProperty", "onVerticalLaunch", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::onVerticalLaunch);
	addChild("verticalLaunchProperty", "verticalLaunched", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::verticalLaunched);
	addChild("verticalLaunchProperty", "verticalLaunchedFloat", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::floating);
	addChild("attackProperties", "onStun", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::onStunCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::onStun);

	addChild("meleeEnemy", "onRespawn", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::falseCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::respawn);

	addChild("meleeEnemy", "dead", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::deadCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::dead);

	addChild("meleeEnemy", "stunned", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::stunCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::stunned);

	addChild("meleeEnemy", "airborne", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::airborneCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::airborne);

	addChild("meleeEnemy", "returnToSpawn", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::returnToSpawnCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::returnToSpawn);

	addChild("meleeEnemy", "stepBack", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::stepBackCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::stepBack);

	CBehaviorTreeNodeRandom *node = (CBehaviorTreeNodeRandom*)addChild("meleeEnemy", "combat", Random, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::combatCondition, nullptr);
	addChild("combat", "idleWar", Sequence, nullptr, nullptr);
	addChild("idleWar", "onIdleWar", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::onIdleWar);
	addChild("idleWar", "idleWarAction", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::idleWar);
	addChild("combat", "attack", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::attack);
	node->setProbability(std::vector<float>{ 0.75f, 0.25f });

	addChild("meleeEnemy", "chase", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::chaseCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::chase);

	addChild("meleeEnemy", "idle", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::trueCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::idle);
}

void CBehaviorTreeMeleeEnemy::load(const json& j, TEntityParseContext& ctx) {
	maxHealth = j.value("maxHealth", 5.0f);
	health = maxHealth;
	movementSpeed = j.value("movementSpeed", 2.5f);
	stepBackSpeed = j.value("stepBackSpeed", 3.5f);
	rotationSpeed = j.value("rotationSpeed", 20.f);
	recallDistance = j.value("recallDistance", 28.f);
	chaseFov = deg2rad(j.value("chaseFov", 60.f));
	fovChaseDistance = j.value("fovChaseDistance", 25.f);
	smallChaseRadius = j.value("smallChaseRadius", 10.f);
	attackFov = deg2rad(j.value("attackFov", 60.f));
	minCombatDistance = j.value("minCombatDistance", 1.f);
	maxCombatDistance = j.value("maxCombatDistance", 2.5f);
	attackCooldown = j.value("attackCooldown", 2.f);
	attackDamage = j.value("attackDamage", 1.f);
	propelDuration = j.value("propelDuration", 1.5f);
	gravity = j.value("gravity", -50.f);
	if (j.count("maxVelocity")) {
		maxVelocity = loadVEC3(j["maxVelocity"]);
	}
}

void CBehaviorTreeMeleeEnemy::debugInMenu() {
}

void CBehaviorTreeMeleeEnemy::registerMsgs() {
	DECL_MSG(CBehaviorTreeMeleeEnemy, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(CBehaviorTreeMeleeEnemy, TMsgAttackHit, onAttackHit);
	DECL_MSG(CBehaviorTreeMeleeEnemy, TMsgRespawn, onRespawn);
	DECL_MSG(CBehaviorTreeMeleeEnemy, TMsgOutOfBounds, onOutOfBounds);
}

void CBehaviorTreeMeleeEnemy::update(float delta) {
	recalc(delta);
}

int CBehaviorTreeMeleeEnemy::damageCalc(float delta) {
	health -= receivedAttack.damage;
	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
	return Leave;
}

int CBehaviorTreeMeleeEnemy::onDeath(float delta) {
	health = 0.f;
	isDead = true;

	getCollider()->destroy();

	TCompRender *render = get<TCompRender>();
	render->disable();

	TCompShadow* shadow = get<TCompShadow>();
	shadow->disable();

	TCompRespawner* spawner = get<TCompRespawner>();
	spawner->onDead();

	return Leave;
}

int CBehaviorTreeMeleeEnemy::dead(float delta) {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::onGrab(float delta) {
	getCollider()->destroy();
	timer.reset();
	grabbedDuration = receivedAttack.grab->duration;
	return Leave;
}

int CBehaviorTreeMeleeEnemy::grabbed(float delta) {
	if (timer.elapsed() >= grabbedDuration) {
		getCollider()->create();
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeMeleeEnemy::onPropel(float delta) {
	getCollider()->create();
	velocityVector = receivedAttack.propel->velocity;

	timer.reset();

	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
	return Leave;
}

int CBehaviorTreeMeleeEnemy::propelled(float delta) {
	if (timer.elapsed() < propelDuration) {
		VEC3 deltaMovement = velocityVector * delta;
		getCollider()->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
		return Stay;
	}
	else {
		return Leave;
	}
}

int CBehaviorTreeMeleeEnemy::onHorizontalLaunch(float delta) {
	floatingDuration = receivedAttack.horizontalLauncher->suspensionDuration;
	initialLaunchPos = getTransform()->getPosition();
	velocityVector = receivedAttack.horizontalLauncher->velocity;
	return Leave;
}

int CBehaviorTreeMeleeEnemy::horizontalLaunched(float delta) {
	updateGravity(delta);
	VEC3 deltaMovement = velocityVector * delta;
	getCollider()->controller->move(physx::PxVec3(deltaMovement.x, 0, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	if (getTransform()->getPosition().y + deltaMovement.y - initialLaunchPos.y <= 0.001 || grounded) {
		velocityVector.x = 0;
		velocityVector.z = 0;
		timer.reset();
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeMeleeEnemy::onVerticalLaunch(float delta) {
	floatingDuration = receivedAttack.verticalLauncher->suspensionDuration;
	velocityVector = receivedAttack.verticalLauncher->velocity;
	return Leave;
}

int CBehaviorTreeMeleeEnemy::verticalLaunched(float delta) {
	updateGravity(delta);
	if (velocityVector.y <= 0) {
		timer.reset();
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeMeleeEnemy::floating(float delta) {
	if (timer.elapsed() > floatingDuration) {
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeMeleeEnemy::onStun(float delta) {
	stunDuration = receivedAttack.stun->duration;
	stunTimer.reset();
	isStunned = true;
	return Leave;
}

int CBehaviorTreeMeleeEnemy::stunned(float delta) {
	updateGravity(delta);
	if (stunTimer.elapsed() > stunDuration) {
		isStunned = false;
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeMeleeEnemy::airborne(float delta) {
	updateGravity(delta);
	return Leave;
}

int CBehaviorTreeMeleeEnemy::respawn(float delta) {
	health = maxHealth;
	isDead = false;

	getCollider()->create();
	getTransform()->setPosition(spawnPosition);
	getCollider()->controller->setFootPosition(PxExtendedVec3(spawnPosition.x, spawnPosition.y, spawnPosition.z));

	TCompShadow* shadow = get<TCompShadow>();
	shadow->enable();

	TCompRender *render = get<TCompRender>();
	render->enable();

	TCompGivePower *power = get<TCompGivePower>();
	power->reset();

	return Leave;
}

int CBehaviorTreeMeleeEnemy::returnToSpawn(float delta) {
	updateGravity(delta);

	rotateTowards(delta, spawnPosition, rotationSpeed);

	//Move forward
	VEC3 myPosition = getTransform()->getPosition();
	myPosition.y = 0;
	VEC3 targetPos = spawnPosition;
	spawnPosition.y = 0;
	VEC3 myFront = getTransform()->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = myFront * movementSpeed * delta;
	getCollider()->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());

	float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	if (VEC3::Distance(myPosition, spawnPosition) < minCombatDistance
		|| (distance < fovChaseDistance + maxCombatDistance && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov)))
	{
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeMeleeEnemy::chase(float delta) {
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

int CBehaviorTreeMeleeEnemy::stepBack(float delta) {
	updateGravity(delta);

	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);

	VEC3 myPosition = getTransform()->getPosition();
	VEC3 myFront = getTransform()->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = -myFront * stepBackSpeed * delta;
	getCollider()->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	return Leave;
}

int CBehaviorTreeMeleeEnemy::onIdleWar(float delta) {
	idleWarTimer.reset();
	return Leave;
}

int CBehaviorTreeMeleeEnemy::idleWar(float delta) {
	updateGravity(delta);
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (idleWarTimer.elapsed() > 1.f) {
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeMeleeEnemy::attack(float delta) {
	updateGravity(delta);
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (attackTimer.elapsed() > attackCooldown) {
		attackTimer.reset();
		TMsgAttackHit msg = {};
		msg.attacker = CHandle(this);
		msg.info.damage = attackDamage;
		getPlayerEntity()->sendMsg(msg);
	}
	return Leave;
}

int CBehaviorTreeMeleeEnemy::idle(float delta) {
	updateGravity(delta);
	return Leave;
}

bool CBehaviorTreeMeleeEnemy::deathCondition(float delta) {
	return health <= 0;
}

bool CBehaviorTreeMeleeEnemy::deadCondition(float delta) {
	return isDead;
}

bool CBehaviorTreeMeleeEnemy::grabCondition(float delta) {
	return receivedAttack.grab;
}

bool CBehaviorTreeMeleeEnemy::propelCondition(float delta) {
	return receivedAttack.propel;
}

bool CBehaviorTreeMeleeEnemy::horizontalLaunchCondition(float delta) {
	return receivedAttack.horizontalLauncher;
}

bool CBehaviorTreeMeleeEnemy::verticalLaunchCondition(float delta) {
	return receivedAttack.verticalLauncher;
}

bool CBehaviorTreeMeleeEnemy::onStunCondition(float delta) {
	return receivedAttack.stun;
}

bool CBehaviorTreeMeleeEnemy::stunCondition(float delta) {
	return isStunned;
}

bool CBehaviorTreeMeleeEnemy::airborneCondition(float delta) {
	return !grounded;
}

bool CBehaviorTreeMeleeEnemy::returnToSpawnCondition(float delta) {
	return VEC3::Distance(getTransform()->getPosition(), spawnPosition) > recallDistance;
}

bool CBehaviorTreeMeleeEnemy::chaseCondition(float delta) {
	float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	bool isPlayerInFov = getTransform()->isInFov(getPlayerTransform()->getPosition(), chaseFov);
	return distance > minCombatDistance && (distance < smallChaseRadius || (distance < fovChaseDistance && isPlayerInFov));
}

bool CBehaviorTreeMeleeEnemy::combatCondition(float delta) {
	float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	return distance < maxCombatDistance && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov);
}

bool CBehaviorTreeMeleeEnemy::stepBackCondition(float delta) {
	VEC3 myPos = getTransform()->getPosition();
	VEC3 playerPos = getPlayerTransform()->getPosition();
	myPos.y = 0;
	playerPos.y = 0;
	//float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	float distance = VEC3::Distance(myPos, playerPos);
	return distance < minCombatDistance;
}

void CBehaviorTreeMeleeEnemy::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	spawnPosition = getTransform()->getPosition();
	playerHandle = getEntityByName("The Player");
}

void CBehaviorTreeMeleeEnemy::onAttackHit(const TMsgAttackHit& msg) {
	isStunned = false;
	receivedAttack = msg.info;
	current = tree["onAttackHit"];
}

void CBehaviorTreeMeleeEnemy::onRespawn(const TMsgRespawn& msg) {
	current = tree["onRespawn"];
}

void CBehaviorTreeMeleeEnemy::onOutOfBounds(const TMsgOutOfBounds& msg) {
	current = tree["onDeath"];
}

void CBehaviorTreeMeleeEnemy::updateGravity(float delta) {
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

float CBehaviorTreeMeleeEnemy::calculateVerticalDeltaMovement(float delta, float acceleration, float maxVelocityVertical) {
	float resultingDeltaMovement;
	resultingDeltaMovement = velocityVector.y * delta + 0.5f * acceleration * delta * delta;
	//clampear distancia vertical
	resultingDeltaMovement = resultingDeltaMovement > maxVelocityVertical * delta ? maxVelocityVertical * delta : resultingDeltaMovement;
	return resultingDeltaMovement;
}

void CBehaviorTreeMeleeEnemy::rotateTowards(float delta, VEC3 targetPos, float rotationSpeed) {
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

TCompTransform* CBehaviorTreeMeleeEnemy::getTransform() {
	return get<TCompTransform>();
}

TCompCollider* CBehaviorTreeMeleeEnemy::getCollider() {
	return get<TCompCollider>();
}

CEntity* CBehaviorTreeMeleeEnemy::getPlayerEntity() {
	return playerHandle;
}

TCompTransform* CBehaviorTreeMeleeEnemy::getPlayerTransform() {
	return getPlayerEntity()->get<TCompTransform>();
}