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
	DECL_MSG(CAIMeleeEnemy, TMsgAttackHit, OnHit);
}

void CAIMeleeEnemy::update(float delta) {
	IAIController::update(delta);
	if (EngineInput["jump"].getsPressed()) {
		LaunchVertically();
	}
}

void CAIMeleeEnemy::InitStates() {
	AddState("idle", (statehandler)&CAIMeleeEnemy::IdleState);
	AddState("chase", (statehandler)&CAIMeleeEnemy::ChaseState);
	AddState("recall", (statehandler)&CAIMeleeEnemy::RecallState);
	AddState("idle_war", (statehandler)&CAIMeleeEnemy::IdleWarState);
	AddState("attack", (statehandler)&CAIMeleeEnemy::AttackState);
	AddState("death", (statehandler)&CAIMeleeEnemy::DeathState);
	AddState("vertical_launch", (statehandler)&CAIMeleeEnemy::VerticalLaunchState);
	ChangeState("idle");
}

void CAIMeleeEnemy::OnHit(const TMsgAttackHit& msg) {
	float damage = msg.damage;
	health -= damage;

	CEntity *attacker = msg.attacker;
	attacker->sendMsg(TMsgGainPower{ CHandle(this), powerGiven });

	if (health <= 0) {
		ChangeState("death");
	}
}

void CAIMeleeEnemy::OnGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	spawnPosition = getTransform()->getPosition();
	player = getEntityByName("The Player");
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
	getTransform()->getYawPitchRoll(&y, &p, &r);
	int dir = getTransform()->isInLeft(getPlayerTransform()->getPosition()) ? 1 : -1;
	y += dir * rotationSpeed * delta;
	getTransform()->setYawPitchRoll(y, p, r);

	//Move forward
	VEC3 myPosition = getTransform()->getPosition();
	VEC3 myFront = getTransform()->getFront();
	myFront.Normalize();
	VEC3 deltaMovement = myFront * movementSpeed * delta;
	//transform->setPosition(myPosition + deltaMovement);
	getCollider()->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());

	if (!IsPlayerInFov()) {
		recallTimer.reset();
		ChangeState("recall");
	}
	if (IsPlayerInAttackRange()) {
		ChangeState("idle_war");
	}
}

void CAIMeleeEnemy::RecallState(float delta) {
	if (recallTimer.elapsed() <= 2.f) {
		float y, r, p;
		getTransform()->getYawPitchRoll(&y, &p, &r);
		y += 35 * delta;
		getTransform()->setYawPitchRoll(y, p, r);
		if (recallTimer.elapsed() > 1.f) {
			VEC3 deltaMovement = VEC3::Up * 25.f * delta;
			//transform->setPosition(transform->getPosition() + VEC3::Up * 25.f * delta);
			getCollider()->controller->move(PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, PxControllerFilters());
		}
	}
	else {
		getTransform()->setPosition(spawnPosition);
		getCollider()->controller->setPosition(PxExtendedVec3(spawnPosition.x, spawnPosition.y, spawnPosition.z));
		ChangeState("idle");
	}
}

void CAIMeleeEnemy::IdleWarState(float delta) {
	if (!IsPlayerInAttackRange()) {
		ChangeState("idle");
		return;
	}
	if (waitAttackTimer.elapsed() > attackColdown) {
		attackTimer.reset();
		ChangeState("attack");
	}
}

void CAIMeleeEnemy::AttackState(float delta) {
	if (attackTimer.elapsed() > attackDuration) {
		waitAttackTimer.reset();
		ChangeState("idle_war");
		TMsgAttackHit msg{ CHandle(this), 1 };
		getPlayerEntity()->sendMsg(msg);
	}
}

boolean CAIMeleeEnemy::IsPlayerInAttackRange() {
	float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	return distance < attackRadius && getTransform()->isInFov(getPlayerTransform()->getPosition(), chaseFov);
}

boolean CAIMeleeEnemy::IsPlayerInFov() {
	float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	return distance < smallChaseRadius || (distance < fovChaseDistance && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov));
}

void CAIMeleeEnemy::DeathState(float delta) {
	getCollider()->controller->release();
	CHandle(this).getOwner().destroy();
}

void CAIMeleeEnemy::VerticalLaunchState(float delta) {
	float deltaY = CalculateVerticalDeltaMovement(delta, -9.8f, 25); //TODO: fix random value
	getCollider()->controller->move(physx::PxVec3(0, deltaY, 0), 0.f, delta, physx::PxControllerFilters());
	//Nueva velocidad vertical y clampeo
	velocityVector.y += -9.8f * delta;
}

void CAIMeleeEnemy::LaunchVertically() {
	velocityVector.y = 10; //TODO: fix random value
	ChangeState("vertical_launch");
}

//Calcula el movimiento vertical desde el último frame y lo clampea con la máxima distancia 
//recorrida posible en caso de ir a máxima velocidad
float CAIMeleeEnemy::CalculateVerticalDeltaMovement(float delta, float acceleration, float maxVelocityVertical) {
	float resultingDeltaMovement;
	resultingDeltaMovement = velocityVector.y * delta + 0.5f * acceleration * delta * delta;
	//clampear distancia vertical
	resultingDeltaMovement = resultingDeltaMovement > maxVelocityVertical * delta ? maxVelocityVertical * delta : resultingDeltaMovement;
	return resultingDeltaMovement;
}

