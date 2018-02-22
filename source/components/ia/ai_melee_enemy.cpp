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
	AddState("idle_war", (statehandler)&CAIMeleeEnemy::IdleWarState);
	AddState("atack", (statehandler)&CAIMeleeEnemy::AtackState);
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
	collider = get<TCompCollider>();
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
	y += dir * rotationSpeed * delta;
	transform->setYawPitchRoll(y, p, r);

	//Move forward
	VEC3 myPosition = transform->getPosition();
	VEC3 myFront = transform->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = myFront * movementSpeed * delta;
	//transform->setPosition(myPosition + deltaMovement);
	collider->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());

	if (!IsPlayerInFov()) {
		recallTimer.reset();
		ChangeState("recall");
	}
	if (IsPlayerInAtackRange()) {
		ChangeState("idle_war");
	}
}

void CAIMeleeEnemy::RecallState(float delta) {
	if (recallTimer.elapsed() <= 2.f) {
		float y, r, p;
		transform->getYawPitchRoll(&y, &p, &r);
		y += 35 * delta;
		transform->setYawPitchRoll(y, p, r);
		if (recallTimer.elapsed() > 1.f) {
			VEC3 deltaMovement = VEC3::Up * 25.f * delta;
			//transform->setPosition(transform->getPosition() + VEC3::Up * 25.f * delta);
			collider->controller->move(PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, PxControllerFilters());
		}
	}
	else {
		transform->setPosition(spawnPosition);
		collider->controller->setPosition(PxExtendedVec3(spawnPosition.x, spawnPosition.y, spawnPosition.z));
		ChangeState("idle");
	}
}

void CAIMeleeEnemy::IdleWarState(float delta) {
	if (!IsPlayerInAtackRange()) {
		ChangeState("idle");
		return;
	}
	if (waitAtackTimer.elapsed() > atackColdown) {
		atackTimer.reset();
		ChangeState("atack");
	}
}

void CAIMeleeEnemy::AtackState(float delta) {
	if (atackTimer.elapsed() > atackDuration) {
		waitAtackTimer.reset();
		ChangeState("idle_war");
		TMsgAtackHit msg{ CHandle(this), 1 };
		player->sendMsg(msg);
	}
}

boolean CAIMeleeEnemy::IsPlayerInAtackRange() {
	float distance = VEC3::Distance(transform->getPosition(), playerTransform->getPosition());
	return distance < atackRadius && transform->isInFov(playerTransform->getPosition(), chaseFov);
}

boolean CAIMeleeEnemy::IsPlayerInFov() {
	float distance = VEC3::Distance(transform->getPosition(), playerTransform->getPosition());
	return distance < smallChaseRadius || (distance < fovChaseDistance && transform->isInFov(playerTransform->getPosition(), atackFov));
}


