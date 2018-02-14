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
	DECL_MSG(TCompPlayerModel, TMsgEntitiesGroupCreated, OnGroupCreated);
}

void TCompPlayerModel::OnGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	myTransform = get<TCompTransform>();

	CEntity *camera = (CEntity *)getEntityByName("xthe_camera");
	currentCamera = camera->get<TCompCamera>();
	assert(currentCamera);
}

void TCompPlayerModel::update(float dt) {
}

//Aquí llega sin normalizar, se debe hacer justo antes de aplicar el movimiento si se quiere que pueda caminar
void TCompPlayerModel::SetTranslationInput(VEC2 input, float delta) {
	VEC3 desiredDirection = currentCamera->TransformToWorld(input);
	VEC3 targetPos = myTransform->getPosition() + desiredDirection * speedFactor * delta;

	if (input.x != 0 && abs(myTransform->getDeltaYawToAimTo(targetPos)) > 0.01f) {
		float y, p, r;
		myTransform->getYawPitchRoll(&y, &p, &r);
		if (myTransform->isInLeft(targetPos)) {
			y += rotationSpeed *delta;
		}
		else {
			y -= rotationSpeed *delta;
		}
		myTransform->setYawPitchRoll(y, p, r);
	}

	if (input.y != 0) {
		myTransform->setPosition(myTransform->getPosition() + myTransform->getFront() * speedFactor * delta);
	}
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

