#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_model.h"
#include "comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("player_model", TCompPlayerModel);

void TCompPlayerModel::debugInMenu() {
	ImGui::DragFloat("Speed", &speedFactor, 0.1f, 0.f, 20.f);
	ImGui::DragFloat("Rotation", &rotationSpeed, 0.1f, 0.f, 20.f);
}

void TCompPlayerModel::load(const json& j, TEntityParseContext& ctx) {
	speedFactor = j.value("speed", 2.0f);
	rotationSpeed = j.value("rotation_speed", 2.0f);
}

void TCompPlayerModel::update(float dt) {
  

}

void TCompPlayerModel::SetTranslationInput(VEC2 input, float delta) {//Aquí llega sin normalizar, se debe hacer justo antes de aplicar el movimiento si se quiere que pueda caminar
	//Guardo mi transform
	TCompTransform *myTransform = get<TCompTransform>();

	//Pongo a cero la velocidad actual
	float amountMoved = speedFactor * delta;
	
	//Detecto el teclado
	VEC3 localSpeed = VEC3::Zero;
	input.Normalize();
	localSpeed.x = input.x;
	localSpeed.z = input.y;
	

	// Using TransformNormal because I just want to rotate
	VEC3 world_speed = VEC3::TransformNormal(localSpeed, myTransform->asMatrix());
	// Guardo la y de la posicion y le sumo la nueva posicion a la x y a la z
	VEC3 my_new_pos = myTransform->getPosition() + world_speed * amountMoved;

	//Actualizo la posicion del transform
	myTransform->setPosition(my_new_pos);
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

