#include "mcv_platform.h"
#include "comp_forward_msg.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("fwd_msg", TCompFwdMsg);

void TCompFwdMsg::debugInMenu() {
}

void TCompFwdMsg::registerMsgs() {
	DECL_MSG(TCompFwdMsg, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompFwdMsg, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompFwdMsg, TMsgTriggerExit, onTriggerExit);
}

void TCompFwdMsg::load(const json& j, TEntityParseContext& ctx) {
    std::string targetName = j.value("target", "");
    if (targetName != "") {
        target = (CHandle)getEntityByName(targetName);
    }
}

void TCompFwdMsg::onGroupCreated(const TMsgEntitiesGroupCreated & msg) { 
}

void TCompFwdMsg::onTriggerEnter(const TMsgTriggerEnter & msg) {
    if (target.isValid()) {
        target.sendMsg(msg);
    }
}

void TCompFwdMsg::onTriggerExit(const TMsgTriggerExit & msg) {
    if (target.isValid()) {
        target.sendMsg(msg);
    }
}
