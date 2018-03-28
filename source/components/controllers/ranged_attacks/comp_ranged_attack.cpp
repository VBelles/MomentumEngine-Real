#include "mcv_platform.h"
#include "comp_ranged_attack.h"
#include "entity/entity_parser.h"
#include "components\comp_transform.h"

DECL_OBJ_MANAGER("ranged_attack", TCompRangedAttack);

void TCompRangedAttack::debugInMenu() {
	ImGui::DragFloat("Speed", &speed, 0.1f, 0.f, 20.f);
}

void TCompRangedAttack::registerMsgs() {
	DECL_MSG(TCompRangedAttack, TMsgEntityCreated, onCreate);
	DECL_MSG(TCompRangedAttack, TMsgAssignRangedAttackOwner, onAssignRangedAttackOwner);
	DECL_MSG(TCompRangedAttack, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompRangedAttack, TMsgColliderDestroyed, OnColliderDestroyed);
}

void TCompRangedAttack::load(const json& j, TEntityParseContext& ctx) {
	speed = j.value("speed", 0.01f);
	lifetime = j.value("lifetime", 60.0f);
}

void TCompRangedAttack::update(float dt) {
	if (!hit) {
		TCompCollider* collider = get<TCompCollider>();
		if (timer.elapsed() > lifetime) {
			collider->disable();
			hit = true;
		}
		else {
			TCompTransform *transform = get<TCompTransform>();
			PxRigidDynamic *rigidDynamic = (PxRigidDynamic*)collider->actor;
			VEC3 newPos = transform->getPosition() + transform->getFront() * speed;
			PxTransform newTransform;
			newTransform.p = { newPos.x, newPos.y, newPos.z };
			newTransform.q = { transform->getRotation().x, transform->getRotation().y, transform->getRotation().z, transform->getRotation().w };
			rigidDynamic->setKinematicTarget(newTransform);
		}
	}
}

void TCompRangedAttack::onCreate(const TMsgEntityCreated& msg) {
	TCompCollider* collider = get<TCompCollider>();
	PxRigidDynamic *rigidDynamic = (PxRigidDynamic*)collider->actor;
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
		collider->disable();

		CEntity *otherEntity = msg.h_other_entity;
		otherEntity->sendMsg(TMsgAttackHit{ ownerHandle, attackInfo });

		attackInfo.release();
		hit = true;
	}
}

void TCompRangedAttack::OnColliderDestroyed(const TMsgColliderDestroyed& msg) {
	if (hit) {
		CHandle(this).getOwner().destroy();
	}
}