#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_hitbox.h"
#include "comp_transform.h"

DECL_OBJ_MANAGER("hitbox", TCompHitbox);

void TCompHitbox::debugInMenu() {
}

void TCompHitbox::registerMsgs() {
	DECL_MSG(TCompHitbox, TMsgEntityCreated, onCreate);
	DECL_MSG(TCompHitbox, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompHitbox, TMsgTriggerEnter, onTriggerEnter);
}

void TCompHitbox::onCreate(const TMsgEntityCreated& msg) {
}

void TCompHitbox::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	disable();
}

void TCompHitbox::load(const json& j, TEntityParseContext& ctx) {
	if (j.count("offset")) {
		offset = loadVEC3(j["offset"]);
	}
}

void TCompHitbox::update(float dt) {
	if (enabled) {
		TCompCollider *collider = get<TCompCollider>();
		TCompTransform *transform = get<TCompTransform>();
		PxRigidDynamic *rigidDynamic = (PxRigidDynamic*)collider->actor;

		VEC3 front = transform->getFront();
		VEC3 right = -transform->getLeft();
		front.y = 0.f;
		right.y = 0.f;
		front.Normalize();
		right.Normalize();
		VEC3 desiredDirection = front * offset.x + right * offset.z;

		rigidDynamic->setKinematicTarget({ transform->getPosition().x + desiredDirection.x, transform->getPosition().y + offset.y, transform->getPosition().z + desiredDirection.z });
	}

	if (EngineInput["release_power"].getsPressed()) {
		enable();
	}
	if (EngineInput["gain_power"].getsPressed()) {
		disable();
	}
}

void TCompHitbox::onTriggerEnter(const TMsgTriggerEnter& msg) {
	if (handles.find(msg.h_other_entity) == handles.end()) {
		handles.insert(msg.h_other_entity);
		CEntity *owner = CHandle(this).getOwner();
		owner->sendMsg(TMsgTriggerEnter{ msg.h_other_entity });
		dbg("Trigger enter\n");
	}
}

void TCompHitbox::disable() {
	if (enabled) {
		enabled = false;
		TCompCollider *collider = get<TCompCollider>();
		collider->actor->release();
	}
}

void TCompHitbox::enable() {
	if (!enabled) {
		handles.clear();
		enabled = true;

		TCompCollider *collider = get<TCompCollider>();

		Engine.getPhysics().createActor(*collider);

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