#include "mcv_platform.h"
#include "comp_collectable.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("collectable", TCompCollectable);

void TCompCollectable::debugInMenu() {
	ImGui::Checkbox("Active", &active);
}

void TCompCollectable::registerMsgs() {
	DECL_MSG(TCompCollectable, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompCollectable, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompCollectable, TMsgColliderDestroyed, onColliderDestroyed);
}

void TCompCollectable::load(const json& j, TEntityParseContext& ctx) {
	std::string typeName = j.value("type", "undefined");
	type = getTypeByName(typeName);
	rotationSpeed = j.value("rotation_speed", 0.f);
	if (j.count("rotation_axis")) {
		rotationAxis = loadVEC3(j["rotation_axis"]);
	}
	active = j.value("active", active);
}

void TCompCollectable::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	transformHandle = get<TCompTransform>();
	assert(transformHandle.isValid());
}

void TCompCollectable::update(float delta) {
	if (abs(rotationSpeed) > 0) {
		TCompTransform* transform = getTransform();
		QUAT quat = QUAT::CreateFromAxisAngle(rotationAxis, rotationSpeed * delta);
		transform->setRotation(transform->getRotation() * quat);
	}
}

void TCompCollectable::onTriggerEnter(const TMsgTriggerEnter & msg) {
	if (!collected && active) {
		CEntity* collector = msg.h_other_entity;
		TMsgCollect msg{ CHandle(this), type };
		collector->sendMsg(msg);
	}
}

void TCompCollectable::collect() {
	if (!collected && active) {
		collected = true;
		((TCompCollider*)get<TCompCollider>())->destroy();
	}
}

void TCompCollectable::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	CHandle(this).getOwner().destroy();
}

TCompCollectable::Type TCompCollectable::getTypeByName(std::string name) {
	auto it = typeByName.find(name);
	if (it != typeByName.end()) {
		return typeByName[name];
	}
	return Type::UNDEFINED;
}

void TCompCollectable::setActive(bool active) {
	this->active = active;
}

bool TCompCollectable::isActive() {
	return active;
}

TCompTransform * TCompCollectable::getTransform() {
	return transformHandle;
}
