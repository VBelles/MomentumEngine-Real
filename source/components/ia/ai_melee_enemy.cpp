#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_melee_enemy.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "render/render_utils.h"

DECL_OBJ_MANAGER("ai_melee_enemy", CAIMeleeEnemy);


void CAIMeleeEnemy::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	InitStates();
}

void CAIMeleeEnemy::registerMsgs() {
	DECL_MSG(CAIMeleeEnemy, TMsgEntitiesGroupCreated, OnGroupCreated);
	DECL_MSG(CAIMeleeEnemy, TMsgDamage, OnHit);
}

void CAIMeleeEnemy::InitStates() {
	AddState("idle", (statehandler)&CAIMeleeEnemy::IdleState);
	AddState("chase", (statehandler)&CAIMeleeEnemy::ChaseState);
	AddState("recall", (statehandler)&CAIMeleeEnemy::RecallState);
	ChangeState("idle");
}

void CAIMeleeEnemy::OnHit(const TMsgDamage& msg) {
	float damage = msg.damage;
	health -= damage;
	if (health < 0) {
		CHandle(this).getOwner().destroy();
	}
}

void CAIMeleeEnemy::OnGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	transform = getMyTransform();
	spawnPosition = transform->getPosition();
	player = (CEntity *)getEntityByName("The Player");
	playerTransform = player->get<TCompTransform>();
}

void CAIMeleeEnemy::debugInMenu() {
	IAIController::debugInMenu();
}

void CAIMeleeEnemy::IdleState(float delta) {
	if (IsPlayerInFov()) {
		ChangeState("chase");
	}
}

void CAIMeleeEnemy::ChaseState(float delta) {
	//Rotate to the player
	float y, r, p;
	transform->getYawPitchRoll(&y, &p, &r);
	int dir = transform->isInLeft(playerTransform->getPosition()) ? 1 : -1;
	y += dir * 1 * delta;
	transform->setYawPitchRoll(y, p, r);

	//Move forward
	VEC3 myPosition = transform->getPosition();
	VEC3 myFront = transform->getFront();
	myFront.Normalize();
	myPosition = myPosition + myFront * 1.f * delta;
	transform->setPosition(myPosition);

	if (!IsPlayerInFov()) {
		ChangeState("recall");
	}
}

void CAIMeleeEnemy::RecallState(float delta) {
	transform->setPosition(spawnPosition);
	ChangeState("idle");
}

boolean CAIMeleeEnemy::IsPlayerInFov() {
	float distance = VEC3::Distance(transform->getPosition(), playerTransform->getPosition());
	return distance < 10.f || (distance < 20.f && transform->isInFov(playerTransform->getPosition(), deg2rad(60)));
}


