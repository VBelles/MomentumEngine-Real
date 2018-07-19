#include "mcv_platform.h"
#include "MeleeEnemy.h"
#include "entity/common_msgs.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_render.h"
#include "components/comp_respawner.h"
#include "components/comp_give_power.h"
#include "components/comp_hitboxes.h"
#include "components/player/power_stats.h"
#include "skeleton/comp_skeleton.h"

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
	addChild("combat", "attack", Sequence, nullptr, nullptr);
	addChild("attack", "onAttack", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::onAttack);
	addChild("attack", "attackAction", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::attack);
	node->setProbability(std::vector<float>{ 0.5f, 0.5f });

	addChild("meleeEnemy", "chase", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::chaseCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::chase);

	addChild("meleeEnemy", "idle", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::trueCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::idle);
}

void CBehaviorTreeMeleeEnemy::load(const json& j, TEntityParseContext& ctx) {
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

void CBehaviorTreeMeleeEnemy::debugInMenu() {
}

void CBehaviorTreeMeleeEnemy::registerMsgs() {
	DECL_MSG(CBehaviorTreeMeleeEnemy, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(CBehaviorTreeMeleeEnemy, TMsgAttackHit, onAttackHit);
	DECL_MSG(CBehaviorTreeMeleeEnemy, TMsgRespawn, onRespawn);
	DECL_MSG(CBehaviorTreeMeleeEnemy, TMsgOutOfBounds, onOutOfBounds);
	DECL_MSG(CBehaviorTreeMeleeEnemy, TMsgPerfectDodged, onPerfectDodged);
	DECL_MSG(CBehaviorTreeMeleeEnemy, TMsgHitboxEnter, onHitboxEnter);
}

void CBehaviorTreeMeleeEnemy::update(float delta) {
	recalc(delta);
}

int CBehaviorTreeMeleeEnemy::damageCalc(float delta) {
	PROFILE_FUNCTION("damageCalc");
	health -= receivedAttack.damage;
	getRender()->TurnRed(0.5f);
	return Leave;
}

int CBehaviorTreeMeleeEnemy::onDeath(float delta) {
	PROFILE_FUNCTION("onDeath");
	health = 0.f;
	isDead = true;

	getCollider()->destroy();

	getRender()->disable();

	TCompRespawner* spawner = get<TCompRespawner>();
	spawner->onDead();

	EngineScripting.throwEvent(onEnemyKilled, ((CEntity*)CHandle(this).getOwner())->getName());

	return Leave;
}

int CBehaviorTreeMeleeEnemy::dead(float delta) {
	PROFILE_FUNCTION("dead");
	return Leave;
}

int CBehaviorTreeMeleeEnemy::onGrab(float delta) {
	PROFILE_FUNCTION("onGrab");
	//dbg("grabbed\n");
	getSkeleton()->setTimeFactor(0);
	getCollider()->destroy();
	timer.reset();
	grabbedDuration = receivedAttack.grab->duration;
	return Leave;
}

int CBehaviorTreeMeleeEnemy::grabbed(float delta) {
	PROFILE_FUNCTION("grabbed");
	if (timer.elapsed() >= grabbedDuration) {
		getCollider()->create();
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeMeleeEnemy::onPropel(float delta) {
	PROFILE_FUNCTION("onPropel");
	getCollider()->create();
	velocityVector = receivedAttack.propel->velocity;
	if (receivedAttack.propel->duration > 0.f) {
		propelDuration = receivedAttack.propel->duration;
	}
	else {
		propelDuration = defaultPropelDuration;
	}
	getSkeleton()->setTimeFactor(0);

	timer.reset();

	getRender()->TurnRed(0.5f);
	return Leave;
}

int CBehaviorTreeMeleeEnemy::propelled(float delta) {
	PROFILE_FUNCTION("propelled");
	if (timer.elapsed() < propelDuration) {
		VEC3 deltaMovement = velocityVector * delta;
		updateGravity(delta);
		EnginePhysics.move(getCollider()->controller, toPxVec3(deltaMovement), delta);
		return Stay;
	}
	else {
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
}

int CBehaviorTreeMeleeEnemy::onHorizontalLaunch(float delta) {
	PROFILE_FUNCTION("onHorizontalLaunch");
	getSkeleton()->setTimeFactor(0);
	floatingDuration = receivedAttack.horizontalLauncher->suspensionDuration;
	initialLaunchPos = getTransform()->getPosition();
	velocityVector = receivedAttack.horizontalLauncher->velocity;
	return Leave;
}

int CBehaviorTreeMeleeEnemy::horizontalLaunched(float delta) {
	PROFILE_FUNCTION("horizontalLaunched");
	updateGravity(delta);
	VEC3 deltaMovement = velocityVector * delta;
	EnginePhysics.move(getCollider()->controller, PxVec3(deltaMovement.x, 0, deltaMovement.z), delta);
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

int CBehaviorTreeMeleeEnemy::onVerticalLaunch(float delta) {
	PROFILE_FUNCTION("onVerticalLaunch");
	getSkeleton()->setTimeFactor(0);
	floatingDuration = receivedAttack.verticalLauncher->suspensionDuration;
	velocityVector = receivedAttack.verticalLauncher->velocity;
	return Leave;
}

int CBehaviorTreeMeleeEnemy::verticalLaunched(float delta) {
	PROFILE_FUNCTION("verticalLaunched");
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

int CBehaviorTreeMeleeEnemy::floating(float delta) {
	PROFILE_FUNCTION("floating");
	getSkeleton()->setTimeFactor(0);
	if (timer.elapsed() > floatingDuration) {
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeMeleeEnemy::onStun(float delta) {
	PROFILE_FUNCTION("onStun");
	getSkeleton()->setTimeFactor(0);
	stunDuration = receivedAttack.stun->duration;
	stunTimer.reset();
	isStunned = true;
	return Leave;
}

int CBehaviorTreeMeleeEnemy::stunned(float delta) {
	PROFILE_FUNCTION("stunned");
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

int CBehaviorTreeMeleeEnemy::airborne(float delta) {
	PROFILE_FUNCTION("airborne");
	getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	updateGravity(delta);
	return Leave;
}

int CBehaviorTreeMeleeEnemy::respawn(float delta) {
	PROFILE_FUNCTION("respawn");
	health = maxHealth;
	isDead = false;

	getCollider()->create();
	getTransform()->setPosition(spawnPosition);
	getCollider()->controller->setFootPosition(PxExtendedVec3(spawnPosition.x, spawnPosition.y, spawnPosition.z));

	getRender()->enable();

	TCompGivePower *power = get<TCompGivePower>();
	power->reset();

	getSkeleton()->blendCycle(0);
	getSkeleton()->setTimeFactor(1);

	return Leave;
}

int CBehaviorTreeMeleeEnemy::returnToSpawn(float delta) {
	PROFILE_FUNCTION("returnToSpawn");
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
	EnginePhysics.move(getCollider()->controller, toPxVec3(deltaMovement), delta);

	float distanceSquared = VEC3::DistanceSquared(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	if (VEC3::DistanceSquared(myPosition, targetPos) < minCombatDistanceSqrd
		|| (distanceSquared < fovChaseDistanceSqrd + minCombatDistanceSqrd && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov))) {
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeMeleeEnemy::chase(float delta) {
	PROFILE_FUNCTION("chase");
	getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	updateGravity(delta);

	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);

	//Move forward
	VEC3 myPosition = getTransform()->getPosition();
	VEC3 myFront = getTransform()->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = myFront * movementSpeed * delta;
	EnginePhysics.move(getCollider()->controller, toPxVec3(deltaMovement), delta);
	return Leave;
}

int CBehaviorTreeMeleeEnemy::stepBack(float delta) {
	PROFILE_FUNCTION("stepBack");
	getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	updateGravity(delta);

	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);

	VEC3 myPosition = getTransform()->getPosition();
	VEC3 myFront = getTransform()->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = -myFront * stepBackSpeed * delta;
	EnginePhysics.move(getCollider()->controller, toPxVec3(deltaMovement), delta);
	return Leave;
}

int CBehaviorTreeMeleeEnemy::onIdleWar(float delta) {
	PROFILE_FUNCTION("onIdleWar");
	getSkeleton()->blendCycle(0, 0.2f, 0.2f);
	updateGravity(delta);
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	idleWarTimer.reset();
	return Leave;
}

int CBehaviorTreeMeleeEnemy::idleWar(float delta) {
	PROFILE_FUNCTION("idleWar");
	updateGravity(delta);
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (idleWarTimer.elapsed() > getSkeleton()->getAnimationDuration(0)) {
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeMeleeEnemy::onAttack(float delta) {
	PROFILE_FUNCTION("onAttack");
	updateGravity(delta);
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (attackTimer.elapsed() < attackCooldown) {
		current = nullptr;
	}
	else {
		isFirstAttackLaunched = false;
		attackTimer.reset();
		getSkeleton()->executeAction(2, 0.2f, 0.2f);
	}
	return Leave;
}

int CBehaviorTreeMeleeEnemy::attack(float delta) {
	PROFILE_FUNCTION("attack");
	updateGravity(delta);
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (attackTimer.elapsed() < (getSkeleton()->getAnimationDuration(2))) {
		if (!isFirstAttackLaunched && attackTimer.elapsed() >= frames2sec(attacksFrameData["attack"].x)) {
			getHitboxes()->enable("attack");
			isFirstAttackLaunched = true;
		}
		else if (attackTimer.elapsed() >= frames2sec(attacksFrameData["attack"].x + attacksFrameData["attack"].y)) {
			getHitboxes()->disable("attack");
		}
		return Stay;
	}
	else {
		getHitboxes()->disable("attack");
		return Leave;
	}
}

int CBehaviorTreeMeleeEnemy::idle(float delta) {
	PROFILE_FUNCTION("idle");
	getSkeleton()->blendCycle(0, 0.2f, 0.2f);
	updateGravity(delta);
	return Leave;
}

bool CBehaviorTreeMeleeEnemy::deathCondition(float delta) {
	PROFILE_FUNCTION("deathCondition");
	return health <= 0;
}

bool CBehaviorTreeMeleeEnemy::deadCondition(float delta) {
	PROFILE_FUNCTION("deadCondition");
	return isDead;
}

bool CBehaviorTreeMeleeEnemy::grabCondition(float delta) {
	PROFILE_FUNCTION("grabCondition");
	return receivedAttack.grab;
}

bool CBehaviorTreeMeleeEnemy::propelCondition(float delta) {
	PROFILE_FUNCTION("propelCondition");
	return receivedAttack.propel;
}

bool CBehaviorTreeMeleeEnemy::horizontalLaunchCondition(float delta) {
	PROFILE_FUNCTION("horizontalLaunchCondition");
	return receivedAttack.horizontalLauncher;
}

bool CBehaviorTreeMeleeEnemy::verticalLaunchCondition(float delta) {
	PROFILE_FUNCTION("verticalLaunchCondition");
	return receivedAttack.verticalLauncher;
}

bool CBehaviorTreeMeleeEnemy::onStunCondition(float delta) {
	PROFILE_FUNCTION("onStunCondition");
	return receivedAttack.stun;
}

bool CBehaviorTreeMeleeEnemy::stunCondition(float delta) {
	PROFILE_FUNCTION("stunCondition");
	return isStunned;
}

bool CBehaviorTreeMeleeEnemy::airborneCondition(float delta) {
	PROFILE_FUNCTION("airborneCondition");
	return !grounded;
}

bool CBehaviorTreeMeleeEnemy::returnToSpawnCondition(float delta) {
	PROFILE_FUNCTION("returnToSpawnCondition");
	return VEC3::DistanceSquared(getTransform()->getPosition(), spawnPosition) > recallDistanceSqrd;
}

bool CBehaviorTreeMeleeEnemy::chaseCondition(float delta) {
	PROFILE_FUNCTION("chaseCondition");
	float distanceSqrd = VEC3::DistanceSquared(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	bool isPlayerInFov = getTransform()->isInFov(getPlayerTransform()->getPosition(), chaseFov);
	return distanceSqrd > minCombatDistanceSqrd && (distanceSqrd < smallChaseRadiusSqrd || (distanceSqrd < fovChaseDistanceSqrd && isPlayerInFov));
}

bool CBehaviorTreeMeleeEnemy::combatCondition(float delta) {
	PROFILE_FUNCTION("combatCondition");
	float distanceSqrd = VEC3::DistanceSquared(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	return distanceSqrd < maxCombatDistanceSqrd && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov);
}

bool CBehaviorTreeMeleeEnemy::stepBackCondition(float delta) {
	PROFILE_FUNCTION("stepBackCondition");
	VEC3 myPos = getTransform()->getPosition();
	VEC3 playerPos = getPlayerTransform()->getPosition();
	myPos.y = 0;
	playerPos.y = 0;
	//float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	float distanceSqrd = VEC3::DistanceSquared(myPos, playerPos);
	return distanceSqrd < minCombatDistanceSqrd;
}

void CBehaviorTreeMeleeEnemy::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	transformHandle = get<TCompTransform>();
	colliderHandle = get<TCompCollider>();
	renderHandle = get<TCompRender>();
	skeletonHandle = get<TCompSkeleton>();
	hitboxesHandle = get<TCompHitboxes>();
	spawnPosition = getTransform()->getPosition();
	playerHandle = getEntityByName(PLAYER_NAME);
}

void CBehaviorTreeMeleeEnemy::onAttackHit(const TMsgAttackHit& msg) {
	PROFILE_FUNCTION("onAttackHit");
	getSkeleton()->blendCycle(0);
	getSkeleton()->setTimeFactor(1);
	isStunned = false;
	getHitboxes()->disableAll();
	receivedAttack = msg.info;
	current = tree["onAttackHit"];
}

void CBehaviorTreeMeleeEnemy::onRespawn(const TMsgRespawn& msg) {
	PROFILE_FUNCTION("onRespawn");
	current = tree["onRespawn"];
}

void CBehaviorTreeMeleeEnemy::onOutOfBounds(const TMsgOutOfBounds& msg) {
	PROFILE_FUNCTION("onOutOfBounds");
	current = tree["onDeath"];
}

void CBehaviorTreeMeleeEnemy::onPerfectDodged(const TMsgPerfectDodged & msg) {
	PROFILE_FUNCTION("onPerfectDodged");
	dbg("Damn! I've been dodged.\n");
	//algo así podría estar guay
	//getSkeleton()->setTimeFactor(0.25f);
}

void CBehaviorTreeMeleeEnemy::onHitboxEnter(const TMsgHitboxEnter& msg) {
	PROFILE_FUNCTION("onHitboxEnter");
	if (attackInfos.find(msg.hitbox) != attackInfos.end()) {
		TMsgAttackHit attackHit = {};
		attackHit.attacker = CHandle(this).getOwner();
		attackHit.info = attackInfos[msg.hitbox];
		((CEntity*)msg.h_other_entity)->sendMsg(attackHit);
	}
}

void CBehaviorTreeMeleeEnemy::updateGravity(float delta) {
	PROFILE_FUNCTION("updateGravity");
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

float CBehaviorTreeMeleeEnemy::calculateVerticalDeltaMovement(float delta, float acceleration, float maxVelocityVertical) {
	PROFILE_FUNCTION("calculateVerticalDeltaMovement");
	float resultingDeltaMovement;
	resultingDeltaMovement = velocityVector.y * delta + 0.5f * acceleration * delta * delta;
	//clampear distancia vertical
	resultingDeltaMovement = resultingDeltaMovement > maxVelocityVertical * delta ? maxVelocityVertical * delta : resultingDeltaMovement;
	return resultingDeltaMovement;
}

void CBehaviorTreeMeleeEnemy::rotateTowards(float delta, VEC3 targetPos, float rotationSpeed) {
	PROFILE_FUNCTION("rotateTowards");
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
	return transformHandle;
}

TCompCollider* CBehaviorTreeMeleeEnemy::getCollider() {
	return colliderHandle;
}

void CBehaviorTreeMeleeEnemy::setSpawnPosition(VEC3 spawnPosition) {
	this->spawnPosition = spawnPosition;
}

TCompSkeleton* CBehaviorTreeMeleeEnemy::getSkeleton() {
	return skeletonHandle;
}

TCompRender* CBehaviorTreeMeleeEnemy::getRender() {
	return renderHandle;
}

TCompHitboxes* CBehaviorTreeMeleeEnemy::getHitboxes() {
	return hitboxesHandle;
}

CEntity* CBehaviorTreeMeleeEnemy::getPlayerEntity() {
	return playerHandle;
}

TCompTransform* CBehaviorTreeMeleeEnemy::getPlayerTransform() {
	return getPlayerEntity()->get<TCompTransform>();
}