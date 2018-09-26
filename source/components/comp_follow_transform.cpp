#include "mcv_platform.h"
#include "comp_follow_transform.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("follow_transform", TCompFollowTransform);

void TCompFollowTransform::debugInMenu() {
	ImGui::DragFloat3("offset", &offset.x, 0.5f, -1000.f, 1000.f);

}

void TCompFollowTransform::load(const json& j, TEntityParseContext& ctx) {
	entityName = j.value("entityName", "");
	offset = loadVEC3(j["offset"]);
}

void TCompFollowTransform::registerMsgs() {
	DECL_MSG(TCompFollowTransform, TMsgAllScenesCreated, onAllScenesCreated);
}


void TCompFollowTransform::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	CEntity* otherEntity = getEntityByName(entityName);
	followingTransformHandle = otherEntity->get<TCompTransform>();
	transformHandle = get<TCompTransform>();
}


void TCompFollowTransform::update(float dt) {
	getTransform()->setPosition(getFollowingTransform()->getPosition() + offset);
}
