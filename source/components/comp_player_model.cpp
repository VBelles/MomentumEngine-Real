#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_model.h"
#include "player/AirborneActionState.h"
#include "player/GroundedActionState.h"
#include "player/JumpSquatActionState.h"

DECL_OBJ_MANAGER("player_model", TCompPlayerModel);

using namespace physx;

void TCompPlayerModel::debugInMenu() {
	ImGui::DragFloat("Speed", &ssj1->maxHorizontalSpeed, 0.1f, 0.f, 20.f);
	ImGui::DragFloat("Rotation", &ssj1->rotationSpeed, 0.1f, 0.f, 20.f);
	ImGui::DragFloat("fallingMultiplier", &ssj1->fallingMultiplier, 0.01f, 1.f, 2.f);
	ImGui::DragFloat3("jumpVelocity", &ssj1->jumpVelocityVector.x, 1.f, -50.f, 1000000.f);
	ImGui::DragFloat3("accelerationVector", &accelerationVector.x, 1.f, -1500.f, -0.1f);
}

void TCompPlayerModel::load(const json& j, TEntityParseContext& ctx) {
	if (j.count("initialAcceleration")) accelerationVector = loadVEC3(j["initialAcceleration"]);
	if (j.count("initialVelocity")) velocityVector = loadVEC3(j["initialVelocity"]);
	acceleration = j.value("acceleration", 0.0f);

	ssj1 = loadPowerStats(j["ssj1"]);
	ssj2 = loadPowerStats(j["ssj2"]);
	ssj3 = loadPowerStats(j["ssj3"]);
	
}

PowerStats * TCompPlayerModel::loadPowerStats(const json & j) {
	PowerStats* ssj = new PowerStats();
	ssj->maxHorizontalSpeed = j.value("maxSpeed", 0.0f);
	ssj->rotationSpeed = j.value("rotation_speed", 0.0f);
	ssj->fallingMultiplier = j.value("falling_multiplier", 0.0f);
	ssj->maxVelocityVertical = j.value("maxVelocityVertical", 0.0f);
	ssj->jumpVelocityVector = loadVEC3(j["jumpVelocity"]);
	return ssj;
}

void TCompPlayerModel::SetActionState(ActionStates newState) {
	IActionState* exitingState = actionState;
	actionState = actionStates[newState];
	if (exitingState) exitingState->OnStateExit(actionState);
	if (actionState)  actionState->OnStateEnter(exitingState);
}


void TCompPlayerModel::registerMsgs() {
	DECL_MSG(TCompPlayerModel, TMsgEntitiesGroupCreated, OnGroupCreated);
}

PowerStats* TCompPlayerModel::GetPowerStats() {
	return currentPowerStats;
}

void TCompPlayerModel::OnGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	myTransform = get<TCompTransform>();

	CEntity *camera = (CEntity *)getEntityByName("xthe_camera");
	currentCamera = camera->get<TCompCamera>();
	assert(currentCamera);

	collider = get<TCompCollider>();
	assert(collider);

	actionStates = {
		{ ActionStates::Airborne, new AirborneActionState(this) },
		{ ActionStates::Grounded, new GroundedActionState(this) },
		{ ActionStates::JumpSquat, new JumpSquatActionState(this) },
	};
	SetActionState(ActionStates::Grounded);
	currentPowerStats = ssj1;
}



void TCompPlayerModel::update(float dt) {
	actionState->update(dt);
}

//Aquí llega sin normalizar, se debe hacer justo antes de aplicar el movimiento si se quiere que pueda caminar
void TCompPlayerModel::SetMovementInput(VEC2 input, float delta) {
	actionState->SetMovementInput(input, delta);	
}

void TCompPlayerModel::JumpButtonPressed() {
	actionState->OnJumpHighButton();
}
