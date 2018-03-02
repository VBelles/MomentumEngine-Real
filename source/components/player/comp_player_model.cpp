#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_model.h"
#include "components/comp_render_ui.h"
#include "PowerGauge.h"
#include "states/AirborneActionState.h"
#include "states/GroundedActionState.h"
#include "states/movement_states/normal_jump/JumpSquatActionState.h"
#include "states/movement_states/normal_jump/GhostJumpSquatActionState.h"
#include "states/movement_states/GhostJumpWindowActionState.h"
#include "states/movement_states/RunActionState.h"
#include "states/movement_states/normal_jump/AirborneNormalActionState.h"
#include "states/movement_states/long_jump/AirborneLongActionState.h"
#include "states/movement_states/long_jump/GhostJumpSquatLongActionState.h"
#include "states/movement_states/long_jump/JumpSquatLongActionState.h"
#include "states/movement_states/TurnAroundActionState.h"
#include "states/attack_states/FastAttackActionState.h"
#include "states/attack_states/StrongAttackActionState.h"
#include "states/attack_states/FallingAttackActionState.h"
#include "states/attack_states/HorizontalLauncherActionState.h"
#include "states/attack_states/VerticalLauncherActionState.h"
#include "states/attack_states/GrabHighActionState.h"
#include "states/attack_states/GrabLongActionState.h"
#include "states/attack_states/PropelHighActionState.h"
#include "states/attack_states/PropelLongActionState.h"

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
	ssj->airAcceleration = j.value("airAcceleration", 0.0f);
	ssj->shortHopVelocity = j.value("shortHopVelocity", 0.0f);
	return ssj;
}

void TCompPlayerModel::SetMovementState(ActionStates newState) {
	//dbg("Frame: %d\n", frame);
	nextMovementState = newState;
}

void TCompPlayerModel::ChangeMovementState(ActionStates newState) {
	IActionState* exitingState = movementState;
	movementState = movementStates[newState];
	if (exitingState) exitingState->OnStateExit(movementState);
	if (movementState) movementState->OnStateEnter(exitingState);
}

void TCompPlayerModel::SetAttackState(ActionStates newState) {
	nextAttackState = newState;
}

void TCompPlayerModel::ChangeAttackState(ActionStates newState) {
	IActionState* exitingState = attackState;
	attackState = attackStates[newState];
	if (exitingState) exitingState->OnStateExit(attackState);
	if (attackState) attackState->OnStateEnter(exitingState);
}


void TCompPlayerModel::registerMsgs() {
	DECL_MSG(TCompPlayerModel, TMsgEntitiesGroupCreated, OnGroupCreated);
	DECL_MSG(TCompPlayerModel, TMsgAttackHit, OnAttackHit);
	DECL_MSG(TCompPlayerModel, TMsgHitboxEnter, OnHitboxEnter);
	DECL_MSG(TCompPlayerModel, TMsgCollect, OnCollect);
	DECL_MSG(TCompPlayerModel, TMsgGainPower, OnGainPower);
	DECL_MSG(TCompPlayerModel, TMsgOutOfBounds, OnOutOfBounds);
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
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor { 0, 0, 0, 0 });
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 200 - 25, 0 + 25));
		ImGui::SetNextWindowSize(ImVec2(200, 70));
		ImGui::Begin("Ui", &showWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

		//Hp bar
		std::string hpProgressBarText = "HP: " + std::to_string(hp) + "/" + std::to_string(maxHp);
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor { 0, 255, 0 });
		ImGui::ProgressBar((float)hp / maxHp, ImVec2(-1, 0), hpProgressBarText.c_str());
		ImGui::PopStyleColor();

		//Power bar
		std::string powerProgressBarText = "Power: " + std::to_string((int)powerGauge->power) + "/" + std::to_string((int)powerGauge->maxPower);
		ImVec4 color = powerGauge->powerLevel == 1 ? ImColor{ 255, 255, 0 } : powerGauge->powerLevel == 2 ? ImColor{ 255, 255 / 2, 0 } : ImColor{ 255, 0, 0 };
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
		ImGui::ProgressBar((float)powerGauge->power / powerGauge->maxPower, ImVec2(-1, 0), powerProgressBarText.c_str());
		ImGui::PopStyleColor();

		//Chrysalis counter
		std::string chrysalisProgressBarText = "Chrysalis: " + std::to_string(chrysalis) + "/" + std::to_string(chrysalisTarget);
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor { 255, 191, 0 });
		ImGui::ProgressBar((float)chrysalis / chrysalisTarget, ImVec2(-1, 0), chrysalisProgressBarText.c_str());
		ImGui::PopStyleColor();

		ImGui::End();
		ImGui::PopStyleColor();

		if (!CApp::get().showDebug) return;

		ImGui::Begin("Params Main Character", &showWindow);
		ImGui::DragFloat("Speed_Ssj1", &ssj1->maxHorizontalSpeed, 0.1f, 0.f, 40.f);
		ImGui::DragFloat("Rotation_Ssj1", &ssj1->rotationSpeed, 0.1f, 0.f, 20.f);
		ImGui::DragFloat("FallingMultiplier_Ssj1", &ssj1->fallingMultiplier, 0.01f, 1.f, 2.f);
		ImGui::DragFloat("LongGravityMultiplier_Ssj1", &ssj1->longGravityMultiplier, 1.f, 0.f, 2.f);
		ImGui::DragFloat("MaxVerticalVelocity_Ssj1", &ssj1->maxVelocityVertical, 1.f, 0.f, 100.f);
		ImGui::DragFloat("Acceleration_Ssj1", &ssj1->acceleration, 1.f, 0.f, 100.f);
		ImGui::DragFloat("AirAcceleration_Ssj1", &ssj1->airAcceleration, 1.f, 0.f, 100.f);
		ImGui::DragFloat("ShortHopVelocity_Ssj1", &ssj1->shortHopVelocity, 1.f, 0.f, 100.f);
		ImGui::DragFloat3("JumpVelocity_Ssj1", &ssj1->jumpVelocityVector.x, 1.f, -50.f, 1000000.f);
		ImGui::DragFloat3("LongJumpVelocity_Ssj1", &ssj1->longJumpVelocityVector.x, 1.f, -50.f, 1000000.f);

		ImGui::DragFloat("Speed_Ssj2", &ssj2->maxHorizontalSpeed, 0.1f, 0.f, 40.f);
		ImGui::DragFloat("Rotation_Ssj2", &ssj2->rotationSpeed, 0.1f, 0.f, 20.f);
		ImGui::DragFloat("FallingMultiplier_Ssj2", &ssj2->fallingMultiplier, 0.01f, 1.f, 2.f);
		ImGui::DragFloat("LongGravityMultiplier_Ssj2", &ssj2->longGravityMultiplier, 1.f, 0.f, 2.f);
		ImGui::DragFloat("MaxVerticalVelocity_Ssj2", &ssj2->maxVelocityVertical, 1.f, 0.f, 100.f);
		ImGui::DragFloat("Acceleration_Ssj2", &ssj2->acceleration, 1.f, 0.f, 100.f);
		ImGui::DragFloat("AirAcceleration_Ssj2", &ssj2->airAcceleration, 1.f, 0.f, 100.f);
		ImGui::DragFloat("ShortHopVelocity_Ssj2", &ssj2->shortHopVelocity, 1.f, 0.f, 100.f);
		ImGui::DragFloat3("JumpVelocity_Ssj2", &ssj2->jumpVelocityVector.x, 1.f, -50.f, 1000000.f);
		ImGui::DragFloat3("LongJumpVelocity_Ssj2", &ssj2->longJumpVelocityVector.x, 1.f, -50.f, 1000000.f);

		ImGui::DragFloat("Speed_Ssj3", &ssj3->maxHorizontalSpeed, 0.1f, 0.f, 40.f);
		ImGui::DragFloat("Rotation_Ssj3", &ssj3->rotationSpeed, 0.1f, 0.f, 20.f);
		ImGui::DragFloat("FallingMultiplier_Ssj3", &ssj3->fallingMultiplier, 0.01f, 1.f, 2.f);
		ImGui::DragFloat("LongGravityMultiplier_Ssj3", &ssj3->longGravityMultiplier, 1.f, 0.f, 2.f);
		ImGui::DragFloat("MaxVerticalVelocity_Ssj3", &ssj3->maxVelocityVertical, 1.f, 0.f, 100.f);
		ImGui::DragFloat("Acceleration_Ssj3", &ssj3->acceleration, 1.f, 0.f, 100.f);
		ImGui::DragFloat("AirAcceleration_Ssj3", &ssj3->airAcceleration, 1.f, 0.f, 100.f);
		ImGui::DragFloat("ShortHopVelocity_Ssj3", &ssj3->shortHopVelocity, 1.f, 0.f, 100.f);
		ImGui::DragFloat3("JumpVelocity_Ssj3", &ssj3->jumpVelocityVector.x, 1.f, -50.f, 1000000.f);
		ImGui::DragFloat3("LongJumpVelocity_Ssj3", &ssj3->longJumpVelocityVector.x, 1.f, -50.f, 1000000.f);

		if (ImGui::DragFloat("Gravity", &accelerationVector.y, 1.f, -1500.f, -0.1f)) {
			gravity = accelerationVector.y;
		}
		ImGui::End();
	});
	myTransformHandle = get<TCompTransform>();

	CEntity *camera = (CEntity *)getEntityByName("xthe_camera");
	currentCameraHandle = camera->get<TCompCamera>();
	assert(currentCameraHandle.isValid());

	colliderHandle = get<TCompCollider>();
	assert(colliderHandle.isValid());

	movementStates = {
		{ ActionStates::Idle, nullptr },
		{ ActionStates::JumpSquat, new JumpSquatActionState(CHandle(this)) },
		{ ActionStates::GhostJumpSquat, new GhostJumpSquatActionState(CHandle(this)) },
		{ ActionStates::GhostJumpWindow, new GhostJumpWindowActionState(CHandle(this)) },
		{ ActionStates::Run, new RunActionState(CHandle(this)) },
		{ ActionStates::AirborneNormal, new AirborneNormalActionState(CHandle(this)) },
		{ ActionStates::GhostJumpSquatLong, new GhostJumpSquatLongActionState(CHandle(this)) },
		{ ActionStates::JumpSquatLong, new JumpSquatLongActionState(CHandle(this)) },
		{ ActionStates::AirborneLong, new AirborneLongActionState(CHandle(this)) },
		{ ActionStates::TurnAround, new TurnAroundActionState(CHandle(this)) },
	};

	strongAttackHitbox = getEntityByName("Strong attack hitbox");
	fallingAttackHitbox = getEntityByName("Falling attack hitbox");
	verticalLauncherHitbox = getEntityByName("Vertical launcher hitbox");
	grabHitbox = getEntityByName("Grab hitbox");

	attackStates = {
		{ ActionStates::Idle, nullptr },
		{ ActionStates::FastAttack, new FastAttackActionState(CHandle(this), strongAttackHitbox) },
		{ ActionStates::StrongAttack, new StrongAttackActionState(CHandle(this), strongAttackHitbox) },
		{ ActionStates::FallingAttack, new FallingAttackActionState(CHandle(this), fallingAttackHitbox) },
		{ ActionStates::VerticalLauncher, new VerticalLauncherActionState(CHandle(this), verticalLauncherHitbox) },
		{ ActionStates::HorizontalLauncher, new HorizontalLauncherActionState(CHandle(this), verticalLauncherHitbox) },
		{ ActionStates::GrabHigh, new GrabHighActionState(CHandle(this), grabHitbox) },
		{ ActionStates::GrabLong, new GrabLongActionState(CHandle(this), grabHitbox) },
		{ ActionStates::PropelHigh, new PropelHighActionState(CHandle(this)) },
		{ ActionStates::PropelLong, new PropelLongActionState(CHandle(this)) },
	};
	nextMovementState = ActionStates::Run;
	nextAttackState = ActionStates::Idle;
	ChangeMovementState(ActionStates::Run);
	ChangeAttackState(ActionStates::Idle);
	currentPowerStats = ssj1;
}

void TCompPlayerModel::OnCollect(const TMsgCollect & msg) {
	std::string type = msg.type;
	if (type == "chrysalis") {
		++chrysalis;
		if (chrysalis >= chrysalisTarget) {
			// Open boss door.
			CEntity* door = (CEntity*)getEntityByName("door");
			TMsgDestroy msg;
			door->sendMsg(msg);
		}
	}
	else if (type == "coin") {
		// add coin
	}
	else {
		dbg("Collected unknown object %s\n", type);
	}
}

void TCompPlayerModel::update(float dt) {
	frame++;

	movementState->update(dt);
	if (attackState != attackStates[ActionStates::Idle]) {
		attackState->update(dt);
	}
	powerGauge->Update(dt);
	if (!lockWalk) {
		deltaMovement = movementState->GetDeltaMovement();
	}
	else {
		if (attackState != attackStates[ActionStates::Idle]) {
			deltaMovement = attackState->GetDeltaMovement();
		}
	}
	UpdateMovement(dt, deltaMovement);
	
	if (movementState != movementStates[nextMovementState]) {
		ChangeMovementState(nextMovementState);
	}
	if (attackState != attackStates[nextAttackState]) {
		ChangeAttackState(nextAttackState);
	}
}

void TCompPlayerModel::UpdateMovement(float dt, VEC3 deltaMovement) {
	auto c = GetCollider();
	assert(c);
	assert(c->controller);
	physx::PxControllerCollisionFlags myFlags = GetCollider()->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, dt, physx::PxControllerFilters());
	isGrounded = myFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_DOWN);
	if (dynamic_cast<AirborneActionState*>(movementState)) {//NULL si no lo consigue
		if (isGrounded) {
			isTouchingCeiling = false;
			(static_cast<AirborneActionState*>(movementState))->OnLanding();
			if (attackState != attackStates[ActionStates::Idle]) {
				(static_cast<AirborneActionState*>(attackState))->OnLanding();
			}
		}
		if (!isTouchingCeiling) {
			isTouchingCeiling = myFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_UP);
			if (isTouchingCeiling) {
				velocityVector.y = 0.f;
			}
		}
	}
	else if (dynamic_cast<GroundedActionState*>(movementState)) {
		if (!isGrounded) {
			(static_cast<GroundedActionState*>(movementState))->OnLeavingGround();
		}
		else {
			//Si sigue en el suelo anulamos la velocidad ganada por la gravedad
			velocityVector.y = 0.f;
		}
	}
}

//Aqu\ED llega sin normalizar, se debe hacer justo antes de aplicar el movimiento si se quiere que pueda caminar
void TCompPlayerModel::SetMovementInput(VEC2 input, float delta) {
	if (!lockWalk) {
		movementState->SetMovementInput(input);
	}
	else {
		movementState->SetMovementInput(VEC2::Zero);
	}
	if (!lockAttackState && attackState != attackStates[ActionStates::Idle]) {
		(static_cast<AirborneActionState*>(attackState))->SetMovementInput(input);
	}
}

void TCompPlayerModel::JumpButtonPressed() {
	if (!lockMovementState) {
		movementState->OnJumpHighButton();
	}
	else {
		if (attackState != attackStates[ActionStates::Idle]) {
			attackState->OnJumpHighButton();
		}
	}
}

void TCompPlayerModel::JumpButtonReleased() {
	if (!lockMovementState) {
		movementState->OnJumpHighButtonReleased();
	}
	else {
		if (attackState != attackStates[ActionStates::Idle]) {
			attackState->OnJumpHighButtonReleased();
		}
	}
}

void TCompPlayerModel::LongJumpButtonPressed() {
	if (!lockMovementState) {
		movementState->OnJumpLongButton();
	}
	else {
		if (attackState != attackStates[ActionStates::Idle]) {
			attackState->OnJumpLongButton();
		}
	}
}

void TCompPlayerModel::FastAttackButtonPressed() {
	if (!lockAttackState) {
		movementState->OnFastAttackButton();
		if (attackState != attackStates[ActionStates::Idle]) {
			attackState->OnFastAttackButton();
		}
	}
}

void TCompPlayerModel::FastAttackButtonReleased() {
	//if (!lockAttackState) {
	if (attackState != attackStates[ActionStates::Idle]) {
		attackState->OnFastAttackButtonReleased();
	}
	//}
}

void TCompPlayerModel::StrongAttackButtonPressed() {
	if (!lockAttackState) {
		movementState->OnStrongAttackButton();
		if (attackState != attackStates[ActionStates::Idle]) {
			attackState->OnStrongAttackButton();
		}
	}
}

void TCompPlayerModel::StrongAttackButtonReleased() {
	//if (!lockAttackState) {
	if (attackState != attackStates[ActionStates::Idle]) {
		attackState->OnStrongAttackButtonReleased();
	}
	//}
}

void TCompPlayerModel::CenterCameraButtonPressed() {
	GetCamera()->CenterCamera();
}

void TCompPlayerModel::ReleasePowerButtonPressed() {
	if (!lockAttackState) {
		//attackState->OnReleasePower();
		powerGauge->ReleasePower();
	}
}

void TCompPlayerModel::GainPowerButtonPressed() {
	powerGauge->GainPower();
}

bool TCompPlayerModel::IsAttackFree() {
	return (attackState == attackStates[ActionStates::Idle]);
}

void TCompPlayerModel::OnAttackHit(const TMsgAttackHit& msg) {
	hp -= msg.damage;
	if (hp <= 0) {
		dbg("YOU DIED!\n");
		hp = 0;
	}
}

void TCompPlayerModel::OnHitboxEnter(const TMsgHitboxEnter& msg) {
	if (attackState != attackStates[ActionStates::Idle]) {
		attackState->OnHitboxEnter(msg.h_other_entity);
	}
}

void TCompPlayerModel::OnGainPower(const TMsgGainPower& msg) {
	powerGauge->IncreasePower(msg.power);
}

void TCompPlayerModel::OnOutOfBounds(const TMsgOutOfBounds& msg) {
	dbg("out of bounds \n");

	SetAttackState(ActionStates::Idle);
	SetMovementState(ActionStates::AirborneNormal);

	GetCollider()->controller->setFootPosition({ respawnPosition.x, respawnPosition.y, respawnPosition.z });
	velocityVector = VEC3(0, 0, 0);
}
