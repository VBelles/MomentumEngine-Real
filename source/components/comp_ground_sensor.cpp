#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_ground_sensor.h"
#include "comp_transform.h"

DECL_OBJ_MANAGER("ground_sensor", TCompGroundSensor);

void TCompGroundSensor::debugInMenu() {
}

void TCompGroundSensor::registerMsgs() {
	DECL_MSG(TCompGroundSensor, TMsgEntityCreated, onCreate);
	DECL_MSG(TCompGroundSensor, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompGroundSensor, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompGroundSensor, TMsgTriggerExit, onTriggerExit);
}

void TCompGroundSensor::onCreate(const TMsgEntityCreated& msg) {
}

void TCompGroundSensor::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	TCompCollider *collider = get<TCompCollider>();
	PxRigidDynamic *rigidDynamic = (PxRigidDynamic*)collider->actor;
	rigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
}

void TCompGroundSensor::load(const json& j, TEntityParseContext& ctx) {
}

void TCompGroundSensor::update(float dt) {
	TCompCollider *collider = get<TCompCollider>();
	TCompTransform *transform = get<TCompTransform>();
	PxRigidDynamic *rigidDynamic = (PxRigidDynamic*)collider->actor;
	rigidDynamic->setKinematicTarget({ transform->getPosition().x, transform->getPosition().y, transform->getPosition().z });
}

void TCompGroundSensor::onTriggerEnter(const TMsgTriggerEnter& msg) {
	dbg("Trigger enter\n");
}

void TCompGroundSensor::onTriggerExit(const TMsgTriggerExit& msg) {
	dbg("Trigger exit\n");
}