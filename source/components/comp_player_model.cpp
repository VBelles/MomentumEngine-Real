#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_model.h"


DECL_OBJ_MANAGER("player_model", TCompPlayerModel);

void TCompPlayerModel::debugInMenu() {
	ImGui::DragFloat("Speed", &speedFactor, 0.1f, 0.f, 20.f);
	ImGui::DragFloat("Rotation", &rotationSpeed, 0.1f, 0.f, 20.f);
}

void TCompPlayerModel::load(const json& j, TEntityParseContext& ctx) {
	speedFactor = j.value("speed", 2.0f);
	rotationSpeed = j.value("rotation_speed", 2.0f);
}


void TCompPlayerModel::registerMsgs() {
	DECL_MSG(TCompPlayerModel, TMsgEntityCreated, onCreate);
}

void TCompPlayerModel::onCreate(const TMsgEntityCreated& msg) {
	myTransform = get<TCompTransform>();

	CEntity *camera = (CEntity *)getEntityByName("xthe_camera");
	currentCamera = camera->get<TCompCamera>();
	assert(currentCamera);


}

void TCompPlayerModel::update(float dt) {
	dbg("Position: (%f, %f, %f)", currentCamera->getPosition().x, currentCamera->getPosition().y, currentCamera->getPosition().z);
}

void TCompPlayerModel::SetTranslationInput(VEC2 input, float delta) {//Aqu� llega sin normalizar, se debe hacer justo antes de aplicar el movimiento si se quiere que pueda caminar
	input.Normalize();
	VEC3 myNewPos = myTransform->getPosition();
	myNewPos += myTransform->getFront() * input.y * speedFactor * delta;
	myNewPos -= myTransform->getLeft() * input.x * speedFactor * delta;

	myTransform->setPosition(myNewPos);
}

void TCompPlayerModel::SetRotationInput(float input, float delta) {
	TCompTransform *myTransform = get<TCompTransform>();

	float amountRotated = rotationSpeed * delta;
	float currentYaw = 0.f;
	float currentPitch = 0.f;
	myTransform->getYawPitchRoll(&currentYaw, &currentPitch);
	currentYaw += amountRotated * input;
	myTransform->setYawPitchRoll(currentYaw, currentPitch, 0.f);
}

