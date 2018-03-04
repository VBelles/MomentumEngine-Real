#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_melee_enemy.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "render/render_utils.h"
#include "components/player/comp_player_model.h"
#include "components/comp_respawner.h"
#include "components/comp_shadow.h"

DECL_OBJ_MANAGER("ai_melee_enemy", CAIMeleeEnemy);


void CAIMeleeEnemy::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	InitStates();
}

void CAIMeleeEnemy::registerMsgs() {
	DECL_MSG(CAIMeleeEnemy, TMsgEntitiesGroupCreated, OnGroupCreated);
	DECL_MSG(CAIMeleeEnemy, TMsgAttackHit, OnHit);
	DECL_MSG(CAIMeleeEnemy, TMsgGrabbed, OnGrabbed);
	DECL_MSG(CAIMeleeEnemy, TMsgPropelled, OnPropelled);
	DECL_MSG(CAIMeleeEnemy, TMsgLaunchedVertically, OnLaunchedVertically);
	DECL_MSG(CAIMeleeEnemy, TMsgLaunchedHorizontally, OnLaunchedHorizontally);
	DECL_MSG(CAIMeleeEnemy, TMsgRespawn, OnRespawn);
}

void CAIMeleeEnemy::debugInMenu() {
	IAIController::debugInMenu();
}

void CAIMeleeEnemy::update(float delta) {
	IAIController::update(delta);
}

void CAIMeleeEnemy::InitStates() {
	AddState("idle", (statehandler)&CAIMeleeEnemy::IdleState);
	AddState("chase", (statehandler)&CAIMeleeEnemy::ChaseState);
	AddState("recall", (statehandler)&CAIMeleeEnemy::RecallState);
	AddState("idle_war", (statehandler)&CAIMeleeEnemy::IdleWarState);
	AddState("attack", (statehandler)&CAIMeleeEnemy::AttackState);
	AddState("death", (statehandler)&CAIMeleeEnemy::DeathState);
	AddState("vertical_launch", (statehandler)&CAIMeleeEnemy::VerticalLaunchState);
	AddState("horizontal_launch", (statehandler)&CAIMeleeEnemy::HorizontalLaunchState);
	AddState("grabbed", (statehandler)&CAIMeleeEnemy::GrabbedState);
	AddState("propelled", (statehandler)&CAIMeleeEnemy::PropelledState);
	AddState("floating", (statehandler)&CAIMeleeEnemy::FloatingState);
	ChangeState("idle");
}


void CAIMeleeEnemy::OnGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	spawnPosition = getTransform()->getPosition();
	player = getEntityByName("The Player");
	TCompPlayerModel* playerModel = getPlayerEntity()->get<TCompPlayerModel>();
	launchPowerStats = &*playerModel->GetPowerStats();

	TCompShadow *shadow = get<TCompShadow>();
	originalShadowMesh = shadow->mesh->getName();
}

void CAIMeleeEnemy::OnHit(const TMsgAttackHit& msg) {
	float damage = msg.damage;
	health -= damage;

	CEntity *attacker = msg.attacker;
	attacker->sendMsg(TMsgGainPower{ CHandle(this), powerGiven });

	if (health <= 0) {
		ChangeState("death");
	}
	else {
		ChangeState("idle");
	}
}

void CAIMeleeEnemy::OnGrabbed(const TMsgGrabbed& msg) {
	dbg("grabbed\n");
	grabbedTimer.reset();
	ChangeState("grabbed");
	CEntity *attacker = msg.attacker;
	attacker->sendMsg(TMsgGainPower{ CHandle(this), powerGiven });//Si agarras a dos ganas doble poder
	getCollider()->disable();
}

void CAIMeleeEnemy::GrabbedState(float delta) {
	//dbg("En grabbed\n");
	if (grabbedTimer.elapsed() >= grabbedDuration) {
		getCollider()->enable();
		ChangeState("idle");
	}
}

void CAIMeleeEnemy::OnPropelled(const TMsgPropelled& msg) {
	CEntity *attacker = msg.attacker;
	propelVelocityVector = msg.velocityVector;
	getCollider()->enable();

	propelTimer.reset();
	ChangeState("propelled");

}


void CAIMeleeEnemy::OnLaunchedVertically(const TMsgLaunchedVertically& msg) {
	TCompPlayerModel* playerModel = getPlayerEntity()->get<TCompPlayerModel>();
	launchPowerStats = &*playerModel->GetPowerStats();
	velocityVector.y = launchPowerStats->jumpVelocityVector.y;
	launchPowerStats->currentGravityMultiplier = launchPowerStats->normalGravityMultiplier;
	ChangeState("vertical_launch");
}

void CAIMeleeEnemy::OnLaunchedHorizontally(const TMsgLaunchedHorizontally& msg) {
	TCompPlayerModel* playerModel = getPlayerEntity()->get<TCompPlayerModel>();
	launchPowerStats = &*playerModel->GetPowerStats();
	initialHorizontalLaunchY = getTransform()->getPosition().y + 0.001f;
	dbg("Y: %f", initialHorizontalLaunchY);
	float horizontalVelocity = launchPowerStats->longJumpVelocityVector.z;
	velocityVector = msg.direction * horizontalVelocity;
	velocityVector.y = launchPowerStats->longJumpVelocityVector.y;
	launchPowerStats->currentGravityMultiplier = launchPowerStats->longGravityMultiplier;
	ChangeState("horizontal_launch");
	horizontalLaunchTimer.reset();
}

void CAIMeleeEnemy::OnRespawn(const TMsgRespawn & msg) {
	if (isDead) {
		ChangeState("idle");
		isDead = false;
		health = 5.f;
		getCollider()->enable();
		getTransform()->setPosition(spawnPosition);
		getCollider()->controller->setFootPosition(PxExtendedVec3(spawnPosition.x, spawnPosition.y, spawnPosition.z));
		TCompRender *render = get<TCompRender>();
		render->setMesh(originalMeshPath);
		TCompShadow *shadow = get<TCompShadow>();
		shadow->setMesh(originalShadowMesh);
	}
}


void CAIMeleeEnemy::IdleState(float delta) {
	UpdateGravity(delta);
	if (IsPlayerInFov()) {
		ChangeState("chase");
	}
}

void CAIMeleeEnemy::ChaseState(float delta) {
	UpdateGravity(delta);
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

	if (VEC3::Distance(getTransform()->getPosition(), spawnPosition) > recallDistance) {
		recallTimer.reset();
		ChangeState("recall");
	}
	if (IsPlayerInAttackRange(startAttackingRadius)) {
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
			getCollider()->controller->move(PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, PxControllerFilters());
		}
	}
	else {
		getTransform()->setPosition(spawnPosition);
		getCollider()->controller->setFootPosition(PxExtendedVec3(spawnPosition.x, spawnPosition.y, spawnPosition.z));
		ChangeState("idle");
	}
}

void CAIMeleeEnemy::IdleWarState(float delta) {
	UpdateGravity(delta);
	if (!IsPlayerInAttackRange(startAttackingRadius)) {
		ChangeState("idle");
		return;
	}
	if (waitAttackTimer.elapsed() > attackCooldown) {
		attackTimer.reset();
		ChangeState("attack");
	}
}

void CAIMeleeEnemy::AttackState(float delta) {
	UpdateGravity(delta);
	if (attackTimer.elapsed() > attackDuration) {
		waitAttackTimer.reset();
		ChangeState("idle_war");
		if(IsPlayerInAttackRange(rangeRadius)) {
			TMsgAttackHit msg{ CHandle(this), 1 };
			getPlayerEntity()->sendMsg(msg);
		}
	}
}

void CAIMeleeEnemy::DeathState(float delta) {
	if (!isDead) {
		getCollider()->disable();
		//CHandle(this).getOwner().destroy();
		TCompRender *render = get<TCompRender>();
		render->setMesh("data/meshes/nada.mesh");
		TCompRespawner* spawner = get<TCompRespawner>();
		TCompShadow *shadow = get<TCompShadow>();
		shadow->setMesh("data/meshes/nada.mesh");
		spawner->OnDead();
		isDead = true;
	}
	
}

void CAIMeleeEnemy::VerticalLaunchState(float delta) {
	UpdateGravity(delta);
	if (velocityVector.y <= 0) {
		ChangeState("floating");
		launchedFloatingTimer.reset();
	}
}

void CAIMeleeEnemy::HorizontalLaunchState(float delta) {
	UpdateGravity(delta);
	VEC3 deltaMovement = velocityVector * delta;
	getCollider()->controller->move(physx::PxVec3(deltaMovement.x, 0, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	if (horizontalLaunchTimer.elapsed() >= minHorizontalLaunchDuration && getTransform()->getPosition().y <= initialHorizontalLaunchY) {
		velocityVector.x = 0;
		velocityVector.z = 0;
		ChangeState("floating");
		launchedFloatingTimer.reset();
	}
}



void CAIMeleeEnemy::PropelledState(float delta) {
	dbg("On propelled\n");
	if (propelTimer.elapsed() < propelDuration) {
		VEC3 deltaMovement = propelVelocityVector * delta;
		getCollider()->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	}
	else {
		ChangeState("idle");
	}
}

void CAIMeleeEnemy::FloatingState(float delta) {
	if (launchedFloatingTimer.elapsed() > floatingDuration) {
		ChangeState("idle");
	}
}

boolean CAIMeleeEnemy::IsPlayerInAttackRange(float range) {
	float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	return distance < range && getTransform()->isInFov(getPlayerTransform()->getPosition(), chaseFov);
}

boolean CAIMeleeEnemy::IsPlayerInFov() {
	float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	return distance < smallChaseRadius || (distance < fovChaseDistance && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov));
}


void CAIMeleeEnemy::UpdateGravity(float delta) {
	TCompPlayerModel* playerModel = getPlayerEntity()->get<TCompPlayerModel>();
	float deltaY = CalculateVerticalDeltaMovement(delta,
		playerModel->GetAccelerationVector()->y * launchPowerStats->currentGravityMultiplier,
		launchPowerStats->maxVelocityVertical);
	getCollider()->controller->move(physx::PxVec3(0, deltaY, 0), 0.f, delta, physx::PxControllerFilters());
	velocityVector.y += playerModel->GetAccelerationVector()->y * launchPowerStats->currentGravityMultiplier * delta;
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

