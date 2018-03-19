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
	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
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
	//Quitar collider
}

void CAIFlyingEnemy::OnPropelled(VEC3 velocity) {
	ChangeState("propelled");
	propelVelocityVector = velocity;
	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
}

void CAIFlyingEnemy::OnGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	transform = getMyTransform();
	spawnPosition = transform->getPosition();
	player = (CEntity *)getEntityByName("The Player");
	playerTransform = player->get<TCompTransform>();
	collider = get<TCompCollider>();
}

void CAIFlyingEnemy::debugInMenu() {
	IAIController::debugInMenu();
}

void CAIFlyingEnemy::IdleState(float delta) {
}

boolean CAIFlyingEnemy::IsPlayerInAttackRange() {
	float distance = VEC3::Distance(transform->getPosition(), playerTransform->getPosition());
	return distance < startAttackingRadius && transform->isInFov(playerTransform->getPosition(), chaseFov);
}

boolean CAIFlyingEnemy::IsPlayerInFov() {
	float distance = VEC3::Distance(transform->getPosition(), playerTransform->getPosition());
	return distance < smallChaseRadius || (distance < fovChaseDistance && transform->isInFov(playerTransform->getPosition(), attackFov));
}

void  CAIFlyingEnemy::DeathState(float delta) {
	TCompCollider *collider = get<TCompCollider>();
	collider->disable();
	CHandle(this).getOwner().destroy();
}

void CAIFlyingEnemy::GrabbedState(float delta) {
	dbg("En grabbed\n");
}

void CAIFlyingEnemy::PropelledState(float delta) {
	dbg("On propelled\n");
	//Si no ha pasado el timer
	//mover propelVelocityVector * delta
}
