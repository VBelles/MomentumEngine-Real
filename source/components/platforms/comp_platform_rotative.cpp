#include "mcv_platform.h"
#include "comp_platform_rotative.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("platform_rotative", TCompPlatformRotative);

void TCompPlatformRotative::registerMsgs() {
	DECL_MSG(TCompPlatformRotative, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompPlatformRotative::load(const json& j, TEntityParseContext& ctx) {
	if (j.count("rotation_speed")) {
		rotationSpeed = loadVEC3(j["rotation_speed"]) * (float)M_PI / 180.f;
	}
}

void TCompPlatformRotative::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	colliderHandle = get<TCompCollider>();
	assert(colliderHandle.isValid());
	transformHandle = get<TCompTransform>();
	assert(transformHandle.isValid());
	getRigidDynamic()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
}


void TCompPlatformRotative::update(float delta) {
	TCompTransform* transform = getTransform();
	VEC3 yawPitchRoll;
	transform->getYawPitchRoll(&yawPitchRoll.x, &yawPitchRoll.y, &yawPitchRoll.z);
	yawPitchRoll += rotationSpeed * delta;
	transform->setYawPitchRoll(yawPitchRoll.x, yawPitchRoll.y, yawPitchRoll.z);
	getRigidDynamic()->setKinematicTarget(toPhysx(transform));
}

TCompCollider* TCompPlatformRotative::getCollider() {
	return colliderHandle;
}

TCompTransform* TCompPlatformRotative::getTransform() {
	return transformHandle;
}

PxRigidDynamic* TCompPlatformRotative::getRigidDynamic() {
	return static_cast<PxRigidDynamic*>(getCollider()->actor);
}
