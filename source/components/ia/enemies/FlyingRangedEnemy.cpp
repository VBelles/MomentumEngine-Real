#include "mcv_platform.h"
#include "FlyingRangedEnemy.h"
#include "entity/common_msgs.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_render.h"
#include "components/comp_respawner.h"
#include "components/comp_shadow.h"
#include "components/comp_give_power.h"
#include "components/player/comp_player_model.h"
#include "entity/entity_parser.h"

DECL_OBJ_MANAGER("behaviorTree_flying_ranged_enemy", CBehaviorTreeFlyingRangedEnemy);

CBehaviorTreeFlyingRangedEnemy::CBehaviorTreeFlyingRangedEnemy()
	: IBehaviorTree::IBehaviorTree() {
	createRoot("meleeEnemy", Priority, nullptr, nullptr);

	addChild("meleeEnemy", "onAttackHit", Sequence, &CBehaviorTreeFlyingRangedEnemy::falseCondition, nullptr);
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

	addChild("meleeEnemy", "onRespawn", Action, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::falseCondition, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::respawn);

	addChild("meleeEnemy", "dead", Action, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::deadCondition, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::dead);

	addChild("meleeEnemy", "stunned", Action, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::stunCondition, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::stunned);

	addChild("meleeEnemy", "returnToSpawn", Action, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::returnToSpawnCondition, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::returnToSpawn);

	CBehaviorTreeNodeRandom *node = (CBehaviorTreeNodeRandom*)addChild("meleeEnemy", "combat", Random, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::combatCondition, nullptr);
	addChild("combat", "idleWar", Sequence, nullptr, nullptr);
	addChild("idleWar", "onIdleWar", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::onIdleWar);
	addChild("idleWar", "idleWarAction", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::idleWar);
	addChild("combat", "attack", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::attack);
	node->setProbability(std::vector<float>{ 0.75f, 0.25f });

	addChild("meleeEnemy", "idle", Action, (BehaviorTreeCondition)&CBehaviorTreeFlyingRangedEnemy::trueCondition, (BehaviorTreeAction)&CBehaviorTreeFlyingRangedEnemy::idle);
}

void CBehaviorTreeFlyingRangedEnemy::load(const json& j, TEntityParseContext& ctx) {
	maxHealth = j.value("maxHealth", 5.0f);
	health = maxHealth;
	movementSpeed = j.value("movementSpeed", 2.5f);
	rotationSpeed = j.value("rotationSpeed", 20.f);
	recallDistance = j.value("recallDistance", 5.f);
	attackFov = deg2rad(j.value("attackFov", 60.f));
	minCombatDistance = j.value("minCombatDistance", 2.f);
	maxCombatDistance = j.value("maxCombatDistance", 20.f);
	attackCooldown = j.value("attackCooldown", 2.f);
	attackDamage = j.value("attackDamage", 1.f);
	propelDuration = j.value("propelDuration", 1.5f);
	gravity = j.value("gravity", -50.f);
	if (j.count("attackPrefab")) {
		attackPrefab = j["attackPrefab"].get<std::string>();
	}
	if (j.count("attackSpawnOffset")) {
		attackSpawnOffset = loadVEC3(j["attackSpawnOffset"]);
	}
	if (j.count("attackTargetOffset")) {
		attackTargetOffset = loadVEC3(j["attackTargetOffset"]);
	}
}

void CBehaviorTreeFlyingRangedEnemy::debugInMenu() {
}

void CBehaviorTreeFlyingRangedEnemy::registerMsgs() {
	DECL_MSG(CBehaviorTreeFlyingRangedEnemy, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(CBehaviorTreeFlyingRangedEnemy, TMsgAttackHit, onAttackHit);
	DECL_MSG(CBehaviorTreeFlyingRangedEnemy, TMsgRespawn, onRespawn);
	DECL_MSG(CBehaviorTreeFlyingRangedEnemy, TMsgOutOfBounds, onOutOfBounds);
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

	TCompShadow* shadow = get<TCompShadow>();
	shadow->disable();

	TCompRespawner* spawner = get<TCompRespawner>();
	spawner->onDead();

	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::dead(float delta) {
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::onGrab(float delta) {
	getCollider()->destroy();
	timer.reset();
	grabbedDuration = receivedAttack.grab->duration;
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::grabbed(float delta) {
	if (timer.elapsed() >= grabbedDuration) {
		getCollider()->create();
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeFlyingRangedEnemy::onPropel(float delta) {
	getCollider()->create();
	velocityVector = receivedAttack.propel->velocity;

	timer.reset();

	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::propelled(float delta) {
	if (timer.elapsed() < propelDuration) {
		VEC3 deltaMovement = velocityVector * delta;
		getCollider()->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
		return Stay;
	}
	else {
		return Leave;
	}
}

int CBehaviorTreeFlyingRangedEnemy::onHorizontalLaunch(float delta) {
	floatingDuration = receivedAttack.horizontalLauncher->suspensionDuration;
	initialLaunchPos = getTransform()->getPosition();
	velocityVector = receivedAttack.horizontalLauncher->velocity;
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::horizontalLaunched(float delta) {
	VEC3 deltaMovement = velocityVector * delta;
	getCollider()->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	velocityVector.y += gravity * delta;
	if (getTransform()->getPosition().y + deltaMovement.y - initialLaunchPos.y <= 0.001) {
		velocityVector.x = 0;
		velocityVector.z = 0;
		timer.reset();
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeFlyingRangedEnemy::onVerticalLaunch(float delta) {
	floatingDuration = receivedAttack.verticalLauncher->suspensionDuration;
	velocityVector = receivedAttack.verticalLauncher->velocity;
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::verticalLaunched(float delta) {
	VEC3 deltaMovement = velocityVector * delta;
	getCollider()->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	velocityVector.y += gravity * delta;
	if (velocityVector.y <= 0) {
		timer.reset();
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeFlyingRangedEnemy::floating(float delta) {
	if (timer.elapsed() > floatingDuration) {
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeFlyingRangedEnemy::onStun(float delta) {
	stunDuration = receivedAttack.stun->duration;
	stunTimer.reset();
	isStunned = true;
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::stunned(float delta) {
	if (stunTimer.elapsed() > stunDuration) {
		isStunned = false;
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

	TCompShadow* shadow = get<TCompShadow>();
	shadow->enable();

	TCompRender *render = get<TCompRender>();
	render->enable();

	TCompGivePower *power = get<TCompGivePower>();
	power->reset();

	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::returnToSpawn(float delta) {
	rotateTowards(delta, spawnPosition, rotationSpeed);

	VEC3 myPosition = getTransform()->getPosition();
	VEC3 deltaMovement = spawnPosition - myPosition;
	deltaMovement.Normalize();
	deltaMovement = deltaMovement * movementSpeed * delta;

	getCollider()->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());

	float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	if (VEC3::Distance(myPosition + deltaMovement, spawnPosition) < minCombatDistance 
		|| (distance < maxCombatDistance + maxCombatDistance && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov)))
	{
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeFlyingRangedEnemy::onIdleWar(float delta) {
	idleWarTimer.reset();
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::idleWar(float delta) {
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (idleWarTimer.elapsed() > 1.f) {
		return Leave;
	}
	else {
		return Stay;
	}
}

int CBehaviorTreeFlyingRangedEnemy::attack(float delta) {
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (attackTimer.elapsed() > attackCooldown) {
		TEntityParseContext ctx;
		if (parseScene(attackPrefab, ctx)) {
			assert(!ctx.entities_loaded.empty());

			AttackInfo attackInfo;
			attackInfo.damage = attackDamage;

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

			TMsgAssignRangedAttackOwner msg{ CHandle(this).getOwner(), attackInfo, attackInitialPos, attackDirection };

			CEntity *attackEntity = ctx.entities_loaded[0];
			attackEntity->sendMsg(msg);

			attackTimer.reset();
		}
	}
	return Leave;
}

int CBehaviorTreeFlyingRangedEnemy::idle(float delta) {
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
	return VEC3::Distance(getTransform()->getPosition(), spawnPosition) > recallDistance;
}

bool CBehaviorTreeFlyingRangedEnemy::combatCondition(float delta) {
	float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	return distance < maxCombatDistance && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov);
}

void CBehaviorTreeFlyingRangedEnemy::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	spawnPosition = getTransform()->getPosition();
	playerHandle = getEntityByName(PLAYER_NAME);
}

void CBehaviorTreeFlyingRangedEnemy::onAttackHit(const TMsgAttackHit& msg) {
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

CEntity* CBehaviorTreeFlyingRangedEnemy::getPlayerEntity() {
	return playerHandle;
}

TCompTransform* CBehaviorTreeFlyingRangedEnemy::getPlayerTransform() {
	return getPlayerEntity()->get<TCompTransform>();
}