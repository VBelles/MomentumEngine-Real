#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_model.h"
#include "components/comp_render_ui.h"
#include "PowerGauge.h"
#include "states/movement_states/AirborneActionState.h"
#include "states/movement_states/GroundedActionState.h"
#include "states/movement_states/normal_jump/JumpSquatActionState.h"
#include "states/movement_states/normal_jump/GhostJumpSquatActionState.h"
#include "states/movement_states/GhostJumpWindowActionState.h"
#include "states/movement_states/RunActionState.h"
#include "states/movement_states/normal_jump/AirborneNormalActionState.h"
#include "states/movement_states/long_jump/AirborneLongActionState.h"
#include "states/movement_states/long_jump/GhostJumpSquatLongActionState.h"
#include "states/movement_states/long_jump/JumpSquatLongActionState.h"

DECL_OBJ_MANAGER("player_model", TCompPlayerModel);

using namespace physx;

void TCompPlayerModel::debugInMenu() {
	ImGui::DragFloat("Speed_Ssj1", &ssj1->maxHorizontalSpeed, 0.1f, 0.f, 40.f);
	ImGui::DragFloat("Rotation_Ssj1", &ssj1->rotationSpeed, 0.1f, 0.f, 20.f);
	ImGui::DragFloat("FallingMultiplier_Ssj1", &ssj1->fallingMultiplier, 0.01f, 1.f, 2.f);
	ImGui::DragFloat("LongGravityMultiplier_Ssj1", &ssj1->longGravityMultiplier, 1.f, 0.f, 2.f);
	ImGui::DragFloat("MaxVerticalVelocity_Ssj1", &ssj1->maxVelocityVertical, 1.f, 0.f, 100.f);
	ImGui::DragFloat("Acceleration_Ssj1", &ssj1->acceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat3("JumpVelocity_Ssj1", &ssj1->jumpVelocityVector.x, 1.f, -50.f, 1000000.f);
	ImGui::DragFloat3("LongJumpVelocity_Ssj1", &ssj1->longJumpVelocityVector.x, 1.f, -50.f, 1000000.f);

	ImGui::DragFloat("Speed_Ssj2", &ssj2->maxHorizontalSpeed, 0.1f, 0.f, 40.f);
	ImGui::DragFloat("Rotation_Ssj2", &ssj2->rotationSpeed, 0.1f, 0.f, 20.f);
	ImGui::DragFloat("FallingMultiplier_Ssj2", &ssj2->fallingMultiplier, 0.01f, 1.f, 2.f);
	ImGui::DragFloat("LongGravityMultiplier_Ssj2", &ssj2->longGravityMultiplier, 1.f, 0.f, 2.f);
	ImGui::DragFloat("MaxVerticalVelocity_Ssj2", &ssj2->maxVelocityVertical, 1.f, 0.f, 100.f);
	ImGui::DragFloat("Acceleration_Ssj2", &ssj2->acceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat3("JumpVelocity_Ssj2", &ssj2->jumpVelocityVector.x, 1.f, -50.f, 1000000.f);
	ImGui::DragFloat3("LongJumpVelocity_Ssj2", &ssj2->longJumpVelocityVector.x, 1.f, -50.f, 1000000.f);

	ImGui::DragFloat("Speed_Ssj3", &ssj3->maxHorizontalSpeed, 0.1f, 0.f, 40.f);
	ImGui::DragFloat("Rotation_Ssj3", &ssj3->rotationSpeed, 0.1f, 0.f, 20.f);
	ImGui::DragFloat("FallingMultiplier_Ssj3", &ssj3->fallingMultiplier, 0.01f, 1.f, 2.f);
	ImGui::DragFloat("LongGravityMultiplier_Ssj3", &ssj3->longGravityMultiplier, 1.f, 0.f, 2.f);
	ImGui::DragFloat("MaxVerticalVelocity_Ssj3", &ssj3->maxVelocityVertical, 1.f, 0.f, 100.f);
	ImGui::DragFloat("Acceleration_Ssj3", &ssj3->acceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat3("JumpVelocity_Ssj3", &ssj3->jumpVelocityVector.x, 1.f, -50.f, 1000000.f);
	ImGui::DragFloat3("LongJumpVelocity_Ssj3", &ssj3->longJumpVelocityVector.x, 1.f, -50.f, 1000000.f);

	if (ImGui::DragFloat("Gravity", &accelerationVector.y, 1.f, -1500.f, -0.1f)) {
		gravity = accelerationVector.y;
	}

}

void TCompPlayerModel::load(const json& j, TEntityParseContext& ctx) {
	gravity = j.value("gravity", 0.0f);
	accelerationVector.y = gravity;

	ssj1 = loadPowerStats(j["ssj1"]);
	ssj2 = loadPowerStats(j["ssj2"]);
	ssj3 = loadPowerStats(j["ssj3"]);

	powerGauge = new PowerGauge(this);

}

PowerStats * TCompPlayerModel::loadPowerStats(const json & j) {
	PowerStats* ssj = new PowerStats();
	ssj->maxHorizontalSpeed = j.value("maxSpeed", 0.0f);
	ssj->rotationSpeed = j.value("rotation_speed", 0.0f);
	ssj->fallingMultiplier = j.value("falling_multiplier", 0.0f);
	ssj->longGravityMultiplier = j.value("long_gravity_multiplier", 0.0f);
	ssj->maxVelocityVertical = j.value("maxVelocityVertical", 0.0f);
	ssj->jumpVelocityVector = loadVEC3(j["jumpVelocity"]);
	ssj->longJumpVelocityVector = loadVEC3(j["longJumpVelocity"]);
	ssj->acceleration = j.value("acceleration", 0.0f);
	return ssj;
}

void TCompPlayerModel::SetActionState(ActionStates newState) {
	//dbg("Frame: %d\n", frame);
	IActionState* exitingState = actionState;
	actionState = actionStates[newState];
	if (exitingState) exitingState->OnStateExit(actionState);
	if (actionState)  actionState->OnStateEnter(exitingState);
}



void TCompPlayerModel::registerMsgs() {
	DECL_MSG(TCompPlayerModel, TMsgEntitiesGroupCreated, OnGroupCreated);
	DECL_MSG(TCompPlayerModel, TMsgAttackHit, OnAttackHit);
}

PowerStats* TCompPlayerModel::GetPowerStats() {
	return currentPowerStats;
}

void TCompPlayerModel::OnLevelChange(int powerLevel) {
	dbg("Power changed: %d\n", powerLevel);
	if (powerLevel == 1) {
		currentPowerStats = ssj1;
	}
	else if (powerLevel == 2) {
		currentPowerStats = ssj2;
	}
	else if (powerLevel == 3) {
		currentPowerStats = ssj3;
	}
}

void TCompPlayerModel::OnGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	TCompRenderUI* renderUI = get<TCompRenderUI>();
	
	renderUI->registerOnRenderUI([&]() {
		bool showWindow = true;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor {0, 0, 0, 0});
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 200 - 25, 0 + 25));
		ImGui::SetNextWindowSize(ImVec2(200, 70));
		ImGui::Begin("Ui", &showWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		
		//Hp bar
		std::string hpProgressBarText = "HP: " + std::to_string(hp) + "/" + std::to_string(maxHp);
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor { 0, 255, 0 });
		ImGui::ProgressBar((float) hp / maxHp, ImVec2(-1,0), hpProgressBarText.c_str());
		ImGui::PopStyleColor();

		//Power bar
		std::string powerProgressBarText ="Power: " + std::to_string((int) powerGauge->power) + "/" + std::to_string((int) powerGauge->maxPower);
		ImVec4 color = powerGauge->powerLevel == 1 ? ImColor{ 255, 255, 0 } : powerGauge->powerLevel == 2 ? ImColor{ 255, 255/2, 0 } : ImColor{ 255, 0, 0 };
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
		ImGui::ProgressBar((float)powerGauge->power / powerGauge->maxPower, ImVec2(-1, 0), powerProgressBarText.c_str());
		ImGui::PopStyleColor();

		ImGui::End();
		ImGui::PopStyleColor();

		ImGui::Begin("Puta vida", &showWindow);
		ImGui::DragFloat("Speed_Ssj1", &ssj1->maxHorizontalSpeed, 0.1f, 0.f, 40.f);
		ImGui::DragFloat("Rotation_Ssj1", &ssj1->rotationSpeed, 0.1f, 0.f, 20.f);
		ImGui::DragFloat("FallingMultiplier_Ssj1", &ssj1->fallingMultiplier, 0.01f, 1.f, 2.f);
		ImGui::DragFloat("LongGravityMultiplier_Ssj1", &ssj1->longGravityMultiplier, 1.f, 0.f, 2.f);
		ImGui::DragFloat("MaxVerticalVelocity_Ssj1", &ssj1->maxVelocityVertical, 1.f, 0.f, 100.f);
		ImGui::DragFloat("Acceleration_Ssj1", &ssj1->acceleration, 1.f, 0.f, 100.f);
		ImGui::DragFloat3("JumpVelocity_Ssj1", &ssj1->jumpVelocityVector.x, 1.f, -50.f, 1000000.f);
		ImGui::DragFloat3("LongJumpVelocity_Ssj1", &ssj1->longJumpVelocityVector.x, 1.f, -50.f, 1000000.f);

		ImGui::DragFloat("Speed_Ssj2", &ssj2->maxHorizontalSpeed, 0.1f, 0.f, 40.f);
		ImGui::DragFloat("Rotation_Ssj2", &ssj2->rotationSpeed, 0.1f, 0.f, 20.f);
		ImGui::DragFloat("FallingMultiplier_Ssj2", &ssj2->fallingMultiplier, 0.01f, 1.f, 2.f);
		ImGui::DragFloat("LongGravityMultiplier_Ssj2", &ssj2->longGravityMultiplier, 1.f, 0.f, 2.f);
		ImGui::DragFloat("MaxVerticalVelocity_Ssj2", &ssj2->maxVelocityVertical, 1.f, 0.f, 100.f);
		ImGui::DragFloat("Acceleration_Ssj2", &ssj2->acceleration, 1.f, 0.f, 100.f);
		ImGui::DragFloat3("JumpVelocity_Ssj2", &ssj2->jumpVelocityVector.x, 1.f, -50.f, 1000000.f);
		ImGui::DragFloat3("LongJumpVelocity_Ssj2", &ssj2->longJumpVelocityVector.x, 1.f, -50.f, 1000000.f);

		ImGui::DragFloat("Speed_Ssj3", &ssj3->maxHorizontalSpeed, 0.1f, 0.f, 40.f);
		ImGui::DragFloat("Rotation_Ssj3", &ssj3->rotationSpeed, 0.1f, 0.f, 20.f);
		ImGui::DragFloat("FallingMultiplier_Ssj3", &ssj3->fallingMultiplier, 0.01f, 1.f, 2.f);
		ImGui::DragFloat("LongGravityMultiplier_Ssj3", &ssj3->longGravityMultiplier, 1.f, 0.f, 2.f);
		ImGui::DragFloat("MaxVerticalVelocity_Ssj3", &ssj3->maxVelocityVertical, 1.f, 0.f, 100.f);
		ImGui::DragFloat("Acceleration_Ssj3", &ssj3->acceleration, 1.f, 0.f, 100.f);
		ImGui::DragFloat3("JumpVelocity_Ssj3", &ssj3->jumpVelocityVector.x, 1.f, -50.f, 1000000.f);
		ImGui::DragFloat3("LongJumpVelocity_Ssj3", &ssj3->longJumpVelocityVector.x, 1.f, -50.f, 1000000.f);

		if (ImGui::DragFloat("Gravity", &accelerationVector.y, 1.f, -1500.f, -0.1f)) {
			gravity = accelerationVector.y;
		}
		ImGui::End();
	
	});
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
		{ ActionStates::GhostJumpSquat, new GhostJumpSquatActionState(this) },
		{ ActionStates::GhostJumpWindow, new GhostJumpWindowActionState(this) },
		{ ActionStates::Run, new RunActionState(this) },
		{ ActionStates::AirborneNormal, new AirborneNormalActionState(this) },
		{ ActionStates::GhostJumpSquatLong, new GhostJumpSquatLongActionState(this) },
		{ ActionStates::JumpSquatLong, new JumpSquatLongActionState(this) },
		{ ActionStates::AirborneLong, new AirborneLongActionState(this) },
	};
	SetActionState(ActionStates::Run);
	currentPowerStats = ssj1;
}



void TCompPlayerModel::update(float dt) {
	frame++;
	actionState->update(dt);
	powerGauge->Update(dt);
}

//Aquí llega sin normalizar, se debe hacer justo antes de aplicar el movimiento si se quiere que pueda caminar
void TCompPlayerModel::SetMovementInput(VEC2 input, float delta) {
	actionState->SetMovementInput(input);
}

void TCompPlayerModel::JumpButtonPressed() {
	actionState->OnJumpHighButton();
}

void TCompPlayerModel::LongJumpButtonPressed() {
	actionState->OnJumpLongButton();
}

void TCompPlayerModel::FastAttackButtonPressed() {
	actionState->OnFastAttack();
}

void TCompPlayerModel::StrongAttackButtonPressed() {
	actionState->OnStrongAttack();
}

void TCompPlayerModel::CenterCameraButtonPressed() {
	currentCamera->CenterCamera();
}

void TCompPlayerModel::ReleasePowerButtonPressed() {
	powerGauge->ReleasePower();
}

void TCompPlayerModel::GainPowerButtonPressed() {
	powerGauge->GainPower();
}

void TCompPlayerModel::OnAttackHit(const TMsgAttackHit& msg) {
	hp -= msg.damage;
	if (hp <= 0) {
		dbg("YOU DIED!\n");
		hp = 0;
	}
}

