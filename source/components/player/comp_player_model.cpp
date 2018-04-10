#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_model.h"
#include "game_constants.h"
#include "components/comp_render_ui.h"
#include "components/comp_tags.h"
#include "components/controllers/comp_camera_player.h"
#include "components/player/filters/PlayerFilterCallback.h"
#include "components/comp_collectable.h"
#include "states/AirborneActionState.h"
#include "states/GroundedActionState.h"
#include "states/base_states/GhostJumpWindowActionState.h"
#include "states/base_states/RunActionState.h"
#include "states/base_states/WalkActionState.h"
#include "states/base_states/normal_jump/JumpSquatActionState.h"
#include "states/base_states/normal_jump/GhostJumpSquatActionState.h"
#include "states/base_states/normal_jump/PropelHighActionState.h"
#include "states/base_states/normal_jump/AirborneNormalActionState.h"
#include "states/base_states/long_jump/AirborneLongActionState.h"
#include "states/base_states/long_jump/GhostJumpSquatLongActionState.h"
#include "states/base_states/long_jump/JumpSquatLongActionState.h"
#include "states/base_states/long_jump/PropelLongActionState.h"
#include "states/base_states/TurnAroundActionState.h"
#include "states/base_states/IdleActionState.h"
#include "states/base_states/LandingActionState.h"
#include "states/base_states/FallingAttackLandingActionState.h"
#include "states/base_states/wall_jump/HuggingWallActionState.h"
#include "states/base_states/wall_jump/WallJumpSquatActionState.h"
#include "states/base_states/wall_jump/AirborneWallJumpActionState.h"
#include "states/base_states/wall_jump/WallJumpSquatPlummetActionState.h"
#include "states/base_states/wall_jump/WallJumpPlummetActionState.h"
#include "states/base_states/FallingAttackActionState.h"
#include "states/base_states/StrongAttackActionState.h"
#include "states/base_states/HorizontalLauncherActionState.h"
#include "states/base_states/VerticalLauncherActionState.h"
#include "states/base_states/ReleasePowerGroundActionState.h"
#include "states/base_states/JumpSquatSpringActionState.h"
#include "states/base_states/IdleTurnAroundActionState.h"
#include "states/concurrent_states/FastAttackActionState.h"
#include "states/concurrent_states/FastAttackAirActionState.h"
#include "states/concurrent_states/GrabHighActionState.h"
#include "states/concurrent_states/GrabLongActionState.h"
#include "states/concurrent_states/ReleasePowerAirActionState.h"

DECL_OBJ_MANAGER("player_model", TCompPlayerModel);

using namespace physx;

void TCompPlayerModel::debugInMenu() {
	ImGui::DragFloat("Speed_Ssj1", &ssj1->maxHorizontalSpeed, 0.1f, 0.f, 40.f);
	ImGui::DragFloat("Rotation_Ssj1", &ssj1->rotationSpeed, 0.1f, 0.f, 20.f);
	ImGui::DragFloat("LandingLag_Ssj1", &ssj1->landingLag, 1.f, 0.f, 20.f);
	ImGui::DragFloat("FallingMultiplier_Ssj1", &ssj1->fallingMultiplier, 0.01f, 1.f, 2.f);
	ImGui::DragFloat("LongGravityMultiplier_Ssj1", &ssj1->longGravityMultiplier, 1.f, 0.f, 2.f);
	ImGui::DragFloat("MaxVerticalVelocity_Ssj1", &ssj1->maxVelocityVertical, 1.f, 0.f, 100.f);
	ImGui::DragFloat("Acceleration_Ssj1", &ssj1->acceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat("Deceleration_Ssj1", &ssj1->deceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat("AirAcceleration_Ssj1", &ssj1->airAcceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat("ShortHopVelocity_Ssj1", &ssj1->shortHopVelocity, 1.f, 0.f, 100.f);
	ImGui::DragFloat("SpringJumpVelocity_Ssj1", &ssj1->springJumpVelocity, 1.f, 0.f, 100.f);
	ImGui::DragFloat3("JumpVelocity_Ssj1", &ssj1->jumpVelocityVector.x, 1.f, -50.f, 1000000.f);
	ImGui::DragFloat3("LongJumpVelocity_Ssj1", &ssj1->longJumpVelocityVector.x, 1.f, -50.f, 1000000.f);
	ImGui::DragFloat3("WallJumpVelocity_Ssj1", &ssj1->wallJumpVelocityVector.x, 1.f, -50.f, 1000000.f);

	ImGui::DragFloat("Speed_Ssj2", &ssj2->maxHorizontalSpeed, 0.1f, 0.f, 40.f);
	ImGui::DragFloat("Rotation_Ssj2", &ssj2->rotationSpeed, 0.1f, 0.f, 20.f);
	ImGui::DragFloat("LandingLag_Ssj2", &ssj2->landingLag, 1.f, 0.f, 20.f);
	ImGui::DragFloat("FallingMultiplier_Ssj2", &ssj2->fallingMultiplier, 0.01f, 1.f, 2.f);
	ImGui::DragFloat("LongGravityMultiplier_Ssj2", &ssj2->longGravityMultiplier, 1.f, 0.f, 2.f);
	ImGui::DragFloat("MaxVerticalVelocity_Ssj2", &ssj2->maxVelocityVertical, 1.f, 0.f, 100.f);
	ImGui::DragFloat("Acceleration_Ssj2", &ssj2->acceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat("Deceleration_Ssj2", &ssj2->deceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat("AirAcceleration_Ssj2", &ssj2->airAcceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat("ShortHopVelocity_Ssj2", &ssj2->shortHopVelocity, 1.f, 0.f, 100.f);
	ImGui::DragFloat("SpringJumpVelocity_Ssj2", &ssj2->springJumpVelocity, 1.f, 0.f, 100.f);
	ImGui::DragFloat3("JumpVelocity_Ssj2", &ssj2->jumpVelocityVector.x, 1.f, -50.f, 1000000.f);
	ImGui::DragFloat3("LongJumpVelocity_Ssj2", &ssj2->longJumpVelocityVector.x, 1.f, -50.f, 1000000.f);
	ImGui::DragFloat3("WallJumpVelocity_Ssj2", &ssj2->wallJumpVelocityVector.x, 1.f, -50.f, 1000000.f);

	ImGui::DragFloat("Speed_Ssj3", &ssj3->maxHorizontalSpeed, 0.1f, 0.f, 40.f);
	ImGui::DragFloat("Rotation_Ssj3", &ssj3->rotationSpeed, 0.1f, 0.f, 20.f);
	ImGui::DragFloat("LandingLag_Ssj3", &ssj3->landingLag, 1.f, 0.f, 20.f);
	ImGui::DragFloat("FallingMultiplier_Ssj3", &ssj3->fallingMultiplier, 0.01f, 1.f, 2.f);
	ImGui::DragFloat("LongGravityMultiplier_Ssj3", &ssj3->longGravityMultiplier, 1.f, 0.f, 2.f);
	ImGui::DragFloat("MaxVerticalVelocity_Ssj3", &ssj3->maxVelocityVertical, 1.f, 0.f, 100.f);
	ImGui::DragFloat("Acceleration_Ssj3", &ssj3->acceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat("Deceleration_Ssj3", &ssj3->deceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat("AirAcceleration_Ssj3", &ssj3->airAcceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat("ShortHopVelocity_Ssj3", &ssj3->shortHopVelocity, 1.f, 0.f, 100.f);
	ImGui::DragFloat("SpringJumpVelocity_Ssj3", &ssj3->springJumpVelocity, 1.f, 0.f, 100.f);
	ImGui::DragFloat3("JumpVelocity_Ssj3", &ssj3->jumpVelocityVector.x, 1.f, -50.f, 1000000.f);
	ImGui::DragFloat3("LongJumpVelocity_Ssj3", &ssj3->longJumpVelocityVector.x, 1.f, -50.f, 1000000.f);
	ImGui::DragFloat3("WallJumpVelocity_Ssj3", &ssj3->wallJumpVelocityVector.x, 1.f, -50.f, 1000000.f);

	if (ImGui::DragFloat("Gravity", &accelerationVector.y, 1.f, -1500.f, -0.1f)) {
		baseGravity = accelerationVector.y;
	}

	ImGui::DragFloat("WalkingSpeed", &walkingSpeed, 0.1f, 0.1f, 7.f);
	ImGui::DragFloat("MaxVelocityUpwards", &maxVelocityUpwards, 0.1f, 10.f, 60.f);
}

void TCompPlayerModel::load(const json& j, TEntityParseContext& ctx) {
	baseGravity = j.value("gravity", 0.0f);
	accelerationVector.y = baseGravity;
	currentGravity = baseGravity;

	walkingSpeed = j.value("walkingSpeed", 5.0f);
	maxVelocityUpwards = j.value("maxVelocityUpwards", 45.0f);

	ssj1 = loadPowerStats(j["ssj1"]);
	ssj2 = loadPowerStats(j["ssj2"]);
	ssj3 = loadPowerStats(j["ssj3"]);

}

PowerStats * TCompPlayerModel::loadPowerStats(const json & j) {
	PowerStats* ssj = new PowerStats();
	ssj->maxHorizontalSpeed = j.value("maxSpeed", 0.0f);
	ssj->rotationSpeed = j.value("rotation_speed", 0.0f);
	ssj->landingLag = j.value("landingLag", 0.0f);
	ssj->fallingMultiplier = j.value("falling_multiplier", 0.0f);
	ssj->longGravityMultiplier = j.value("long_gravity_multiplier", 0.0f);
	ssj->maxVelocityVertical = j.value("maxVelocityVertical", 0.0f);
	ssj->jumpVelocityVector = loadVEC3(j["jumpVelocity"]);
	ssj->longJumpVelocityVector = loadVEC3(j["longJumpVelocity"]);
	ssj->wallJumpVelocityVector = loadVEC3(j["wallJumpVelocity"]);
	ssj->acceleration = j.value("acceleration", 0.0f);
	ssj->deceleration = j.value("deceleration", 0.0f);
	ssj->airAcceleration = j.value("airAcceleration", 0.0f);
	ssj->shortHopVelocity = j.value("shortHopVelocity", 0.0f);
	ssj->springJumpVelocity = j.value("springJumpVelocity", 0.0f);
	ssj->plummetTime = j.value("plummetTime", 0.0f);

	return ssj;
}

void TCompPlayerModel::setBaseState(ActionStates newState) {
	nextBaseState = newState;
}

void TCompPlayerModel::ChangeBaseState(ActionStates newState) {
	IActionState* exitingState = baseState;
	baseState = baseStates[newState];
	if (exitingState) exitingState->OnStateExit(baseState);
	if (baseState) baseState->OnStateEnter(exitingState);
}

void TCompPlayerModel::setConcurrentState(ActionStates newState) {
	nextConcurrentState = newState;
}

void TCompPlayerModel::ChangeConcurrentState(ActionStates newState) {
	IActionState* exitingState = concurrentState;
	concurrentState = concurrentStates[newState];
	if (exitingState) exitingState->OnStateExit(concurrentState);
	if (concurrentState) concurrentState->OnStateEnter(exitingState);
}

void TCompPlayerModel::registerMsgs() {
	DECL_MSG(TCompPlayerModel, TMsgEntitiesGroupCreated, OnGroupCreated);
	DECL_MSG(TCompPlayerModel, TMsgAttackHit, OnAttackHit);
	DECL_MSG(TCompPlayerModel, TMsgHitboxEnter, OnHitboxEnter);
	DECL_MSG(TCompPlayerModel, TMsgCollect, OnCollect);
	DECL_MSG(TCompPlayerModel, TMsgGainPower, OnGainPower);
	DECL_MSG(TCompPlayerModel, TMsgOutOfBounds, OnOutOfBounds);
	DECL_MSG(TCompPlayerModel, TMsgPowerLvlChange, OnLevelChange);
	DECL_MSG(TCompPlayerModel, TMsgOnShapeHit, OnShapeHit);
	DECL_MSG(TCompPlayerModel, TMsgOnContact, OnContact);
}

PowerStats* TCompPlayerModel::GetPowerStats() {
	return currentPowerStats;
}

void TCompPlayerModel::OnLevelChange(const TMsgPowerLvlChange& msg) {
	switch (msg.powerLvl) {
	case 1: currentPowerStats = ssj1; break;
	case 2: currentPowerStats = ssj2; break;
	case 3: currentPowerStats = ssj3; break;
	}
}

void TCompPlayerModel::OnGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	TCompRenderUI* renderUI = get<TCompRenderUI>();

	myTransformHandle = get<TCompTransform>();
	colliderHandle = get<TCompCollider>();
	powerGaugeHandle = get<TCompPowerGauge>();

	respawnPosition = GetTransform()->getPosition();

	renderUI->registerOnRenderUI([&]() {

		bool showWindow = true;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor { 0, 0, 0, 0 });
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 200 - 25, 0 + 25));
		ImGui::SetNextWindowSize(ImVec2(200, 70));
		ImGui::Begin("Ui", &showWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

		//Hp bar
		std::string hpProgressBarText = "HP: " + std::to_string((int)hp) + "/" + std::to_string((int)maxHp);
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor { 0, 255, 0 });
		ImGui::ProgressBar((float)hp / maxHp, ImVec2(-1, 0), hpProgressBarText.c_str());
		ImGui::PopStyleColor();

		//Power bar
		std::string powerProgressBarText = "Power: " + std::to_string((int)GetPowerGauge()->power) + "/" + std::to_string((int)GetPowerGauge()->maxPower);
		ImVec4 color = GetPowerGauge()->powerLevel == 1 ? ImColor{ 255, 255, 0 } : GetPowerGauge()->powerLevel == 2 ? ImColor{ 255, 255 / 2, 0 } : ImColor{ 255, 0, 0 };
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
		ImGui::ProgressBar((float)GetPowerGauge()->power / GetPowerGauge()->maxPower, ImVec2(-1, 0), powerProgressBarText.c_str());
		ImGui::PopStyleColor();

		//Chrysalis counter
		std::string chrysalisProgressBarText = "Chrysalis: " + std::to_string(chrysalis) + "/" + std::to_string(chrysalisTarget);
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor { 255, 191, 0 });
		ImGui::ProgressBar((float)chrysalis / chrysalisTarget, ImVec2(-1, 0), chrysalisProgressBarText.c_str());
		ImGui::PopStyleColor();


		ImGui::End();
		ImGui::PopStyleColor();

		if (showVictoryDialog) {
			//-------- WIN DIALOG --------------------------------
			ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor { 0, 0, 0, 255 });
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - 200, ImGui::GetIO().DisplaySize.y / 4));
			ImGui::SetNextWindowSize(ImVec2(300, 200));
			ImGui::Begin("victoryWindow", &showVictoryDialog, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
			ImGui::TextUnformatted("CONGRATULATIONS!\n\nYou collected enough chrysalis\n\nto open the path to the final boss!\n\n\n");
			ImGui::TextUnformatted("You can keep exploring and see\n\nif you can collect the other two.\n");
			ImGui::End();
			ImGui::PopStyleColor();
		}

		if (!CApp::get().showDebug) return;

		ImGui::Begin("Params Main Character", &showWindow);
		debugInMenu();
		ImGui::End();
	});


	strongAttackHitbox = getEntityByName("Strong attack hitbox");
	fastAttackHitbox = getEntityByName("Fast attack hitbox");
	fastAttackAirHitbox = getEntityByName("Fast attack air hitbox");
	fallingAttackHitbox = getEntityByName("Falling attack hitbox");
	fallingAttackLandingHitbox = getEntityByName("Falling attack landing hitbox");
	verticalLauncherHitbox = getEntityByName("Vertical launcher hitbox");
	horizontalLauncherHitbox = getEntityByName("Horizontal launcher hitbox");
	grabHitbox = getEntityByName("Grab hitbox");
	wallJumpPlummetHitbox = getEntityByName("Wall jump plummet hitbox");
	releasePowerSmallHitbox = getEntityByName("Release power small hitbox");
	releasePowerBigHitbox = getEntityByName("Release power big hitbox");

	CHandle playerModelHandle = CHandle(this);
	baseStates = {
		{ ActionStates::Idle, new IdleActionState(playerModelHandle) },
	{ ActionStates::JumpSquat, new JumpSquatActionState(playerModelHandle) },
	{ ActionStates::GhostJumpSquat, new GhostJumpSquatActionState(playerModelHandle) },
	{ ActionStates::GhostJumpWindow, new GhostJumpWindowActionState(playerModelHandle) },
	{ ActionStates::Run, new RunActionState(playerModelHandle) },
	{ ActionStates::Walk, new WalkActionState(playerModelHandle) },
	{ ActionStates::AirborneNormal, new AirborneNormalActionState(playerModelHandle) },
	{ ActionStates::GhostJumpSquatLong, new GhostJumpSquatLongActionState(playerModelHandle) },
	{ ActionStates::JumpSquatLong, new JumpSquatLongActionState(playerModelHandle) },
	{ ActionStates::AirborneLong, new AirborneLongActionState(playerModelHandle) },
	{ ActionStates::TurnAround, new TurnAroundActionState(playerModelHandle) },
	{ ActionStates::Landing, new LandingActionState(playerModelHandle) },
	{ ActionStates::LandingFallingAttack, new FallingAttackLandingActionState(playerModelHandle, fallingAttackLandingHitbox) },
	{ ActionStates::PropelHigh, new PropelHighActionState(playerModelHandle) },
	{ ActionStates::PropelLong, new PropelLongActionState(playerModelHandle) },
	{ ActionStates::HuggingWall, new HuggingWallActionState(playerModelHandle) },
	{ ActionStates::WallJumpSquat, new WallJumpSquatActionState(playerModelHandle) },
	{ ActionStates::AirborneWallJump, new AirborneWallJumpActionState(playerModelHandle) },
	{ ActionStates::FallingAttack, new FallingAttackActionState(playerModelHandle, fallingAttackHitbox) },
	{ ActionStates::StrongAttack, new StrongAttackActionState(playerModelHandle, strongAttackHitbox) },
	{ ActionStates::VerticalLauncher, new VerticalLauncherActionState(playerModelHandle, verticalLauncherHitbox) },
	{ ActionStates::HorizontalLauncher, new HorizontalLauncherActionState(playerModelHandle, horizontalLauncherHitbox) },
	{ ActionStates::ReleasePowerGround, new ReleasePowerGroundActionState(playerModelHandle, releasePowerSmallHitbox, releasePowerBigHitbox) },
	{ ActionStates::JumpSquatSpring, new JumpSquatSpringActionState(playerModelHandle) },
	{ ActionStates::IdleTurnAround, new IdleTurnAroundActionState(playerModelHandle) },
	{ ActionStates::WallJumpSquatPlummet, new WallJumpSquatPlummetActionState(playerModelHandle) },
	{ ActionStates::WallJumpPlummet, new WallJumpPlummetActionState(playerModelHandle, wallJumpPlummetHitbox) },
	};

	concurrentStates = {
		{ ActionStates::Idle, nullptr },
	{ ActionStates::FastAttack, new FastAttackActionState(playerModelHandle, fastAttackHitbox) },
	{ ActionStates::FastAttackAir, new FastAttackAirActionState(playerModelHandle, fastAttackAirHitbox) },
	{ ActionStates::GrabHigh, new GrabHighActionState(playerModelHandle, grabHitbox) },
	{ ActionStates::GrabLong, new GrabLongActionState(playerModelHandle, grabHitbox) },
	{ ActionStates::ReleasePowerAir, new ReleasePowerAirActionState(playerModelHandle, releasePowerSmallHitbox, releasePowerBigHitbox) },
	};
	nextBaseState = ActionStates::Idle;
	nextConcurrentState = ActionStates::Idle;
	ChangeBaseState(ActionStates::Idle);
	ChangeConcurrentState(ActionStates::Idle);
	currentPowerStats = ssj1;

	playerFilterCallback = new PlayerFilterCallback();

}

void TCompPlayerModel::OnCollect(const TMsgCollect& msg) {
	TCompCollectable* collectable = msg.collectableHandle;
	switch (msg.type) {
	case TCompCollectable::Type::CHRYSALIS:
		collectable->collect();
		++chrysalis;
		if (chrysalis == chrysalisTarget) {
			// Open boss door.
			CEntity* door = (CEntity*)getEntityByName("door");
			if (door) door->sendMsg(TMsgDestroy{});
			dialogTimer.reset();
			showVictoryDialog = true;
		}
		break;
	case TCompCollectable::Type::COIN:
		dbg("Collection coin!\n");
		collectable->collect();
		break;
	default:
		dbg("Collected unknown object %d\n", msg.type);
		break;
	}
}

void TCompPlayerModel::update(float dt) {

	if (showVictoryDialog == true && dialogTimer.elapsed() >= dialogTime) {
		showVictoryDialog = false;
	}

	baseState->update(dt);
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->update(dt);
	}
	if (!lockWalk) {
		deltaMovement = baseState->GetDeltaMovement();
	}
	else if (concurrentState != concurrentStates[ActionStates::Idle]) {
		deltaMovement = concurrentState->GetDeltaMovement();
	}

	ApplyGravity(dt);
	updateMovement(dt, deltaMovement);

	if (baseState != baseStates[nextBaseState]) {
		ChangeBaseState(nextBaseState);
	}
	if (concurrentState != concurrentStates[nextConcurrentState]) {
		ChangeConcurrentState(nextConcurrentState);
	}

}

void TCompPlayerModel::ApplyGravity(float delta) {
	if (isAttachedToPlatform) {
		velocityVector.y = 0;
	}
	else {
		float deltaMovementDueToGravity;
		deltaMovementDueToGravity = 0.5f * currentGravity * delta * delta;
		if (dynamic_cast<GroundedActionState*>(baseState)) {
			deltaMovement.y -= currentPowerStats->maxHorizontalSpeed / 2 * delta;
		}
		else {
			deltaMovement.y += deltaMovementDueToGravity;
			//clampear distancia vertical
			deltaMovement.y = deltaMovement.y > maxVerticalSpeed * delta ? maxVerticalSpeed * delta : deltaMovement.y;
		}
		velocityVector.y += currentGravity * delta;
		velocityVector.y = clamp(velocityVector.y, -maxVerticalSpeed, maxVerticalSpeed);
	}
}

void TCompPlayerModel::updateMovement(float dt, VEC3 deltaMovement) {
	VEC3 pos = GetTransform()->getPosition();
	QUAT rot = GetTransform()->getRotation();
	PxFilterData filterData = { GetCollider()->config.group, GetCollider()->config.mask, 0, 0 };
	physx::PxControllerCollisionFlags myFlags = GetCollider()->controller->move(
		physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, dt,
		physx::PxControllerFilters(&filterData, playerFilterCallback, playerFilterCallback));

	isGrounded = myFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_DOWN);
	if (dynamic_cast<AirborneActionState*>(baseState)) {//NULL si no lo consigue
		if (isGrounded) {
			isTouchingCeiling = false;
			(static_cast<AirborneActionState*>(baseState))->OnLanding();
			if (concurrentState != concurrentStates[ActionStates::Idle]) {
				(static_cast<AirborneActionState*>(concurrentState))->OnLanding();
			}
		}
		if (!isTouchingCeiling) {
			isTouchingCeiling = myFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_UP);
			if (isTouchingCeiling) {
				velocityVector.y = -1.f;
			}
		}
	}
	else if (dynamic_cast<GroundedActionState*>(baseState)) {
		if (!isGrounded) {
			if (!isAttachedToPlatform)//What a beautiful hack
				(static_cast<GroundedActionState*>(baseState))->OnLeavingGround();
		}
		else {
			//Si sigue en el suelo anulamos la velocidad ganada por la gravedad
			velocityVector.y = 0.f;
		}
	}
}

//Aqui llega sin normalizar, se debe hacer justo antes de aplicar el movimiento si se quiere que pueda caminar
void TCompPlayerModel::setMovementInput(VEC2 input, float delta) {
	if (!lockWalk) {
		baseState->setMovementInput(input);
	}
	else {
		baseState->setMovementInput(VEC2::Zero);
	}
	if (!lockConcurrentState && concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->setMovementInput(input);
	}
}

TCompCamera* TCompPlayerModel::GetCamera() {
	CEntity* camera = (CEntity *)getEntityByName("game_camera");
	TCompCamera* currentCamera = camera->get<TCompCamera>();
	assert(currentCamera);
	return currentCamera;
}


void TCompPlayerModel::jumpButtonPressed() {
	if (!lockBaseState) {
		baseState->OnJumpHighButton();
	}
	else {
		if (concurrentState != concurrentStates[ActionStates::Idle]) {
			concurrentState->OnJumpHighButton();
		}
	}
}

void TCompPlayerModel::jumpButtonReleased() {
	if (!lockBaseState) {
		baseState->OnJumpHighButtonReleased();
	}
	else {
		if (concurrentState != concurrentStates[ActionStates::Idle]) {
			concurrentState->OnJumpHighButtonReleased();
		}
	}
}

void TCompPlayerModel::longJumpButtonPressed() {
	if (!lockBaseState) {
		baseState->OnJumpLongButton();
	}
	else {
		if (concurrentState != concurrentStates[ActionStates::Idle]) {
			concurrentState->OnJumpLongButton();
		}
	}
}

void TCompPlayerModel::fastAttackButtonPressed() {
	if (!lockBaseState) {
		baseState->OnFastAttackButton();
	}
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->OnFastAttackButton();
	}
}

void TCompPlayerModel::fastAttackButtonReleased() {
	if (!lockBaseState) {
		baseState->OnFastAttackButtonReleased();
	}
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->OnFastAttackButtonReleased();
	}
}

void TCompPlayerModel::strongAttackButtonPressed() {
	if (!lockBaseState) {
		baseState->OnStrongAttackButton();
	}
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->OnStrongAttackButton();
	}
}

void TCompPlayerModel::strongAttackButtonReleased() {
	if (!lockBaseState) {
		baseState->OnStrongAttackButtonReleased();
	}
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->OnStrongAttackButtonReleased();
	}
}

void TCompPlayerModel::centerCameraButtonPressed() {
	CEntity* camera = (CEntity*)getEntityByName("player_camera");
	TCompCameraPlayer* playerCamera = camera->get<TCompCameraPlayer>();
	playerCamera->centerCamera();
}

void TCompPlayerModel::releasePowerButtonPressed() {
	baseState->OnReleasePowerButton();
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->OnReleasePowerButton();
	}
}

void TCompPlayerModel::gainPowerButtonPressed() {//Debug Only
	GetPowerGauge()->gainPower();
}

bool TCompPlayerModel::isConcurrentActionFree() {
	return (concurrentState == concurrentStates[ActionStates::Idle]);
}

void TCompPlayerModel::OnAttackHit(const TMsgAttackHit& msg) {
	//TODO Esto lo tendría que procesar el estado en concreto, por si tiene armor o algo
	hp -= msg.info.damage;
	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
	if (hp <= 0) {
		OnDead();
	}
}

void TCompPlayerModel::OnHitboxEnter(const TMsgHitboxEnter& msg) {
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->OnHitboxEnter(msg.h_other_entity);
	}
	baseState->OnHitboxEnter(msg.h_other_entity);
}

void TCompPlayerModel::OnGainPower(const TMsgGainPower& msg) {
	GetPowerGauge()->increasePower(msg.power);
}

void TCompPlayerModel::OnOutOfBounds(const TMsgOutOfBounds& msg) {
	hp -= 1;
	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
	if (hp <= 0) {
		OnDead();
	}
	else {
		GetCollider()->controller->setFootPosition({ respawnPosition.x, respawnPosition.y, respawnPosition.z });
		velocityVector = VEC3(0, 0, 0);
		setConcurrentState(ActionStates::Idle);
		setBaseState(ActionStates::AirborneNormal);
		CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
		Engine.getCameras().blendInCamera(playerCameraEntity, 0, CModuleCameras::EPriority::GAMEPLAY);
	}
}

void TCompPlayerModel::OnDead() {
	GetCollider()->controller->setFootPosition({ respawnPosition.x, respawnPosition.y, respawnPosition.z });
	velocityVector = VEC3(0, 0, 0);

	setConcurrentState(ActionStates::Idle);
	setBaseState(ActionStates::AirborneNormal);
	hp = maxHp;
	GetPowerGauge()->resetPower();
	CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
	Engine.getCameras().blendInCamera(playerCameraEntity, 0, CModuleCameras::EPriority::GAMEPLAY);
}


void TCompPlayerModel::OnShapeHit(const TMsgOnShapeHit& msg) {
	baseState->OnShapeHit(msg.hit);
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->OnShapeHit(msg.hit);
	}
	//if (!isGrounded && velocityVector.y < 0.f && msg.hit.actor != lastWallEntered) {
	//	lastWallEntered = msg.hit.actor;

	//	VEC3 hitNormal = VEC3(msg.hit.worldNormal.x, msg.hit.worldNormal.y, msg.hit.worldNormal.z);

	//	//dbg("(%f, %f, %f)\n", msg.hit.worldNormal.x, msg.hit.worldNormal.y, msg.hit.worldNormal.z);
	//	//dbg("%f\n", deg2rad(pitch));

	//	VEC3 worldInput = GetCamera()->TransformToWorld(baseState->GetMovementInput());
	//	if (worldInput.Dot(-hitNormal) >= attachWallByInputMinDot || GetTransform()->getFront().Dot(-hitNormal) >= attachWallByFrontMinDot) {
	//		float pitch = asin(-msg.hit.worldNormal.y);
	//		if (pitch >= huggingWallMinPitch && pitch <= huggingWallMaxPitch) {
	//			HuggingWallActionState* actionState = GetBaseState<HuggingWallActionState*>(ActionStates::HuggingWall);
	//			actionState->SetHit(msg.hit);
	//			setBaseState(ActionStates::HuggingWall);
	//		}
	//	}
	//}
}


void TCompPlayerModel::OnContact(const TMsgOnContact& msg) {
	//dbg("ON CONTACT\n");
	if (msg.pair.events.isSet(PxPairFlag::eNOTIFY_TOUCH_FOUND)) {
		//dbg("TOUCH FOUND\n");
	}
	if (msg.pair.events.isSet(PxPairFlag::eNOTIFY_TOUCH_LOST)) {
		//dbg("TOUCH LOST\n");
	}

}