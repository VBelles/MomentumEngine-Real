#include "mcv_platform.h"
#include "comp_dummy_collectable.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "components/comp_collectable.h"
#include "components/controllers/comp_rigid_anims_director.h"
#include "components/player/attack_info.h"


DECL_OBJ_MANAGER("dummy_collectable", TCompDummyCollectable);

void TCompDummyCollectable::debugInMenu() {
	ImGui::DragFloat("crysalis starting scale", &chrysalisStartingScale, 0.05f, 0.1f, 2.f);
	ImGui::DragFloat("powerup starting scale", &powerupStartingScale, 0.05f, 0.1f, 2.f);
	ImGui::DragFloat("life piece starting scale", &lifePieceStartingScale, 0.05f, 0.1f, 2.f);
	ImGui::DragFloat("time to start rotation", &timeToStartRotation, 0.01f, 0.f, 2.f);
	ImGui::DragFloat("time to start scaling", &timeToStartScaling, 0.01f, 0.f, 2.f);
	ImGui::DragFloat("rotation acceleration", &rotationAcceleration, 0.05f, 0.1f, 20.f);
	ImGui::DragFloat("max rotation speed", &maxRotationSpeed, 0.05f, 0.1f, 20.f);
	ImGui::DragFloat("time to get to scale zero", &timeToScaleZero, 0.05f, 0.1f, 2.f);
	ImGui::DragFloat3("position offset", &positionOffset.x, 0.05f, 0.f, 2.f);
}

void TCompDummyCollectable::registerMsgs() {
	DECL_MSG(TCompDummyCollectable, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompDummyCollectable::load(const json& j, TEntityParseContext& ctx) {
	chrysalisName = j.value("chrysalis", "");
	powerupName = j.value("powerup", "");
	lifePieceName = j.value("life_piece", "");
}

void TCompDummyCollectable::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	playerTransformHandle = get<TCompTransform>();
	assert(playerTransformHandle.isValid());

	chrysalisHandle = getEntityByName(chrysalisName);
	assert(chrysalisHandle.isValid());
	powerupHandle = getEntityByName(powerupName);
	assert(powerupHandle.isValid());
	lifePieceHandle = getEntityByName(lifePieceName);
	assert(lifePieceHandle.isValid());
	currentCollectableHandle = powerupHandle;
}

void TCompDummyCollectable::update(float delta) {
	if (!isActive) return;

}

void TCompDummyCollectable::activateSequence(DummyCollectableType type) {
	switch (type) {
	case CHRYSALIS:
		currentCollectableHandle = chrysalisHandle;
		break;
	case POWERUP:
		currentCollectableHandle = powerupHandle;
		break;
	case LIFEPIECE:
		currentCollectableHandle = lifePieceHandle;
		break;
	}
	timer.reset();
	isActive = true;
}
