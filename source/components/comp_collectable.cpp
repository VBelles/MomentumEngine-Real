#include "mcv_platform.h"
#include "comp_collectable.h"
#include "entity/common_msgs.h"
#include "modules/system_modules/particles/comp_particles.h"

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
	helpDistance = j.value("help_distance", helpDistance);
}

void TCompCollectable::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	transformHandle = get<TCompTransform>();
	assert(transformHandle.isValid());

	std::string name = ((CEntity*)CHandle(this).getOwner())->getName();
	UniqueElement* uniqueElement = EngineUniques.getUniqueElement(type, name);
	if (uniqueElement && uniqueElement->done) {
		((TCompCollider*)get<TCompCollider>())->destroy();
	}
}

void TCompCollectable::update(float delta) {
	if (abs(rotationSpeed) > 0) {
		TCompTransform* transform = getTransform();
		QUAT quat = QUAT::CreateFromAxisAngle(rotationAxis, rotationSpeed * delta);
		transform->setRotation(transform->getRotation() * quat);
	}


	if (!playerHandle.isValid()) {
		playerHandle = getEntityByName(PLAYER_NAME);
	}
	CEntity* player = playerHandle;
	TCompTransform* playerTransform = player->get<TCompTransform>();
	VEC2 playerPos = { playerTransform->getPosition().x, playerTransform->getPosition().z };
	VEC2 pos = { getTransform()->getPosition().x, getTransform()->getPosition().z };
	if (helpDistance >= 0.f && VEC2::Distance(playerPos, pos) > helpDistance) {
		TCompParticles* particles = get<TCompParticles>();
		if (particles && nearEffectLaunched) {
			particles->kill("help");
			nearEffectLaunched = false;
		}
	}
	else if (helpDistance >= 0.f) {
		TCompParticles* particles = get<TCompParticles>();
		if (particles && !nearEffectLaunched) {
			nearEffectLaunched = true;
			particles->launch("help");
		}
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
		TCompParticles* particles = get<TCompParticles>();
		if (particles) {
			particles->kill();
		}
		collected = true;
		((TCompCollider*)get<TCompCollider>())->destroy();
	}
}

void TCompCollectable::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	CHandle(this).getOwner().destroy();
}

ElementType TCompCollectable::getTypeByName(std::string name) {
	auto it = typeByName.find(name);
	if (it != typeByName.end()) {
		return typeByName[name];
	}
	return UNDEFINED;
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
