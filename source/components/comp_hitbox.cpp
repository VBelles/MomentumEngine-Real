#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_hitbox.h"
#include "comp_transform.h"
#include "comp_hierarchy.h"
#include "components/comp_collider.h"

DECL_OBJ_MANAGER("hitbox", TCompHitbox);

void TCompHitbox::debugInMenu() {
}

void TCompHitbox::registerMsgs() {
	DECL_MSG(TCompHitbox, TMsgEntityCreated, onCreate);
	DECL_MSG(TCompHitbox, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompHitbox, TMsgTriggerEnter, onTriggerEnter);
}

void TCompHitbox::onCreate(const TMsgEntityCreated& msg) {
	disable();
}

void TCompHitbox::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {

}

void TCompHitbox::load(const json& j, TEntityParseContext& ctx) {
	if (j.count("offset")) {
		offset = loadVEC3(j["offset"]);
	}
}

void TCompHitbox::update(float dt) {
	TCompCollider *collider = get<TCompCollider>();
	if (collider->isCreated()) {
		TCompTransform *transform = get<TCompTransform>();
		PxRigidDynamic *rigidDynamic = (PxRigidDynamic*)collider->actor;

		VEC3 front = transform->getFront();
		VEC3 right = -transform->getLeft();
		front.y = 0.f;
		right.y = 0.f;
		front.Normalize();
		right.Normalize();
		VEC3 desiredDirection = front * offset.z + right * offset.x;

		rigidDynamic->setKinematicTarget({ transform->getPosition().x + desiredDirection.x, transform->getPosition().y + offset.y, transform->getPosition().z + desiredDirection.z });
	}
}

void TCompHitbox::onTriggerEnter(const TMsgTriggerEnter& msg) {
	if (handles.find(msg.h_other_entity) == handles.end()) {
		handles.insert(msg.h_other_entity);
		CEntity *owner = CHandle(this).getOwner();
		TCompHierarchy *hierarchy = owner->get<TCompHierarchy>();
		CEntity *parent = hierarchy->h_parent;
		parent->sendMsg(TMsgHitboxEnter{"", msg.h_other_entity });
	}
}

void TCompHitbox::disable() {
	TCompCollider* collider = get<TCompCollider>();
	if (collider->isCreated()) {
		collider->destroy();
	}
}

void TCompHitbox::enable() {
	TCompCollider* collider = get<TCompCollider>();
	if (!collider->isCreated()) {
		handles.clear();

		collider->create();

		PxRigidDynamic *rigidDynamic = (PxRigidDynamic*)collider->actor;
		rigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	}
}

VEC3 TCompHitbox::getOffset() {
	return offset;
}

void TCompHitbox::setOffset(VEC3 newOffset) {
	offset = newOffset;
}