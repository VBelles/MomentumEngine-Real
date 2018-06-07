#include "mcv_platform.h"
#include "comp_slash.h"
#include "components/comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("slash", TCompSlash);

void TCompSlash::debugInMenu() {
	ImGui::Checkbox("Enabled", &enabled);
}

void TCompSlash::registerMsgs() {
	DECL_MSG(TCompSlash, TMsgAllScenesCreated, onAllScenesCreated);
}

void TCompSlash::load(const json& j, TEntityParseContext& ctx) {
	setEnable(j.value("enabled", false));
	targetName = j.value("target", "");
}

void TCompSlash::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	CEntity* entity = getEntityByName(targetName);
	assert(entity);
	targetTransformHandle = entity->get<TCompTransform>();
	assert(targetTransformHandle.isValid());

}

void TCompSlash::update(float delta) {

}


void TCompSlash::setEnable(bool enabled) {
	this->enabled = enabled;
}

TCompTransform* TCompSlash::getTargetTransform() {
	return targetTransformHandle;
}
