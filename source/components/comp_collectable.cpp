#include "mcv_platform.h"
#include "comp_collectable.h"
#include "components/comp_transform.h"
#include "components/comp_render.h"
#include "components/comp_collider.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("collectable", TCompCollectable);

void TCompCollectable::debugInMenu() {
	ImGui::DragFloat("selfIllumVariationHalfAmplitude", &selfIllumVariationHalfAmplitude, 0.01f, 0, 0.5f);
	ImGui::DragFloat("selfIllumVariationTimeFactor", &selfIllumVariationTimeFactor, 0.1f, 0, 20.f);
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
	hasSelfIllumVariation = j.value("has_self_illum_variation", false);
	selfIllumVariationHalfAmplitude = j.value("self_illum_variation_half_amplitude", 0.15f);
	selfIllumVariationTimeFactor = j.value("self_illum_variation_time_factor", 3.3f);

}

void TCompCollectable::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	timer.reset();
	transformHandle = get<TCompTransform>();
	assert(transformHandle.isValid());
}

void TCompCollectable::update(float delta) {
	if (abs(rotationSpeed) > 0) {
		TCompTransform* transform = getTransform();
		QUAT quat = QUAT::CreateFromAxisAngle(rotationAxis, rotationSpeed * delta);	
		transform->setRotation(transform->getRotation() * quat);
	}
	if (hasSelfIllumVariation) {
		TCompRender* render = get<TCompRender>();
		if(render)
			render->selfIllumRatio = (1 - selfIllumVariationHalfAmplitude) + selfIllumVariationHalfAmplitude * sin(timer.elapsed() * selfIllumVariationTimeFactor);
	}
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

TCompTransform * TCompCollectable::getTransform() {
	return transformHandle;
}
