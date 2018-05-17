#include "mcv_platform.h"
#include "comp_ranged_attack.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("ranged_attack", TCompRangedAttack);

void TCompRangedAttack::debugInMenu() {
	ImGui::DragFloat("Speed", &speed, 0.1f, 0.f, 20.f);
}

void TCompRangedAttack::registerMsgs() {
	DECL_MSG(TCompRangedAttack, TMsgEntityCreated, onCreate);
	DECL_MSG(TCompRangedAttack, TMsgAssignRangedAttackOwner, onAssignRangedAttackOwner);
	DECL_MSG(TCompRangedAttack, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompRangedAttack, TMsgColliderDestroyed, onColliderDestroyed);
}

void TCompRangedAttack::load(const json& j, TEntityParseContext& ctx) {
	speed = j.value("speed", 10.f);
	lifetime = j.value("lifetime", 30.0f);
}

void TCompRangedAttack::update(float delta) {
	if (!hit) {
		TCompCollider* collider = get<TCompCollider>();
		if (timer.elapsed() > lifetime) {
			collider->destroy();
			hit = true;
		}
		else {
			TCompTransform* transform = get<TCompTransform>();
			PxRigidDynamic* rigidDynamic = (PxRigidDynamic*)collider->actor;
			transform->setPosition(transform->getPosition() + transform->getFront() * speed * delta);
			rigidDynamic->setKinematicTarget(toPxTransform(transform));
		}
	}
}

void TCompRangedAttack::onCreate(const TMsgEntityCreated& msg) {
	TCompCollider* collider = get<TCompCollider>();
	PxRigidDynamic* rigidDynamic = (PxRigidDynamic*)collider->actor;
	rigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
}

void TCompRangedAttack::onAssignRangedAttackOwner(const TMsgAssignRangedAttackOwner& msg) {
	ownerHandle = msg.ownerHandle;
	attackInfo = msg.attackInfo;
	TCompTransform *transform = get<TCompTransform>();
	transform->lookAt(msg.initialPos, msg.initialPos + msg.direction);
	timer.reset();
}

void TCompRangedAttack::onTriggerEnter(const TMsgTriggerEnter& msg) {
	if (msg.h_other_entity != ownerHandle) {
		TCompCollider* collider = get<TCompCollider>();
		collider->destroy();

		AttackInfo msgAttackInfo = attackInfo;
		CEntity *otherEntity = msg.h_other_entity;
		otherEntity->sendMsg(TMsgAttackHit{ ownerHandle, msgAttackInfo });

		hit = true;
	}
}

void TCompRangedAttack::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	if (hit) {
		CHandle(this).getOwner().destroy();
	}
}