#include "mcv_platform.h"
#include "handle/handle.h"
#include "ai_flying_enemy.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "components/comp_render.h"
#include "render/render_utils.h"

DECL_OBJ_MANAGER("ai_flying_enemy", CAIFlyingEnemy);


void CAIFlyingEnemy::load(const json& j, TEntityParseContext& ctx) {
	setEntity(ctx.current_entity);
	InitStates();
}

void CAIFlyingEnemy::registerMsgs() {
	DECL_MSG(CAIFlyingEnemy, TMsgEntitiesGroupCreated, OnGroupCreated);
	DECL_MSG(CAIFlyingEnemy, TMsgAttackHit, OnHit);
}

void CAIFlyingEnemy::update(float delta) {
	IAIController::update(delta);
}

void CAIFlyingEnemy::InitStates() {
	AddState("idle", (statehandler)&CAIFlyingEnemy::IdleState);
	AddState("death", (statehandler)&CAIFlyingEnemy::DeathState);
	AddState("grabbed", (statehandler)&CAIFlyingEnemy::GrabbedState);
	AddState("propelled", (statehandler)&CAIFlyingEnemy::PropelledState);
	ChangeState("idle");
}

void CAIFlyingEnemy::OnHit(const TMsgAttackHit& msg) {
	float damage = msg.info.damage;
	health -= damage;
	CEntity *attacker = msg.attacker;
	if (msg.info.givesPower) {
		//esto se tendría que hacer antes de enviar el mensaje, para subir de nivel antes
		attacker->sendMsg(TMsgGainPower{ CHandle(this), powerGiven });
	}
	if (health <= 0) {
		ChangeState("death");
	}
	else {
		if (msg.info.grab) {
			OnGrabbed(msg.info.grab->duration);
		}
		if (msg.info.propel) {
			OnPropelled(msg.info.propel->velocity);
		}
	}

}

void CAIFlyingEnemy::OnGrabbed(float duration) {
	ChangeState("grabbed");
	grabbedTimer.reset();
	grabbedDuration = duration;
	//Quitar collider
	getCollider()->destroy();
}

void CAIFlyingEnemy::GrabbedState(float delta) {
	if (grabbedTimer.elapsed() >= grabbedDuration) {
		getCollider()->create();
		ChangeState("idle");
	}
}

void CAIFlyingEnemy::OnPropelled(VEC3 velocity) {
	ChangeState("propelled");
	getCollider()->create();
	propelVelocityVector = velocity;
	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
}

void CAIFlyingEnemy::PropelledState(float delta) {
	//Si no ha pasado el timer
	//mover propelVelocityVector * delta
	ChangeState("idle");
}

void CAIFlyingEnemy::OnGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	TCompTransform* transform = getTransform();
	spawnPosition = transform->getPosition();
	player = getEntityByName("The Player");
}

void CAIFlyingEnemy::debugInMenu() {
	IAIController::debugInMenu();
}

void CAIFlyingEnemy::IdleState(float delta) {
}

boolean CAIFlyingEnemy::IsPlayerInAttackRange() {
	float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	return distance < startAttackingRadius && getTransform()->isInFov(getPlayerTransform()->getPosition(), chaseFov);
}

boolean CAIFlyingEnemy::IsPlayerInFov() {
	float distance = VEC3::Distance(getTransform()->getPosition(), getPlayerTransform()->getPosition());
	return distance < smallChaseRadius || (distance < fovChaseDistance && getTransform()->isInFov(getPlayerTransform()->getPosition(), attackFov));
}

void  CAIFlyingEnemy::DeathState(float delta) {
	TCompCollider *collider = get<TCompCollider>();
	collider->destroy();
	CHandle(this).getOwner().destroy();
}
