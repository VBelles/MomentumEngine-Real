#include "mcv_platform.h"
#include "comp_dummy_collectable.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"

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
	ImGui::DragFloat3("chrysalis position offset", &chrysalisPositionOffset.x, 0.05f, 0.f, 2.f);
	ImGui::DragFloat3("powerup position offset", &powerupPositionOffset.x, 0.05f, 0.f, 2.f);
	ImGui::DragFloat3("lifePiece position offset", &lifePiecePositionOffset.x, 0.05f, 0.f, 2.f);
	ImGui::DragFloat("chrysalis yaw offset", &chrysalisYawOffset, 0.001f, -3.14f, 3.14f);
	ImGui::DragFloat("powerup yaw offset", &powerupYawOffset, 0.001f, -3.14f, 3.14f);
	ImGui::DragFloat("life piece yaw offset", &lifePieceYawOffset, 0.001f, -3.14f, 3.14f);
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
	if (timer.elapsed() >= timeToStartRotation) {

	}
	if (timer.elapsed() >= timeToStartScaling + timeToScaleZero) {
		CEntity* entity = currentCollectableHandle;
		//desactivar render
		TCompRender* render = entity->get<TCompRender>();
		render->disable();
		isActive = false;
	}
}

void TCompDummyCollectable::activateSequence(DummyCollectableType type) {
	float scale = chrysalisStartingScale;
	VEC3 positionOffset = chrysalisPositionOffset;
	float yawOffset = chrysalisYawOffset;
	switch (type) {
	case CHRYSALIS:
		currentCollectableHandle = chrysalisHandle;
		break;
	case POWERUP:
		currentCollectableHandle = powerupHandle;
		scale = powerupStartingScale;
		positionOffset = powerupPositionOffset;
		yawOffset = powerupYawOffset;
		break;
	case LIFEPIECE:
		currentCollectableHandle = lifePieceHandle;
		scale = lifePieceStartingScale;
		positionOffset = lifePiecePositionOffset;
		yawOffset = lifePieceYawOffset;
		break;
	}
	CEntity* entity = currentCollectableHandle;
	//activar render
	TCompRender* render = entity->get<TCompRender>();
	render->enable();
	//situar delante de player
	TCompTransform* transform = entity->get<TCompTransform>();
	VEC3 position = getPlayerTransform()->getPosition() +
		getPlayerTransform()->getFront() * positionOffset.z +
		VEC3::Up * positionOffset.y +
		getPlayerTransform()->getLeft() * positionOffset.x;
	transform->setPosition(position);
	float yaw, pitch;
	getPlayerTransform()->getYawPitchRoll(&yaw, &pitch);
	transform->setYawPitchRoll(yaw + yawOffset, pitch);
	transform->setScale(scale);
	timer.reset();
	isActive = true;
}
