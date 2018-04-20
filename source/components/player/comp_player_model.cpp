#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_model.h"
#include "components/comp_render_ui.h"
#include "components/comp_tags.h"
#include "components/comp_render.h"
#include "components/comp_camera.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_collectable.h"
#include "components/controllers/comp_camera_player.h"
#include "components/player/filters/player_filter_callback.h"
#include "components/player/comp_power_gauge.h"
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
#include "states/base_states/death/DeathActionState.h"
#include "states/base_states/death/PitFallingActionState.h"
#include "states/concurrent_states/FastAttackActionState.h"
#include "states/concurrent_states/FastAttackAirActionState.h"
#include "states/concurrent_states/GrabHighActionState.h"
#include "states/concurrent_states/GrabLongActionState.h"
#include "states/concurrent_states/ReleasePowerAirActionState.h"
#include "modules/game/physics/basic_query_filter_callback.h"
#include "skeleton/comp_skeleton.h"

DECL_OBJ_MANAGER("player_model", TCompPlayerModel);

using namespace physx;

void TCompPlayerModel::registerMsgs() {
	DECL_MSG(TCompPlayerModel, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompPlayerModel, TMsgAttackHit, onAttackHit);
	DECL_MSG(TCompPlayerModel, TMsgHitboxEnter, onHitboxEnter);
	DECL_MSG(TCompPlayerModel, TMsgCollect, onCollect);
	DECL_MSG(TCompPlayerModel, TMsgGainPower, onGainPower);
	DECL_MSG(TCompPlayerModel, TMsgOutOfBounds, onOutOfBounds);
	DECL_MSG(TCompPlayerModel, TMsgPowerLvlChange, onLevelChange);
	DECL_MSG(TCompPlayerModel, TMsgOnShapeHit, onShapeHit);
	DECL_MSG(TCompPlayerModel, TMsgOnContact, onContact);
}

void TCompPlayerModel::debugInMenu() {
	debugInMenu(ssj1, "Ssj1");
	debugInMenu(ssj2, "Ssj2");
	debugInMenu(ssj3, "Ssj3");
	if (ImGui::DragFloat("Gravity", &accelerationVector.y, 1.f, -1500.f, -0.1f)) {
		baseGravity = accelerationVector.y;
	}
	ImGui::DragFloat("WalkingSpeed", &walkingSpeed, 0.1f, 0.1f, 7.f);
	ImGui::DragFloat("MaxVelocityUpwards", &maxVelocityUpwards, 0.1f, 10.f, 60.f);
}

void TCompPlayerModel::debugInMenu(PowerStats* powerStats, std::string name) {
	ImGui::DragFloat((std::string("Speed_") + name).c_str(), &powerStats->maxHorizontalSpeed, 0.1f, 0.f, 40.f);
	ImGui::DragFloat((std::string("Rotation_") + name).c_str(), &powerStats->rotationSpeed, 0.1f, 0.f, 20.f);
	ImGui::DragFloat((std::string("LandingLag_") + name).c_str(), &powerStats->landingLag, 1.f, 0.f, 20.f);
	ImGui::DragFloat((std::string("FallingMultiplier_") + name).c_str(), &powerStats->fallingMultiplier, 0.01f, 1.f, 2.f);
	ImGui::DragFloat((std::string("LongGravityMultiplier_") + name).c_str(), &powerStats->longGravityMultiplier, 1.f, 0.f, 2.f);
	ImGui::DragFloat((std::string("MaxVerticalVelocity_") + name).c_str(), &powerStats->maxVelocityVertical, 1.f, 0.f, 100.f);
	ImGui::DragFloat((std::string("Acceleration_") + name).c_str(), &powerStats->acceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat((std::string("Deceleration_") + name).c_str(), &powerStats->deceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat((std::string("AirAcceleration_") + name).c_str(), &powerStats->airAcceleration, 1.f, 0.f, 100.f);
	ImGui::DragFloat((std::string("ShortHopVelocity_") + name).c_str(), &powerStats->shortHopVelocity, 1.f, 0.f, 100.f);
	ImGui::DragFloat((std::string("SpringJumpVelocity_") + name).c_str(), &powerStats->springJumpVelocity, 1.f, 0.f, 100.f);
	ImGui::DragFloat3((std::string("JumpVelocity_") + name).c_str(), &powerStats->jumpVelocityVector.x, 1.f, -50.f, 1000000.f);
	ImGui::DragFloat3((std::string("LongJumpVelocity_") + name).c_str(), &powerStats->longJumpVelocityVector.x, 1.f, -50.f, 1000000.f);
	ImGui::DragFloat3((std::string("WallJumpVelocity_") + name).c_str(), &powerStats->wallJumpVelocityVector.x, 1.f, -50.f, 1000000.f);
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
	baseState->isChangingBaseState = true;
}

void TCompPlayerModel::changeBaseState(ActionStates newState) {
	IActionState* exitingState = baseState;
	baseState = baseStates[newState];
	if (exitingState) exitingState->onStateExit(baseState);
	if (baseState) baseState->onStateEnter(exitingState);
}

void TCompPlayerModel::setConcurrentState(ActionStates newState) {
	nextConcurrentState = newState;
}

void TCompPlayerModel::changeConcurrentState(ActionStates newState) {
	IActionState* exitingState = concurrentState;
	concurrentState = concurrentStates[newState];
	if (exitingState) exitingState->onStateExit(concurrentState);
	if (concurrentState) concurrentState->onStateEnter(exitingState);
}


void TCompPlayerModel::onLevelChange(const TMsgPowerLvlChange& msg) {
	switch (msg.powerLvl) {
	case 1: currentPowerStats = ssj1; break;
	case 2: currentPowerStats = ssj2; break;
	case 3: currentPowerStats = ssj3; break;
	}
}

void TCompPlayerModel::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	TCompRenderUI* renderUI = get<TCompRenderUI>();

	transformHandle = get<TCompTransform>();
	colliderHandle = get<TCompCollider>();
	powerGaugeHandle = get<TCompPowerGauge>();
	skeletonHandle = get<TCompSkeleton>();

	respawnPosition = getTransform()->getPosition();

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
		std::string powerProgressBarText = "Power: " + std::to_string((int)getPowerGauge()->getPower()) + "/" + std::to_string((int)getPowerGauge()->getMaxPower());
		ImVec4 color = getPowerGauge()->getPowerLevel() == 1 ? ImColor{ 255, 255, 0 } : getPowerGauge()->getPowerLevel() == 2 ? ImColor{ 255, 255 / 2, 0 } : ImColor{ 255, 0, 0 };
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
		ImGui::ProgressBar((float)getPowerGauge()->getPower() / getPowerGauge()->getMaxPower(), ImVec2(-1, 0), powerProgressBarText.c_str());
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
	{ ActionStates::Death, new DeathActionState(playerModelHandle) },
	{ ActionStates::PitFalling, new PitFallingActionState(playerModelHandle) },
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
	changeBaseState(ActionStates::Idle);
	changeConcurrentState(ActionStates::Idle);
	currentPowerStats = ssj1;

	playerFilterCallback = new PlayerFilterCallback();

}

void TCompPlayerModel::onCollect(const TMsgCollect& msg) {
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
		deltaMovement = baseState->getDeltaMovement();
	}
	else if (concurrentState != concurrentStates[ActionStates::Idle]) {
		deltaMovement = concurrentState->getDeltaMovement();
	}

	applyGravity(dt);
	updateMovement(dt, deltaMovement);

	if (baseState != baseStates[nextBaseState]) {
		changeBaseState(nextBaseState);
	}
	if (concurrentState != concurrentStates[nextConcurrentState]) {
		changeConcurrentState(nextConcurrentState);
	}
}

void TCompPlayerModel::applyGravity(float delta) {
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



void TCompPlayerModel::updateMovement(float delta, VEC3 deltaMovement) {
	PxShape* tempShape;
	getController()->getActor()->getShapes(&tempShape, 1);
	PxFilterData filterData = tempShape->getSimulationFilterData();

	sweep(deltaMovement);

	PxControllerCollisionFlags moveFlags = getCollider()->controller->move(toPhysx(deltaMovement), 0.f, delta,
		PxControllerFilters(&filterData, playerFilterCallback, playerFilterCallback));

	isGrounded = moveFlags.isSet(PxControllerCollisionFlag::eCOLLISION_DOWN);
	//dbg("%d\n", isGrounded);
	if (dynamic_cast<AirborneActionState*>(baseState)) {//NULL si no lo consigue
		if (isGrounded) {
			isTouchingCeiling = false;
			(static_cast<AirborneActionState*>(baseState))->onLanding();
			if (concurrentState != concurrentStates[ActionStates::Idle]) {
				(static_cast<AirborneActionState*>(concurrentState))->onLanding();
			}
		}
		if (!isTouchingCeiling) {
			isTouchingCeiling = moveFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_UP);
			if (isTouchingCeiling) {
				velocityVector.y = -1.f;
			}
		}
	}
	else if (dynamic_cast<GroundedActionState*>(baseState)) {
		if (!isGrounded) {
			if (!isAttachedToPlatform)//What a beautiful hack
				(static_cast<GroundedActionState*>(baseState))->onLeavingGround();
		}
		else {
			//Si sigue en el suelo anulamos la velocidad ganada por la gravedad
			velocityVector.y = 0.f;
		}
	}
}

void TCompPlayerModel::sweep(VEC3 deltaMovement) {
	PxShape* playerShape;
	getController()->getActor()->getShapes(&playerShape, 1);
	PxCapsuleGeometry geometry;
	playerShape->getCapsuleGeometry(geometry);

	PxTransform playerPxTransform = getController()->getActor()->getGlobalPose();
	VEC3 position = fromPhysx(playerPxTransform.p);
	VEC3 nextPosition = position + deltaMovement;
	VEC3 direction = nextPosition - position;
	direction.Normalize();
	float distance = VEC3::Distance(nextPosition, position);

	PxSweepBuffer sweepBuffer;
	PxQueryFilterData fd;
	fd.data = PxFilterData(EnginePhysics.Player, EnginePhysics.Scenario, 0, 0);
	fd.flags |= PxQueryFlag::eANY_HIT | PxQueryFlag::ePREFILTER;
	PxHitFlags hitFlags = PxHitFlag::eMESH_ANY| PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eDISTANCE;

	bool status = EnginePhysics.getScene()->sweep(geometry, playerPxTransform, toPhysx(direction), distance, sweepBuffer,
		hitFlags, fd, EnginePhysics.getGameQueryFilterCallback());
	
	if (status) {
		baseState->onSweep(sweepBuffer);
		if (concurrentState != concurrentStates[ActionStates::Idle]) {
			concurrentState->onSweep(sweepBuffer);
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

TCompTransform* TCompPlayerModel::getTransform() {
	return transformHandle;
}

TCompCollider* TCompPlayerModel::getCollider() {
	return colliderHandle;
}

PxCapsuleController* TCompPlayerModel::getController() {
	return static_cast<PxCapsuleController*>(getCollider()->controller);
}

TCompPowerGauge* TCompPlayerModel::getPowerGauge() {
	return powerGaugeHandle;
}

TCompSkeleton* TCompPlayerModel::getSkeleton() {
	return skeletonHandle;
}

void TCompPlayerModel::setHp(float hp) {
	hp = clamp(hp, 0.f, maxHp);
	this->hp = hp;
	if (this->hp == 0.f) {
		setConcurrentState(TCompPlayerModel::ActionStates::Idle);
		setBaseState(TCompPlayerModel::ActionStates::Death);
	}
}

void TCompPlayerModel::setRespawnPosition(VEC3 position) {
	respawnPosition = position;
}

void TCompPlayerModel::damage(float damage) {
	//TODO Esto lo tendra que procesar el estado en concreto, por si tiene armor o algo
	setHp(hp - damage);
	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
}

TCompCamera* TCompPlayerModel::getCamera() {
	CEntity* camera = (CEntity *)getEntityByName(GAME_CAMERA);
	TCompCamera* currentCamera = camera->get<TCompCamera>();
	assert(currentCamera);
	return currentCamera;
}

void TCompPlayerModel::jumpButtonPressed() {
	if (!lockBaseState) {
		baseState->onJumpHighButton();
	}
	else {
		if (concurrentState != concurrentStates[ActionStates::Idle]) {
			concurrentState->onJumpHighButton();
		}
	}
}

void TCompPlayerModel::jumpButtonReleased() {
	if (!lockBaseState) {
		baseState->onJumpHighButtonReleased();
	}
	else {
		if (concurrentState != concurrentStates[ActionStates::Idle]) {
			concurrentState->onJumpHighButtonReleased();
		}
	}
}

void TCompPlayerModel::longJumpButtonPressed() {
	if (!lockBaseState) {
		baseState->onJumpLongButton();
	}
	else {
		if (concurrentState != concurrentStates[ActionStates::Idle]) {
			concurrentState->onJumpLongButton();
		}
	}
}

void TCompPlayerModel::fastAttackButtonPressed() {
	if (!lockBaseState) {
		baseState->onFastAttackButton();
	}
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->onFastAttackButton();
	}
}

void TCompPlayerModel::fastAttackButtonReleased() {
	if (!lockBaseState) {
		baseState->onFastAttackButtonReleased();
	}
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->onFastAttackButtonReleased();
	}
}

void TCompPlayerModel::strongAttackButtonPressed() {
	if (!lockBaseState) {
		baseState->onStrongAttackButton();
	}
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->onStrongAttackButton();
	}
}

void TCompPlayerModel::strongAttackButtonReleased() {
	if (!lockBaseState) {
		baseState->onStrongAttackButtonReleased();
	}
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->onStrongAttackButtonReleased();
	}
}

void TCompPlayerModel::centerCameraButtonPressed() {
	CEntity* camera = (CEntity*)getEntityByName(PLAYER_CAMERA);
	TCompCameraPlayer* playerCamera = camera->get<TCompCameraPlayer>();
	playerCamera->centerCamera();
}

void TCompPlayerModel::releasePowerButtonPressed() {
	baseState->onReleasePowerButton();
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->onReleasePowerButton();
	}
}

void TCompPlayerModel::gainPowerButtonPressed() {//Debug Only
	getPowerGauge()->gainPower();
}

bool TCompPlayerModel::isConcurrentActionFree() {
	return (concurrentState == concurrentStates[ActionStates::Idle]);
}

void TCompPlayerModel::onAttackHit(const TMsgAttackHit& msg) {
	damage(msg.info.damage);
}

void TCompPlayerModel::onHitboxEnter(const TMsgHitboxEnter& msg) {
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->onHitboxEnter(msg.h_other_entity);
	}
	baseState->onHitboxEnter(msg.h_other_entity);
}

void TCompPlayerModel::onGainPower(const TMsgGainPower& msg) {
	getPowerGauge()->increasePower(msg.power);
}



void TCompPlayerModel::onOutOfBounds(const TMsgOutOfBounds& msg) {
	setConcurrentState(TCompPlayerModel::ActionStates::Idle);
	setBaseState(TCompPlayerModel::ActionStates::PitFalling);
}


void TCompPlayerModel::onShapeHit(const TMsgOnShapeHit& msg) {
	baseState->onShapeHit(msg.hit);
	if (concurrentState != concurrentStates[ActionStates::Idle]) {
		concurrentState->onShapeHit(msg.hit);
	}
	//if (!isGrounded && velocityVector.y < 0.f && msg.hit.actor != lastWallEntered) {
	//	lastWallEntered = msg.hit.actor;

	//	VEC3 hitNormal = VEC3(msg.hit.worldNormal.x, msg.hit.worldNormal.y, msg.hit.worldNormal.z);

	//	//dbg("(%f, %f, %f)\n", msg.hit.worldNormal.x, msg.hit.worldNormal.y, msg.hit.worldNormal.z);
	//	//dbg("%f\n", deg2rad(pitch));

	//	VEC3 worldInput = getCamera()->TransformToWorld(baseState->getMovementInput());
	//	if (worldInput.Dot(-hitNormal) >= attachWallByInputMinDot || getTransform()->getFront().Dot(-hitNormal) >= attachWallByFrontMinDot) {
	//		float pitch = asin(-msg.hit.worldNormal.y);
	//		if (pitch >= huggingWallMinPitch && pitch <= huggingWallMaxPitch) {
	//			HuggingWallActionState* actionState = getBaseState<HuggingWallActionState*>(ActionStates::HuggingWall);
	//			actionState->SetHit(msg.hit);
	//			setBaseState(ActionStates::HuggingWall);
	//		}
	//	}
	//}
}


void TCompPlayerModel::onContact(const TMsgOnContact& msg) {
	//dbg("ON CONTACT\n");
	if (msg.pair.events.isSet(PxPairFlag::eNOTIFY_TOUCH_FOUND)) {
		//dbg("TOUCH FOUND\n");
	}
	if (msg.pair.events.isSet(PxPairFlag::eNOTIFY_TOUCH_LOST)) {
		//dbg("TOUCH LOST\n");
	}

}

TCompPlayerModel::~TCompPlayerModel() {
	SAFE_DELETE(ssj1);
	SAFE_DELETE(ssj2);
	SAFE_DELETE(ssj3);
	for (auto& keyValue : baseStates) {
		SAFE_DELETE(keyValue.second);
	}
	for (auto& keyValue : concurrentStates) {
		SAFE_DELETE(keyValue.second);
	}
	SAFE_DELETE(playerFilterCallback);
}
