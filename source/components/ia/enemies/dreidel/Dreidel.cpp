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
}

void Dreidel::load(const json& j, TEntityParseContext& ctx) {
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

int Dreidel::damageCalc(float delta) {
	health -= receivedAttack.damage;
	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
	return Leave;
}

int Dreidel::onDeath(float delta) {
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

int Dreidel::dead(float delta) {
	return Leave;
}

int Dreidel::onGrab(float delta) {
	getSkeleton()->setTimeFactor(0);
	getCollider()->destroy();
	timer.reset();
	grabbedDuration = receivedAttack.grab->duration;
	return Leave;
}

int Dreidel::grabbed(float delta) {
	if (timer.elapsed() >= grabbedDuration) {
		getCollider()->create();
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}

int Dreidel::onPropel(float delta) {
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

int Dreidel::propelled(float delta) {
	if (timer.elapsed() < propelDuration) {
		VEC3 deltaMovement = velocityVector * delta;
		updateGravity(delta);
		//EnginePhysics.move(getCollider()->controller, toPxVec3(deltaMovement), delta)
		EnginePhysics.move(getCollider()->controller, toPxVec3(deltaMovement), delta);
		return Stay;
	}
	else {
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
}

int Dreidel::onHorizontalLaunch(float delta) {
	getSkeleton()->setTimeFactor(0);
	floatingDuration = receivedAttack.horizontalLauncher->suspensionDuration;
	initialLaunchPos = getTransform()->getPosition();
	velocityVector = receivedAttack.horizontalLauncher->velocity;
	return Leave;
}

int Dreidel::horizontalLaunched(float delta) {
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

int Dreidel::onVerticalLaunch(float delta) {
	getSkeleton()->setTimeFactor(0);
	floatingDuration = receivedAttack.verticalLauncher->suspensionDuration;
	velocityVector = receivedAttack.verticalLauncher->velocity;
	return Leave;
}

int Dreidel::verticalLaunched(float delta) {
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

int Dreidel::floating(float delta) {
	getSkeleton()->setTimeFactor(0);
	if (timer.elapsed() > floatingDuration) {
		getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	else {
		return Stay;
	}
}

int Dreidel::onStun(float delta) {
	getSkeleton()->setTimeFactor(0);
	stunDuration = receivedAttack.stun->duration;
	stunTimer.reset();
	isStunned = true;
	return Leave;
}

int Dreidel::stunned(float delta) {
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

int Dreidel::airborne(float delta) {
	getSkeleton()->blendCycle(1, 0.2f, 0.2f);
	updateGravity(delta);
	return Leave;
}

int Dreidel::respawn(float delta) {
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

int Dreidel::returnToSpawn(float delta) {
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

int Dreidel::chase(float delta) {
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

int Dreidel::stepBack(float delta) {
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

int Dreidel::onIdleWar(float delta) {
	getSkeleton()->blendCycle(0, 0.2f, 0.2f);
	updateGravity(delta);
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	idleWarTimer.reset();
	return Leave;
}

int Dreidel::idleWar(float delta) {
	updateGravity(delta);
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (idleWarTimer.elapsed() > getSkeleton()->getAnimationDuration(0)) {
		return Leave;
	}
	else {
		return Stay;
	}
}

int Dreidel::onAttack(float delta) {
	updateGravity(delta);
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (attackTimer.elapsed() < attackCooldown) {
		current = nullptr;
	}
	else {
		hasAttacked = false;
		attackTimer.reset();
		getSkeleton()->executeAction(2, 0.2f, 0.2f);
	}
	return Leave;
}

int Dreidel::attack(float delta) {
	updateGravity(delta);
	rotateTowards(delta, getPlayerTransform()->getPosition(), rotationSpeed);
	if (attackTimer.elapsed() < (getSkeleton()->getAnimationDuration(2))) {
		if (!hasAttacked && attackTimer.elapsed() >= frames2sec(attacksFrameData["attack"].x)) {
			getHitboxes()->enable("attack");
			hasAttacked = true;
		}
		else if (hasAttacked && attackTimer.elapsed() >= frames2sec(attacksFrameData["attack"].x + attacksFrameData["attack"].y)) {
			getHitboxes()->disable("attack");
		}
		return Stay;
	}
	else {
		getHitboxes()->disable("attack");
		return Leave;
	}
}

int Dreidel::idle(float delta) {
	getSkeleton()->blendCycle(0, 0.2f, 0.2f);
	updateGravity(delta);
	return Leave;
}

bool Dreidel::deathCondition(float delta) {
	return health <= 0;
}

bool Dreidel::deadCondition(float delta) {
	return isDead;
}

bool Dreidel::grabCondition(float delta) {
	return receivedAttack.grab;
}

bool Dreidel::propelCondition(float delta) {
	return receivedAttack.propel;
}

bool Dreidel::horizontalLaunchCondition(float delta) {
	return receivedAttack.horizontalLauncher;
}

bool Dreidel::verticalLaunchCondition(float delta) {
	return receivedAttack.verticalLauncher;
}

bool Dreidel::onStunCondition(float delta) {
	return receivedAttack.stun;
}

bool Dreidel::stunCondition(float delta) {
	return isStunned;
}

bool Dreidel::airborneCondition(float delta) {
	return !grounded;
}

bool Dreidel::returnToSpawnCondition(float delta) {
	return VEC3::DistanceSquared(getTransform()->getPosition(), spawnPosition) > recallDistanceSqrd;
}

bool Dreidel::chaseCondition(float delta) {
	float distanceSqrd = VEC3::DistanceSquared(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	bool isPlayerInFov = getTransform()->isInFov(getPlayerTransform()->getPosition(), chaseFov);
	return distanceSqrd > minCombatDistanceSqrd && (distanceSqrd < smallChaseRadiusSqrd || (distanceSqrd < fovChaseDistanceSqrd && isPlayerInFov));
}

bool Dreidel::combatCondition(float delta) {
	float distanceSqrd = VEC3::DistanceSquared(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	return distanceSqrd < maxCombatDistanceSqrd && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov);
}

bool Dreidel::stepBackCondition(float delta) {
	VEC3 myPos = getTransform()->getPosition();
	VEC3 playerPos = getPlayerTransform()->getPosition();
	myPos.y = 0;
	playerPos.y = 0;
	//float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	float distanceSqrd = VEC3::DistanceSquared(myPos, playerPos);
	return distanceSqrd < minCombatDistanceSqrd;
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