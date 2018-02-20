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
}

void TCompGroundSensor::onCreate(const TMsgEntityCreated& msg) {
	//TCompCollider::onCreate(msg);
}

void TCompGroundSensor::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	
}

void TCompGroundSensor::load(const json& j, TEntityParseContext& ctx) {
	TCompCollider::load(j, ctx);
	config.is_trigger = true;
}

void TCompGroundSensor::update(float dt) {
	
}

