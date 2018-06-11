#include "mcv_platform.h"
#include "FlyingRangedEnemy.h"
#include "entity/common_msgs.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_render.h"
#include "components/comp_respawner.h"
#include "components/comp_give_power.h"
#include "components/comp_hitboxes.h"
#include "components/player/power_stats.h"
#include "entity/entity_parser.h"
#include "skeleton/comp_skeleton.h"

DECL_OBJ_MANAGER("behaviorTree_flying_ranged_enemy", CBehaviorTreeFlyingRangedEnemy);

CBehaviorTreeFlyingRangedEnemy::CBehaviorTreeFlyingRangedEnemy()
	: IBehaviorTree::IBehaviorTree() {
	createRoot("flyingRangedEnemy", Priority, nullptr, nullptr);

	addChild("flyingRangedEnemy", "onAttackHit", Sequence, &CBehaviorTreeFlyingRangedEnemy::falseCondition, nullptr);
	addChild("onAttackHit", "damageCalc", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::damageCalc);
	addChild("onAttackHit", "deathCheck", Priority, nullptr, nullptr);
	addChild("deathCheck", "onDeath", Action, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::deathCondition, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::onDeath);
	addChild("deathCheck", "attackProperties", Priority, nullptr, nullptr);
	addChild("attackProperties", "grabProperty", Sequence, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::grabCondition, nullptr);
	addChild("grabProperty", "onGrab", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::onGrab);
	addChild("grabProperty", "grabbed", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::grabbed);
	addChild("attackProperties", "propelProperty", Sequence, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::propelCondition, nullptr);
	addChild("propelProperty", "onPropel", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::onPropel);
	addChild("propelProperty", "propelled", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::propelled);
	addChild("attackProperties", "horizontalLaunchProperty", Sequence, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::horizontalLaunchCondition, nullptr);
	addChild("horizontalLaunchProperty", "onHorizontalLaunch", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::onHorizontalLaunch);
	addChild("horizontalLaunchProperty", "horizontalLaunched", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::horizontalLaunched);
	addChild("horizontalLaunchProperty", "horizontalLaunchedFloat", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::floating);
	addChild("attackProperties", "verticalLaunchProperty", Sequence, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::verticalLaunchCondition, nullptr);
	addChild("verticalLaunchProperty", "onVerticalLaunch", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::onVerticalLaunch);
	addChild("verticalLaunchProperty", "verticalLaunched", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::verticalLaunched);
	addChild("verticalLaunchProperty", "verticalLaunchedFloat", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::floating);
	addChild("attackProperties", "onStun", Action, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::onStunCondition, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::onStun);

	addChild("flyingRangedEnemy", "onRespawn", Action, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::falseCondition, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::respawn);

	addChild("flyingRangedEnemy", "dead", Action, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::deadCondition, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::dead);

	addChild("flyingRangedEnemy", "stunned", Action, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::stunCondition, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::stunned);

	addChild("flyingRangedEnemy", "melee", Sequence, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::meleeAttackCondition, nullptr);
	addChild("melee", "onMeleeAttack", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::onMeleeAttack);
	addChild("melee", "meleeAttack", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::meleeAttack);

	addChild("flyingRangedEnemy", "returnToSpawn", Action, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::returnToSpawnCondition, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::returnToSpawn);

	CBehaviorTreeNodeRandom *node = (CBehaviorTreeNodeRandom*)addChild("flyingRangedEnemy", "combat", Random, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::combatCondition, nullptr);
	addChild("combat", "idleWar", Sequence, nullptr, nullptr);
	addChild("idleWar", "onIdleWar", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::onIdleWar);
	addChild("idleWar", "idleWarAction", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::idleWar);
	addChild("combat", "rangedAttack", Sequence, nullptr, nullptr);
	addChild("rangedAttack", "onRangedAttack", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::onRangedAttack);
	addChild("rangedAttack", "rangedAttackAction", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::rangedAttack);
	node->setProbability(std::vector<float>{ 0.75f, 0.25f });

	addChild("flyingRangedEnemy", "patrol", Action, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::patrolCondition, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::patrol);

	addChild("flyingRangedEnemy", "idle", Action, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::trueCondition, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::idle);
}

void CBehaviorTreeFlyingRangedEnemy::load(const json& j, TEntityParseContext& ctx) {
	maxHealth = j.value("maxHealth", 5.0f);
	health = maxHealth;
	movementSpeed = j.value("movementSpeed", 2.5f);
	rotationSpeed = j.value("rotationSpeed", 20.f);
	recallDistanceSqrd = j.value("recallDistanceSqrd", 25.f);
	attackFov = deg2rad(j.value("attackFov", 60.f));
	minCombatDistanceSqrd = j.value("minCombatDistanceSqrd", 4.f);
	maxCombatDistanceSqrd = j.value("maxCombatDistanceSqrd", 400.f);
	attackCooldown = j.value("attackCooldown", 2.f);
	attackDamage = j.value("attackDamage", 1.f);
	propelDuration = j.value("propelDuration", 1.5f);
	gravity = j.value("gravity", -55.f);
	if (j.count("attackPrefab")) {
		attackPrefab = j["attackPrefab"].get<std::string>();
	}
	if (j.count("attackSpawnOffset")) {
		attackSpawnOffset = loadVEC3(j["attackSpawnOffset"]);
	}
	if (j.count("attackTargetOffset")) {
		attackTargetOffset = loadVEC3(j["attackTargetOffset"]);
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
	wayPoints.clear();
	if (j.count("wayPoints")) {
		currentWaypoint = 0;
		for (int i = 0; i < j["wayPoints"].size(); ++i) {
			const json& jWayPoint = j["wayPoints"][i];
			wayPoints.push_back(loadVEC3(jWayPoint));
		}
	}
}

void CBehaviorTreeFlyingRangedEnemy::debugInMenu() {
}

void CBehaviorTreeFlyingRangedEnemy::registerMsgs() {
	DECL_MSG(CBehaviorTreeFlyingRangedEnemy, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(CBehaviorTreeFlyingRangedEnemy, TMsgAttackHit, onAttackHit);
	DECL_MSG(CBehaviorTreeFlyingRangedEnemy, TMsgRespawn, onRespawn);
	DECL_MSG(CBehaviorTreeFlyingRangedEnemy, TMsgOutOfBounds, onOutOfBounds);
	DECL_MSG(CBehaviorTreeFlyingRangedEnemy, TMsgPerfectDodged, onPerfectDodged);
	DECL_MSG(CBehaviorTreeFlyingRangedEnemy, TMsgHitboxEnter, onHitboxEnter);
}

void CBehaviorTreeFlyingRangedEnemy::update(float delta) {
	recalc(delta);
}

int CBehaviorTreeFlyingRangedEnemy::damageCalc(float delta) {
	health -= receivedAttack.damage;
	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::onDeath(float delta) {
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

int CBehaviorTreeFlyingRangedEnemy::dead(float delta) {
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::onGrab(float delta) {
	getSkeleton()->setTimeFactor(0);
	getCollider()->destroy();
	timer.reset();
	grabbedDuration = receivedAttack.grab->duration;
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::grabbed(float delta) {
	if (timer.elapsed() >= grabbedDuration) {
		getCollider()->create();
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		getSkeleton()->setTimeFactor(1);
		return Stay;
	}
}

int CBehaviorTreeFlyingRangedEnemy::onPropel(float delta) {
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

	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::propelled(float delta) {
	if (timer.elapsed() < propelDuration) {
		VEC3 deltaMovement = velocityVector * delta;
		EnginePhysics.move(getCollider()->controller, toPxVec3(deltaMovement), delta);
		return Stay;
	}
	else {
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
}

int CBehaviorTreeFlyingRangedEnemy::onHorizontalLaunch(float delta) {
	getSkeleton()->setTimeFactor(0);
	floatingDuration = receivedAttack.horizontalLauncher->suspensionDuration;
	initialLaunchPos = getTransform()->getPosition();
	velocityVector = receivedAttack.horizontalLauncher->velocity;
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::horizontalLaunched(float delta) {
	VEC3 deltaMovement = velocityVector * delta;
	EnginePhysics.move(getCollider()->controller, toPxVec3(deltaMovement), delta);
	velocityVector.y += gravity * delta;
	if (getTransform()->getPosition().y + deltaMovement.y - initialLaunchPos.y <= 0.001) {
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

int CBehaviorTreeFlyingRangedEnemy::onVerticalLaunch(float delta) {
	getSkeleton()->setTimeFactor(0);
	floatingDuration = receivedAttack.verticalLauncher->suspensionDuration;
	velocityVector = receivedAttack.verticalLauncher->velocity;
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::verticalLaunched(float delta) {
	VEC3 deltaMovement = velocityVector * delta;
	EnginePhysics.move(getCollider()->controller, toPxVec3(deltaMovement), delta);
	velocityVector.y += gravity * delta;
	if (velocityVector.y <= 0) {
		timer.reset();
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeFlyingRangedEnemy::floating(float delta) {
	getSkeleton()->setTimeFactor(0);
	if (timer.elapsed() > floatingDuration) {
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeFlyingRangedEnemy::onStun(float delta) {
	getSkeleton()->setTimeFactor(0);
	stunDuration = receivedAttack.stun->duration;
	stunTimer.reset();
	isStunned = true;
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::stunned(float delta) {
	if (stunTimer.elapsed() > stunDuration) {
		isStunned = false;
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeFlyingRangedEnemy::respawn(float delta) {
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

int CBehaviorTreeFlyingRangedEnemy::returnToSpawn(float delta) {
	getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	rotateTowards(delta, spawnPosition, rotationSpeed);

	VEC3 myPosition = getTransform()->getPosition();
	VEC3 deltaMovement = spawnPosition - myPosition;
	deltaMovement.Normalize();
	deltaMovement = deltaMovement * movementSpeed * delta;

	EnginePhysics.move(getCollider()->controller, toPxVec3(deltaMovement), delta);

	float distance = VEC3::DistanceSquared(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	if (VEC3::DistanceSquared(myPosition + deltaMovement, spawnPosition) < minCombatDistanceSqrd
		|| (distance < maxCombatDistanceSqrd + maxCombatDistanceSqrd && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov))) {
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeFlyingRangedEnemy::onIdleWar(float delta) {
	getSkeleton()->blendCycle(0, 0.2f, 0.2f);
	idleWarTimer.reset();
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::idleWar(float delta) {
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (idleWarTimer.elapsed() > getSkeleton()->getAnimationDuration(0)) {
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeFlyingRangedEnemy::onMeleeAttack(float delta) {
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (attackTimer.elapsed() < attackCooldown) {
		current = nullptr;
	}
	else {
		hasAttacked = false;
		attackTimer.reset();
		getSkeleton()->executeAction(1, 0.2f, 0.2f);
	}
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::meleeAttack(float delta) {
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (attackTimer.elapsed() < (getSkeleton()->getAnimationDuration(1))) {
		if (!hasAttacked && attackTimer.elapsed() >= frames2sec(attacksFrameData["attack"].x)) {
			getHitboxes()->enable("attack");
			hasAttacked = true;
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

int CBehaviorTreeFlyingRangedEnemy::onRangedAttack(float delta) {
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (attackTimer.elapsed() < attackCooldown) {
		current = nullptr;
	}
	else {
		hasAttacked = false;
		attackTimer.reset();
		getSkeleton()->executeAction(1, 0.2f, 0.2f);
	}
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::rangedAttack(float delta) {
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (attackTimer.elapsed() < (getSkeleton()->getAnimationDuration(1))) {
		if (!hasAttacked && (attackTimer.elapsed() >= (getSkeleton()->getAnimationDuration(1) * (180.f / 280.f)))) {
			hasAttacked = true;
			TEntityParseContext ctx;
			ctx.root_transform.setPosition(getTransform()->getPosition());
			if (parseScene(attackPrefab, ctx)) {
				assert(!ctx.entities_loaded.empty());

				AttackInfo attackInfo;
				attackInfo.damage = attackDamage;

				//attackInfo.stun = new AttackInfo::Stun{ 1.f };
				attackInfo.invulnerabilityTime = 1.2f;

				VEC3 front = getTransform()->getFront();
				VEC3 right = -getTransform()->getLeft();
				front.y = 0.f;
				right.y = 0.f;
				front.Normalize();
				right.Normalize();
				VEC3 desiredOffset = front * attackSpawnOffset.z + right * attackSpawnOffset.x;
				desiredOffset.y = attackSpawnOffset.y;
				VEC3 attackInitialPos = getTransform()->getPosition() + desiredOffset;

				VEC3 attackDirection = getPlayerTransform()->getPosition() - attackInitialPos;
				attackDirection += attackTargetOffset;
				attackDirection.Normalize();

				TMsgAssignRangedAttackOwner msg{ CHandle(this), attackInfo, attackInitialPos, attackDirection };

				CEntity *attackEntity = ctx.entities_loaded[0];
				attackEntity->sendMsg(msg);
			}
		}
		return Stay;
	}
	else {
		return Leave;
	}
}

int CBehaviorTreeFlyingRangedEnemy::patrol(float delta) {
	VEC3 myPosition = getTransform()->getPosition();
	if (VEC3::DistanceSquared(myPosition, wayPoints[currentWaypoint]) < 0.2f) {
		currentWaypoint = (currentWaypoint + 1) % wayPoints.size();
	}

	getSkeleton()->blendCycle(0, 0.2f, 0.2f);
	rotateTowards(delta, wayPoints[currentWaypoint], rotationSpeed);

	VEC3 deltaMovement = wayPoints[currentWaypoint] - myPosition;
	deltaMovement.Normalize();
	deltaMovement = deltaMovement * movementSpeed * delta;

	EnginePhysics.move(getCollider()->controller, toPxVec3(deltaMovement), delta);

	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::idle(float delta) {
	getSkeleton()->blendCycle(0, 0.2f, 0.2f);
	return Leave;
}

bool CBehaviorTreeFlyingRangedEnemy::deathCondition(float delta) {
	return health <= 0;
}

bool CBehaviorTreeFlyingRangedEnemy::deadCondition(float delta) {
	return isDead;
}

bool CBehaviorTreeFlyingRangedEnemy::grabCondition(float delta) {
	return receivedAttack.grab;
}

bool CBehaviorTreeFlyingRangedEnemy::propelCondition(float delta) {
	return receivedAttack.propel;
}

bool CBehaviorTreeFlyingRangedEnemy::horizontalLaunchCondition(float delta) {
	return receivedAttack.horizontalLauncher;
}

bool CBehaviorTreeFlyingRangedEnemy::verticalLaunchCondition(float delta) {
	return receivedAttack.verticalLauncher;
}

bool CBehaviorTreeFlyingRangedEnemy::onStunCondition(float delta) {
	return receivedAttack.stun;
}

bool CBehaviorTreeFlyingRangedEnemy::stunCondition(float delta) {
	return isStunned;
}

bool CBehaviorTreeFlyingRangedEnemy::returnToSpawnCondition(float delta) {
	return VEC3::DistanceSquared(getTransform()->getPosition(), spawnPosition) > recallDistanceSqrd;
}

bool CBehaviorTreeFlyingRangedEnemy::meleeAttackCondition(float delta) {
	return VEC3::DistanceSquared(getTransform()->getPosition(), getPlayerTransform()->getPosition()) < minCombatDistanceSqrd;
}

bool CBehaviorTreeFlyingRangedEnemy::combatCondition(float delta) {
	float distance = VEC3::DistanceSquared(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	return distance < maxCombatDistanceSqrd && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov);
}

bool CBehaviorTreeFlyingRangedEnemy::patrolCondition(float delta) {
	return wayPoints.size() > 0;
}

void CBehaviorTreeFlyingRangedEnemy::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	spawnPosition = getTransform()->getPosition();
	playerHandle = getEntityByName(PLAYER_NAME);
}

void CBehaviorTreeFlyingRangedEnemy::onAttackHit(const TMsgAttackHit& msg) {
	getSkeleton()->blendCycle(0);
	getSkeleton()->setTimeFactor(1);
	isStunned = false;
	receivedAttack = msg.info;
	current = tree["onAttackHit"];
}

void CBehaviorTreeFlyingRangedEnemy::onRespawn(const TMsgRespawn& msg) {
	current = tree["onRespawn"];
}

void CBehaviorTreeFlyingRangedEnemy::onOutOfBounds(const TMsgOutOfBounds& msg) {
	current = tree["onDeath"];
}

void CBehaviorTreeFlyingRangedEnemy::onPerfectDodged(const TMsgPerfectDodged & msg) {
	dbg("Damn! I've been dodged.\n");
}

void CBehaviorTreeFlyingRangedEnemy::onHitboxEnter(const TMsgHitboxEnter& msg) {
	if (attackInfos.find(msg.hitbox) != attackInfos.end()) {
		TMsgAttackHit attackHit = {};
		attackHit.attacker = CHandle(this).getOwner();
		attackHit.info = attackInfos[msg.hitbox];
		((CEntity*)msg.h_other_entity)->sendMsg(attackHit);
	}
}

void CBehaviorTreeFlyingRangedEnemy::rotateTowards(float delta, VEC3 targetPos, float rotationSpeed) {
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

TCompTransform* CBehaviorTreeFlyingRangedEnemy::getTransform() {
	return get<TCompTransform>();
}

TCompCollider* CBehaviorTreeFlyingRangedEnemy::getCollider() {
	return get<TCompCollider>();
}

void CBehaviorTreeFlyingRangedEnemy::setSpawnPosition(VEC3 spawnPosition) {
	this->spawnPosition = spawnPosition;
}

CEntity* CBehaviorTreeFlyingRangedEnemy::getPlayerEntity() {
	return playerHandle;
}

TCompTransform* CBehaviorTreeFlyingRangedEnemy::getPlayerTransform() {
	return getPlayerEntity()->get<TCompTransform>();
}

TCompSkeleton* CBehaviorTreeFlyingRangedEnemy::getSkeleton() {
	return get<TCompSkeleton>();
}

TCompHitboxes* CBehaviorTreeFlyingRangedEnemy::getHitboxes() {
	return get<TCompHitboxes>();
}