#include "mcv_platform.h"
#include "BehaviorTreeMeleeEnemy.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_render.h"
#include "components/comp_respawner.h"
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
	addChild("attackProperties", "verticalLaunchProperty", Sequence, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::verticalLaunchCondition, nullptr);
	addChild("verticalLaunchProperty", "onVerticalLaunch", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::onVerticalLaunch);
	addChild("verticalLaunchProperty", "verticalLaunched", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::verticalLaunched);
	addChild("attackProperties", "onStun", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::onStun);

	addChild("meleeEnemy", "onRespawn", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::falseCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::respawn);

	addChild("meleeEnemy", "dead", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::deadCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::dead);

	addChild("meleeEnemy", "stunned", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::stunCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::stunned);

	addChild("meleeEnemy", "returnToSpawn", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::returnToSpawnCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::returnToSpawn);

	addChild("meleeEnemy", "chase", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::chaseCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::chase);

	addChild("meleeEnemy", "combat", Random, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::combatCondition, nullptr);
	addChild("combat", "idleWar", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::idleWar);
	addChild("combat", "attack", Action, nullptr, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::attack);

	addChild("meleeEnemy", "idle", Action, (BehaviorTreeCondition)&CBehaviorTreeMeleeEnemy::trueCondition, (BehaviorTreeAction)&CBehaviorTreeMeleeEnemy::idle);
}

void CBehaviorTreeMeleeEnemy::load(const json& j, TEntityParseContext& ctx) {
}

void CBehaviorTreeMeleeEnemy::debugInMenu() {
}

void CBehaviorTreeMeleeEnemy::registerMsgs() {
	DECL_MSG(CBehaviorTreeMeleeEnemy, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(CBehaviorTreeMeleeEnemy, TMsgAttackHit, onAttackHit);
	DECL_MSG(CBehaviorTreeMeleeEnemy, TMsgRespawn, onRespawn);
}

void CBehaviorTreeMeleeEnemy::update(float delta) {
	recalc();
}

int CBehaviorTreeMeleeEnemy::damageCalc() {
	health -= receivedAttack.damage;
	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
	return Leave;
}

int CBehaviorTreeMeleeEnemy::onDeath() {
	health = 0.f;
	isDead = false;

	getCollider()->disable();

	TCompRender *render = get<TCompRender>();
	render->disable();

	TCompRespawner* spawner = get<TCompRespawner>();
	spawner->OnDead();

	return Leave;
}

int CBehaviorTreeMeleeEnemy::dead() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::onGrab() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::grabbed() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::onPropel() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::propelled() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::onHorizontalLaunch() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::horizontalLaunched() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::onVerticalLaunch() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::verticalLaunched() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::onStun() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::stunned() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::respawn() {
	health = maxHealth;
	isDead = false;

	getCollider()->enable();
	getTransform()->setPosition(spawnPosition);
	getCollider()->controller->setFootPosition(PxExtendedVec3(spawnPosition.x, spawnPosition.y, spawnPosition.z));

	TCompRender *render = get<TCompRender>();
	render->enable();
	return Leave;
}

int CBehaviorTreeMeleeEnemy::returnToSpawn() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::chase() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::idleWar() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::attack() {
	return Leave;
}

int CBehaviorTreeMeleeEnemy::idle() {
	return Leave;
}

bool CBehaviorTreeMeleeEnemy::deathCondition() {
	return health <= 0;
}

bool CBehaviorTreeMeleeEnemy::deadCondition() {
	return isDead;
}

bool CBehaviorTreeMeleeEnemy::grabCondition() {
	return receivedAttack.grab;
}

bool CBehaviorTreeMeleeEnemy::propelCondition() {
	return receivedAttack.propel;
}

bool CBehaviorTreeMeleeEnemy::horizontalLaunchCondition() {
	return receivedAttack.horizontalLauncher;
}

bool CBehaviorTreeMeleeEnemy::verticalLaunchCondition() {
	return receivedAttack.verticalLauncher;
}

bool CBehaviorTreeMeleeEnemy::stunCondition() {
	return isStunned;
}

bool CBehaviorTreeMeleeEnemy::returnToSpawnCondition() {
	return VEC3::Distance(getTransform()->getPosition(), spawnPosition) > recallDistance;
}

bool CBehaviorTreeMeleeEnemy::chaseCondition() {
	float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	return distance < smallChaseRadius || (distance < fovChaseDistance && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov));
}

bool CBehaviorTreeMeleeEnemy::combatCondition() {
	float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	return distance < rangeRadius && getTransform()->isInFov(getPlayerTransform()->getPosition(), chaseFov);
}

void CBehaviorTreeMeleeEnemy::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	spawnPosition = getTransform()->getPosition();
	playerHandle = getEntityByName("The Player");
	TCompPlayerModel* playerModel = getPlayerEntity()->get<TCompPlayerModel>();
}

void CBehaviorTreeMeleeEnemy::onAttackHit(const TMsgAttackHit& msg) {
	receivedAttack = msg.info;
	current = tree["onAttackHit"];
}

void CBehaviorTreeMeleeEnemy::onRespawn(const TMsgRespawn& msg) {
	current = tree["onRespawn"];
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