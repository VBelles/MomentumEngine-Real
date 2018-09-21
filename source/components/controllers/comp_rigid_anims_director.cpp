#include "mcv_platform.h"
#include "comp_rigid_anims_director.h"
#include "components/comp_transform.h"
#include "entity/entity_parser.h"
#include "components/controllers/comp_rigid_anim.h"

DECL_OBJ_MANAGER("rigid_anims_director", TCompRigidAnimsDirector);

void TCompRigidAnimsDirector::debugInMenu() {
}

void TCompRigidAnimsDirector::load(const json& j, TEntityParseContext& ctx) {
	if (j.count("animated_entities")) {
		auto& entities = j["animated_entities"];
		assert(entities.is_array());
		for (std::string entity : entities) {
			animatedEntitiesNames.push_back(entity);
		}
	}
	isMoving = j.value("is_moving", isMoving);
}


void TCompRigidAnimsDirector::registerMsgs() {
	DECL_MSG(TCompRigidAnimsDirector, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompRigidAnimsDirector::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	for (std::string name : animatedEntitiesNames) {
		CHandle entity = getEntityByName(name);
		if (entity.isValid()) {
			animatedEntities.push_back(entity);
		}
	}

	setIsMoving(isMoving);
}


void TCompRigidAnimsDirector::setIsMoving(bool isMoving) {
	this->isMoving = isMoving;
	for (CEntity* entity : animatedEntities) {
		TCompRigidAnim* anim = entity->get<TCompRigidAnim>();
		if (anim) {
			anim->setIsMoving(isMoving);
		}
	}
}

void TCompRigidAnimsDirector::setStopOnNextLoop(bool stop) {
	isMoving = !stop;
	for (CEntity* entity : animatedEntities) {
		TCompRigidAnim* anim = entity->get<TCompRigidAnim>();
		if (anim) {
			anim->setStopOnNextLoop(stop);
		}
	}
}
