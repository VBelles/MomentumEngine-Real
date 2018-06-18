#include "mcv_platform.h"
#include "comp_sun.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("sun", TCompSun);

void TCompSun::debugInMenu() {
	ImGui::DragFloat3("rotationCenter", &rotationCenter.x, 1.f, -1500.f, 1500.f);
	ImGui::DragFloat("rotationSpeed", &rotationSpeed, 0.0001f, 0.f, 90.f);
	ImGui::DragFloat("distance", &distance, 1.f, 100.f, 800.f);

	ImGui::DragFloat("yaw", &currentYaw, 1.f, -180.f, 180.f);
	ImGui::DragFloat("pitch", &currentPitch, 1.f, -1.f, -89.95f);
	ImGui::DragFloat("roll", &currentRoll, 1.f, -180.f, 180.f);
}

void TCompSun::load(const json& j, TEntityParseContext& ctx) {
	rotationCenter = loadVEC3(j["rotation_center"]);
	rotationSpeed = j.value("rotation_speed", 0.001f);//grados por segundo
	distance = j.value("distance", 600.f);
	initialYaw = j.value("initial_yaw", 0.f);//grados
	initialPitch = j.value("initial_pitch", -1.f);//grados (de -1 a - 179)
	initialRoll = j.value("initial_roll", 0.f);//grados
	currentYaw = initialYaw;
	currentPitch = initialPitch;
	currentRoll = initialRoll;
}

void TCompSun::registerMsgs() {
	DECL_MSG(TCompSun, TMsgAllScenesCreated, onAllScenesCreated);
}


void TCompSun::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	TCompTransform* transform = get<TCompTransform>();
	transformHandle = CHandle(transform);
	//go to center
	transform->setPosition(rotationCenter);
	//set pitch
	getTransform()->setYawPitchRoll(deg2rad(initialYaw), deg2rad(initialPitch), deg2rad(initialRoll));
	//move back
	VEC3 pos = transform->getPosition() - transform->getFront() * distance;
	transform->setPosition(pos);
}


void TCompSun::update(float dt) {
	//mover (pasar grados a radianes)
	TCompTransform* transform = getTransform();
	transform->setPosition(rotationCenter);
	//transform->getYawPitchRoll(&currentYaw, &currentPitch);
	currentYaw -= rotationSpeed * dt;
	//transform->setYawPitchRoll(deg2rad(currentYaw), deg2rad(currentPitch), deg2rad(currentRoll));
	//crear matrices yaw, pitch, roll
	MAT44 yawMAtrix = MAT44::CreateFromYawPitchRoll(deg2rad(currentYaw), 0, 0);
	MAT44 pitchMAtrix = MAT44::CreateFromYawPitchRoll(0, deg2rad(currentPitch), 0);
	MAT44 rollMAtrix = MAT44::CreateFromYawPitchRoll(0, 0 , deg2rad(currentRoll));
	//multiplicar en el orden deseado
	MAT44 finalMatrix = yawMAtrix * rollMAtrix * pitchMAtrix;
	transform->setRotation(finalMatrix);
	//transform->setYawPitchRoll(currentYaw, currentPitch);
	VEC3 pos = transform->getPosition() - transform->getFront() * distance;
	transform->setPosition(pos);
	//puede que haya que girar 180 de yaw al llegar a -89.95 de pitch

}
