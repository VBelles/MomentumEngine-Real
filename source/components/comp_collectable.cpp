#include "mcv_platform.h"
#include "comp_collectable.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("collectable", TCompCollectable);

void TCompCollectable::debugInMenu() {
}

void TCompCollectable::registerMsgs() {
	DECL_MSG(TCompCollectable, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompCollectable, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompCollectable, TMsgColliderDestroyed, OnColliderDestroyed);
}

void TCompCollectable::load(const json& j, TEntityParseContext& ctx) {
	std::string typeName = j.value("type", "undefined");
	type = getTypeByName(typeName);
}

void TCompCollectable::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
}

void TCompCollectable::update(float dt) {
}

void TCompCollectable::onTriggerEnter(const TMsgTriggerEnter & msg) {
	if (!collected) {
		CEntity* collector = msg.h_other_entity;
		TMsgCollect msg{ CHandle(this), type };
		collector->sendMsg(msg);
	}
}

void TCompCollectable::collect() {
	if (!collected) {
		collected = true;
		getComp<TCompCollider>()->destroy();
	}

}

void TCompCollectable::OnColliderDestroyed(const TMsgColliderDestroyed& msg) {
	CHandle(this).getOwner().destroy();
}

TCompCollectable::Type TCompCollectable::getTypeByName(std::string name) {
	auto it = typeByName.find(name);
	if (it != typeByName.end()) {
		return typeByName[name];
	}
	return Type::UNDEFINED;
}