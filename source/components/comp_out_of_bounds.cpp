#include "mcv_platform.h"
#include "comp_out_of_bounds.h"

DECL_OBJ_MANAGER("outOfBounds", TComputOfBounds);

void TComputOfBounds::debugInMenu() {
}

void TComputOfBounds::registerMsgs() {
	DECL_MSG(TComputOfBounds, TMsgTriggerEnter, onTriggerEnter);
}

void TComputOfBounds::load(const json& j, TEntityParseContext& ctx) {
}

void TComputOfBounds::update(float dt) {
}

void TComputOfBounds::onTriggerEnter(const TMsgTriggerEnter& msg) {
	CEntity *otherEntity = msg.h_other_entity;
	otherEntity->sendMsg(TMsgOutOfBounds{CHandle(this)});
}