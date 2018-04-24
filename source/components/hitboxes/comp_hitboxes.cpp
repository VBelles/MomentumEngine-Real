#include "mcv_platform.h"
#include "comp_hitboxes.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"

DECL_OBJ_MANAGER("hitboxes", TCompHitboxes);

void TCompHitboxes::debugInMenu() {
}

void TCompHitboxes::registerMsgs() {
	DECL_MSG(TCompHitboxes, TMsgEntityCreated, onCreate);
	DECL_MSG(TCompHitboxes, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompHitboxes, TMsgTriggerEnter, onTriggerEnter);
}

void TCompHitboxes::onCreate(const TMsgEntityCreated& msg) {
	
}

void TCompHitboxes::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {

}

void TCompHitboxes::load(const json& j, TEntityParseContext& ctx) {
	if (j.count("offset")) {
		offset = loadVEC3(j["offset"]);
	}
}

void TCompHitboxes::update(float dt) {
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

void TCompHitboxes::onTriggerEnter(const TMsgTriggerEnter& msg) {
	if (handles.find(msg.h_other_entity) == handles.end()) {
		handles.insert(msg.h_other_entity);
		CEntity *owner = CHandle(this).getOwner();
		TCompHierarchy *hierarchy = owner->get<TCompHierarchy>();
		CEntity *parent = hierarchy->h_parent;
		parent->sendMsg(TMsgHitboxEnter{ msg.h_other_entity });
	}
}

void TCompHitboxes::disable() {
	TCompCollider* collider = get<TCompCollider>();
	if (collider->isCreated()) {
		collider->destroy();
	}
}

void TCompHitboxes::enable() {
	TCompCollider* collider = get<TCompCollider>();
	if (!collider->isCreated()) {
		handles.clear();

		collider->create();

		PxRigidDynamic *rigidDynamic = (PxRigidDynamic*)collider->actor;
		rigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	}
}

VEC3 TCompHitboxes::getOffset() {
	return offset;
}

void TCompHitboxes::setOffset(VEC3 newOffset) {
	offset = newOffset;
}