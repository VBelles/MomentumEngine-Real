#include "mcv_platform.h"
#include "comp_player_model.h"
#include "components/comp_render_ui.h"
#include "components/comp_respawn_point.h"
#include "components/controllers/comp_camera_player.h"
#include "components/player/states/AirborneActionState.h"
#include "components/player/states/GroundedActionState.h"
#include "components/player/states/DummyActionState.h"
#include "components/player/states/base_states/moving_around/RunActionState.h"
#include "components/player/states/base_states/moving_around/IdleActionState.h"
#include "components/player/states/StateManager.h"
#include "components/player/states/base_states/SpendCoinsActionState.h"

DECL_OBJ_MANAGER("player_model", TCompPlayerModel);

void TCompPlayerModel::registerMsgs() {
	DECL_MSG(TCompPlayerModel, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompPlayerModel, TMsgAttackHit, onAttackHit);
	DECL_MSG(TCompPlayerModel, TMsgHitboxEnter, onHitboxEnter);
	DECL_MSG(TCompPlayerModel, TMsgGainPower, onGainPower);
	DECL_MSG(TCompPlayerModel, TMsgOutOfBounds, onOutOfBounds);
	DECL_MSG(TCompPlayerModel, TMsgPowerLvlChange, onLevelChange);
	DECL_MSG(TCompPlayerModel, TMsgShapeHit, onShapeHit);
	DECL_MSG(TCompPlayerModel, TMsgControllerHit, onControllerHit);
	DECL_MSG(TCompPlayerModel, TMsgRespawnChanged, onRespawnChanged);
	DECL_MSG(TCompPlayerModel, TMsgPurityChange, onPurityChange);
}

void TCompPlayerModel::debugInMenu() {
	ImGui::Text("State: %s", States::toString(getStateManager()->getState()->state));
	ImGui::Text("Concurrent state: %s", States::toString(getStateManager()->getConcurrentState()->concurrentState));

	debugInMenu(powerStats[0], "Ssj1");
	debugInMenu(powerStats[1], "Ssj2");
	debugInMenu(powerStats[2], "Ssj3");
	if (ImGui::DragFloat("Gravity", &accelerationVector.y, 1.f, -1500.f, -0.1f)) {
		baseGravity = accelerationVector.y;
	}
	ImGui::DragFloat("WalkingSpeed", &walkingSpeed, 0.1f, 0.1f, 7.f);
	ImGui::DragFloat("MaxVelocityUpwards", &maxVelocityUpwards, 0.1f, 10.f, 60.f);
	ImGui::DragFloat("InvulnerableTime", &invulnerableTime, 0.1f, 0.1f, 3.f);

	ImGui::DragFloat("Max attack slots", &maxAttackSlots, 0.1f, 0.0f, 50.0f);
	ImGui::Text("Taken attack slots: %f", attackSlotsTaken);
	if (ImGui::TreeNode("Attackers")) {
		for (auto& string : attackers) {
			ImGui::Text("%s\n", string);
		}
		ImGui::TreePop();
	}

	ImGui::DragFloat("damageVisionDefaultTime", &damageVisionDefaultTime, 0.1f, 0.1f, 5.f);
	ImGui::DragFloat("damageVisionPlummetTimePercentage", &damageVisionPlummetTimePercentage, 0.05f, 0.0f, 1.f);
	ImGui::DragFloat("damageVisionStayTimePercentage", &damageVisionStayTimePercentage, 0.05f, 0.0f, 1.f);
	ImGui::DragFloat("damageVisionLowestSaturationLevel", &damageVisionLowestSaturationLevel, 0.05f, -1.0f, damageVisionOriginalSaturationLevel);
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


	if (j.count("particle_systems")) {
		auto& j_particles = j["particle_systems"];
		for (size_t i = 0; i < j_particles.size(); ++i) {
			std::string name_core = j_particles[i];
			particleSystems[i] = name_core;
		}
	}

	if (j.count("materials")) {
		auto& j_mats = j["materials"];
		for (size_t i = 0; i < j_mats.size(); ++i) {
			std::string name_material = j_mats[i];
			materials[i] = name_material;
			dbg("material: %s\n", name_material.c_str());
		}
	}

	if (j.count("locked_states")) {
		auto& j_locked_states = j["locked_states"];
		for (size_t i = 0; i < j_locked_states.size(); ++i) {
			std::string name_state = j_locked_states[i];
			initialLockedStates.push_back(name_state);
			dbg("locked states: %s\n", name_state.c_str());
		}
	}

	if (j.count("locked_concurrent_states")) {
		auto& j_locked_states = j["locked_concurrent_states"];
		for (size_t i = 0; i < j_locked_states.size(); ++i) {
			std::string name_state = j_locked_states[i];
			initialLockedConcurrentStates.push_back(name_state);
			dbg("locked concurrent states: %s\n", name_state.c_str());
		}
	}

	powerStats[0] = loadPowerStats(j["ssj1"]);
	powerStats[1] = loadPowerStats(j["ssj2"]);
	powerStats[2] = loadPowerStats(j["ssj3"]);

	maxAttackSlots = j.value("attack_slots", maxAttackSlots);
	attackSlotsTaken = 0.f;
	attackers.clear();

	refillHp();
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

void TCompPlayerModel::onLevelChange(const TMsgPowerLvlChange& msg) {
	currentPowerStats = powerStats[msg.powerLvl - 1];

	TCompRender *render = get<TCompRender>();
	render->setAllMaterials(0, (int)render->meshes.size() / 2, materials[msg.powerLvl - 1]);

	getStateManager()->getState<RunActionState>(Run)->onLevelChange(msg.powerLvl);
	getStateManager()->getState<IdleActionState>(Idle)->onLevelChange(msg.powerLvl);
	getStateManager()->getState<IdleActionState>(Walk)->onLevelChange(msg.powerLvl);

	EngineScripting.throwEvent(onPowerLevelChange, std::to_string(msg.powerLvl));

	if (msg.powerLvl > msg.prevPowerLvl) {
		Particles::LaunchConfig config{ 
			CHandle(this).getOwner(),	// Entity
			"",							// Bone
			VEC3(0, 0.4f, 0)			// Offset
		};
		getParticles()->launch(msg.powerLvl == 2 ? "lvl_up_2" : "lvl_up_3");
		getSound()->play(msg.powerLvl == 2 ? "lvl_up_2" : "lvl_up_3");
	}
	else {
		getSound()->play("lvl_down");
	}

	getMusicPlayer()->setLevel(static_cast<CMusicPlayer::Level>(msg.powerLvl - 1));
}

void TCompPlayerModel::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	TCompRenderUI* renderUI = get<TCompRenderUI>();

	entityHandle = CHandle(this).getOwner();
	transformHandle = get<TCompTransform>();
	colliderHandle = get<TCompCollider>();
	soundHandle = get<TCompSound>();
	particlesHandle = get<TCompParticles>();
	skeletonHandle = get<TCompSkeleton>();
	renderHandle = get<TCompRender>();
	powerGaugeHandle = get<TCompPowerGauge>();
	collectableManagerHandle = get<TCompCollectableManager>();
	cameraRenderHandle = static_cast<CEntity*>(getEntityByName(GAME_CAMERA))->get<TCompCamera>();
	cameraPlayerHandle = static_cast<CEntity*>(getEntityByName(PLAYER_CAMERA))->get<TCompCameraPlayer>();
	hitboxesHandle = get<TCompHitboxes>();

	float pitch;
	respawnPosition = getTransform()->getPosition();
	getTransform()->getYawPitchRoll(&respawnYaw, &pitch);

	/*renderUI->registerOnRenderUI([&]() {

		bool showWindow = true;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor { 0, 0, 0, 0 });
		ImGui::SetNextWindowPos(ImVec2(50, 35));
		ImGui::SetNextWindowSize(ImVec2(200, 300));
		ImGui::Begin("HpPower", &showWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

		//Hp bar
		std::string hpProgressBarText = "HP: " + std::to_string((int)hp) + "/" + std::to_string((int)maxHp);
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor { 34, 177, 76 });
		//ImGui::ProgressBar((float)hp / maxHp, ImVec2(-1, 0), hpProgressBarText.c_str());
		ImGui::PopStyleColor();

		//Power bar
		std::string powerProgressBarText = "Power";
		ImVec4 color = getPowerGauge()->getPowerLevel() == 1 ? ImColor{ 133, 78, 128 } : getPowerGauge()->getPowerLevel() == 2 ? ImColor{ 24, 174, 186 } : ImColor{ 255, 255, 255 };
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
		//ImGui::ProgressBar(getPowerGauge()->getBarPercentage(), ImVec2(-1, 0), powerProgressBarText.c_str());
		ImGui::PopStyleColor();

		ImGui::End();
		ImGui::PopStyleColor();

		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor { 0, 0, 0, 0 });
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 200 - 50, ImGui::GetIO().DisplaySize.y - 100));
		ImGui::SetNextWindowSize(ImVec2(200, 300));
		ImGui::Begin("Ui", &showWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

		//Chrysalis counter
		std::string chrysalisProgressBarText = "Chrysalis: " + std::to_string(getCollectableManager()->getNumberOfChrysalis()) + "/" + std::to_string(chrysalisTarget);
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor { 255, 191, 0 });
		ImGui::ProgressBar((float)getCollectableManager()->getNumberOfChrysalis() / chrysalisTarget, ImVec2(-1, 0), chrysalisProgressBarText.c_str());
		ImGui::PopStyleColor();

		//Coin counter
		std::string coinText = "Coins: " + std::to_string(getCollectableManager()->getNumberOfCoins());
		ImGui::ProgressBar(0, ImVec2(-1, 0), coinText.c_str());

		ImGui::End();
		ImGui::PopStyleColor();

		if (showVictoryDialog) {
			//-------- WIN DIALOG --------------------------------
			ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor { 0, 0, 0, 120 });
			ImVec2 size = ImVec2(300, 200);
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - size.x / 2,
				100));
			ImGui::SetNextWindowSize(size);
			ImGui::Begin("victoryWindow", &showVictoryDialog, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
			ImGui::TextUnformatted("CONGRATULATIONS!\n\nYou collected enough chrysalis\n\nto open the path!\n\n\n");
			ImGui::TextUnformatted("You can keep exploring and see\n\nif you can collect more.\n");
			ImGui::End();
			ImGui::PopStyleColor();
		}

		if (CApp::get().isDebug()) {
			ImGui::Begin("Params Main Character", &showWindow);
			debugInMenu();
			ImGui::End();
		}

	});*/

	damageVisionOriginalSaturationLevel = cb_globals.global_saturation_adjustment;

	currentPowerStats = powerStats[0];

	stateManager = new StateManager(CHandle(this));
	for (auto& lockedState : initialLockedStates) {
		stateManager->lockState(lockedState);
	}
	for (auto& lockedState : initialLockedConcurrentStates) {
		stateManager->lockConcurrentState(lockedState);
	}
}

void TCompPlayerModel::update(float delta) {
	PROFILE_FUNCTION("update");

	if (!canClimb && disabledClimbingTimer.elapsed() >= 0.05f) {
		canClimb = true;
		TCompCollider* collider = getCollider();
		collider->controller->setSlopeLimit(cosf(deg2rad(collider->config.slope)));
		collider->controller->setStepOffset(collider->config.step);
	}
	if (platformChangeSlopeTimer.elapsed() > 0.5f) {
		TCompCollider* collider = getCollider();
		collider->controller->setSlopeLimit(cosf(deg2rad(collider->config.slope)));
	}

	if (isPlayerRotating) {
		isPlayerRotating = !stateManager->getState()->rotatePlayerTowards(delta, rotatingTargetPos, rotationSpeed);
	}

	if (isInvulnerable && invulnerableTimer.elapsed() >= invulnerableTime) {
		isInvulnerable = false;
	}

	if (showVictoryDialog == true && dialogTimer.elapsed() >= dialogTime) {
		showVictoryDialog = false;
	}

	stateManager->updateStates(delta);
	if (!lockWalk) {
		deltaMovement = stateManager->getState()->getDeltaMovement();
	}
	else {
		deltaMovement = stateManager->getConcurrentState()->getDeltaMovement();
	}

	applyGravity(delta);
	updateMovement(delta, deltaMovement);

	stateManager->performStateChange();

	if (damageVisionTimer.elapsed() < damageVisionTime) {
		float plummetStayTime = damageVisionPlummetTime + damageVisionStayTime;

		if (damageVisionTimer.elapsed() < damageVisionPlummetTime) {
			cb_globals.global_saturation_adjustment =
				lerp(damageVisionOriginalSaturationLevel,
					damageVisionLowestSaturationLevel,
					damageVisionTimer.elapsed() / damageVisionPlummetTime);
		}
		else if(damageVisionTimer.elapsed() < plummetStayTime){
			cb_globals.global_saturation_adjustment = damageVisionLowestSaturationLevel;
		}
		else {
			cb_globals.global_saturation_adjustment =
				lerp(damageVisionLowestSaturationLevel,
					damageVisionOriginalSaturationLevel,
					(damageVisionTimer.elapsed() - plummetStayTime)/(damageVisionTime - plummetStayTime));
		}
	}
	else {
		cb_globals.global_saturation_adjustment = damageVisionOriginalSaturationLevel;
	}

	VEC3 position = PhysxUtils::toVec3(getController()->getFootPosition());
	cb_globals.player_speed = position - cb_globals.player_position;
	cb_globals.player_speed_length = cb_globals.player_speed.Length();
	cb_globals.player_position = position;
}

void TCompPlayerModel::applyGravity(float delta) {
	PROFILE_FUNCTION("applyGravity");
	if (dynamic_cast<GroundedActionState*>(stateManager->getState()) && !wannaJump) {
		float horizontalVelocity = clamp((VEC2(velocityVector.x, velocityVector.z)).Length(), 3.f, 100.f);
		deltaMovement.y -= horizontalVelocity * 2.f * delta;
	}
	else {
		wannaJump = false;
		float deltaMovementDueToGravity = 0.5f * currentGravity * delta * delta;
		deltaMovement.y += deltaMovementDueToGravity;
		//clampear distancia vertical
		deltaMovement.y = deltaMovement.y > maxVerticalSpeed * delta ? maxVerticalSpeed * delta : deltaMovement.y;
	}
	velocityVector.y += currentGravity * delta;
	velocityVector.y = clamp(velocityVector.y, -maxVerticalSpeed, maxVerticalSpeed);
}

void TCompPlayerModel::updateMovement(float delta, VEC3 deltaMovement) {
	PROFILE_FUNCTION("updateMovement");
	moveState = MoveState();
	PxControllerCollisionFlags moveFlags = EnginePhysics.move(getController(), toPxVec3(deltaMovement), delta);
	moveState.isTouchingBot = moveFlags.isSet(PxControllerCollisionFlag::eCOLLISION_DOWN);
	moveState.isTouchingTop = moveFlags.isSet(PxControllerCollisionFlag::eCOLLISION_UP);
	moveState.isTouchingSide = moveFlags.isSet(PxControllerCollisionFlag::eCOLLISION_SIDES);
	PxControllerState state;
	getController()->getState(state);
	moveState.standingShape = state.touchedShape;
	stateManager->getState()->onMove(moveState);
}

void TCompPlayerModel::onShapeHit(const TMsgShapeHit& msg) {
	HitState hitState = HitState(
		getEntity(msg.hit.actor), //Entity
		msg.hit //Hit
	);
	PxCapsuleController* controller = getController();
	PxExtendedVec3 hitPos = msg.hit.worldPos;
	PxExtendedVec3 cctPos = controller->getPosition();
	float halfHeight = controller->getHeight() * 0.5f;
	if (hitPos.y >= cctPos.y + halfHeight) { //Top hit
		moveState.topHits.push_back(hitState);
	}
	else if (hitPos.y <= cctPos.y - halfHeight) { //Bot hit
		moveState.botHits.push_back(hitState);
	}
	else { //Side hit
		moveState.sideHits.push_back(hitState);
	}
	moveState.allHits.push_back(hitState);
}

void TCompPlayerModel::onControllerHit(const TMsgControllerHit& msg) {}

//Aqui llega sin normalizar, se debe hacer justo antes de aplicar el movimiento si se quiere que pueda caminar
void TCompPlayerModel::setMovementInput(VEC2 input, float delta) {
	if (!lockWalk) {
		if (isWalking) {
			input.Normalize();
			input *= 0.5f;
		}
		stateManager->getState()->setMovementInput(input);
	}
	else {
		stateManager->getState()->setMovementInput(VEC2::Zero);
	}
	stateManager->getConcurrentState()->setMovementInput(input);
}

void TCompPlayerModel::setHp(float hp) {
	hp = clamp(hp, 0.f, maxHp);
	this->hp = hp;
	if (this->hp == 0.f && stateManager->getState()->state != PitFalling) {
		stateManager->changeState(Death);
		stateManager->changeConcurrentState(Free);
	}
}

void TCompPlayerModel::setMaxHp(float hp) {
	if (hp > 0) {
		maxHp = hp;
	}
}

void TCompPlayerModel::setRespawnPosition(VEC3 position, float yaw) {
	respawnPosition = position;
	respawnYaw = yaw;
}

void TCompPlayerModel::disableOutline() {
	TCompRender *render = get<TCompRender>();
	for (size_t i = render->meshes.size() / 2; i < render->meshes.size(); ++i) {
		render->meshes[i].enabled = false;
	}
	render->refreshMeshesInRenderManager();
}

void TCompPlayerModel::enableOutline() {
	TCompRender *render = get<TCompRender>();
	for (size_t i = render->meshes.size() / 2; i < render->meshes.size(); ++i) {
		render->meshes[i].enabled = true;
	}
	render->refreshMeshesInRenderManager();
}

void TCompPlayerModel::stopPlayerVelocity() {
	velocityVector = VEC3::Zero;
}

void TCompPlayerModel::rotatePlayerTowards(VEC3 targetPos, float rotationSpeed) {
	isPlayerRotating = true;
	rotatingTargetPos = targetPos;
	this->rotationSpeed = rotationSpeed;
}

void TCompPlayerModel::walkTo(VEC3 targetPosition) {
	VEC3 position = getTransform()->getPosition();
	position.y = 0;
	VEC3 direction = targetPosition - position;
	direction.Normalize();
	velocityVector = direction * walkingSpeed;
	stateManager->getState(Walk)->autoWalk = true;
}

void TCompPlayerModel::disableClimbing() {
	getCollider()->controller->setSlopeLimit(cosf(deg2rad(10)));
	getCollider()->controller->setStepOffset(0.f);
	canClimb = false;
	disabledClimbingTimer.reset();
}

void TCompPlayerModel::onPlatform() {
	getCollider()->controller->setSlopeLimit(cosf(deg2rad(55)));
	platformChangeSlopeTimer.reset();
	isOnPlatform = true;
	//dbg("On platform\n");
}



void TCompPlayerModel::damage(float damage) {
	setHp(hp - damage);
	if (damage > 0.f) {
		getSound()->play("lose_hp");
	}
}

void TCompPlayerModel::makeInvulnerable(float time) {
	invulnerableTime = time;
	isInvulnerable = true;
	invulnerableTimer.reset();
}

void TCompPlayerModel::makeVulnerable() {
	isInvulnerable = false;
}

void TCompPlayerModel::startDamageVision(float time) {
	damageVisionTimer.reset();
	//if (time > 0) {
	//	damageVisionTime = time;
	//}
	//else {
	//	damageVisionTime = damageVisionDefaultTime;
	//}
	damageVisionTime = damageVisionDefaultTime;

	damageVisionPlummetTime = damageVisionTime * damageVisionPlummetTimePercentage;
	damageVisionStayTime = damageVisionTime * damageVisionStayTimePercentage;
}

void TCompPlayerModel::jumpButtonPressed() {
	stateManager->getState()->onJumpHighButton();
	stateManager->getConcurrentState()->onJumpHighButton();
}

void TCompPlayerModel::jumpButtonReleased() {
	stateManager->getState()->onJumpHighButtonReleased();
	stateManager->getConcurrentState()->onJumpHighButtonReleased();
}

void TCompPlayerModel::longJumpButtonPressed() {
	stateManager->getState()->onJumpLongButton();
	stateManager->getConcurrentState()->onJumpLongButton();
}

void TCompPlayerModel::fastAttackButtonPressed() {
	if (!lockAttack) {
		stateManager->getState()->onFastAttackButton();
	}
	stateManager->getConcurrentState()->onFastAttackButton();
}

void TCompPlayerModel::fastAttackButtonReleased() {
	stateManager->getState()->onFastAttackButtonReleased();
	stateManager->getConcurrentState()->onFastAttackButtonReleased();
}

void TCompPlayerModel::strongAttackButtonPressed() {
	if (!lockAttack) {
		stateManager->getState()->onStrongAttackButton();
	}
	stateManager->getConcurrentState()->onStrongAttackButton();
}

void TCompPlayerModel::strongAttackButtonReleased() {
	stateManager->getState()->onStrongAttackButtonReleased();
	stateManager->getConcurrentState()->onStrongAttackButtonReleased();
}

void TCompPlayerModel::centerCameraButtonPressed() {
	CEntity* camera = (CEntity*)getEntityByName(PLAYER_CAMERA);
	TCompCameraPlayer* playerCamera = camera->get<TCompCameraPlayer>();
	playerCamera->centerCamera();
}

void TCompPlayerModel::releasePowerButtonPressed() {
	if (!lockAttack) {
		stateManager->getState()->onReleasePowerButton();
	}
	stateManager->getConcurrentState()->onReleasePowerButton();
}

void TCompPlayerModel::spendCoinsButtonPressed() {
	if (!lockAttack) {
		stateManager->getState()->onSpendCoinsButton();
	}
	stateManager->getConcurrentState()->onSpendCoinsButton();
}

void TCompPlayerModel::spendCoinsButtonReleased() {
	if (!lockAttack) {
		stateManager->getState()->onSpendCoinsButtonReleased();
	}
	stateManager->getConcurrentState()->onSpendCoinsButtonReleased();
}

void TCompPlayerModel::dodgeButtonPressed() {
	if (!lockAttack) {
		stateManager->getState()->onDodgeButton();
	}
	stateManager->getConcurrentState()->onDodgeButton();
}

void TCompPlayerModel::walkButtonPressed() {
	isWalking = !isWalking;
}

void TCompPlayerModel::gainPowerButtonPressed() {//Debug Only
	getPowerGauge()->gainPower();
}

void TCompPlayerModel::onAttackHit(const TMsgAttackHit& msg) {
	if (!isInvulnerable) {
		CEntity* attacker = msg.attacker;
		receivedAttack = msg.info;
		stateManager->getState()->onDamage(msg);
		//lo que diferencia hard de soft es el stun
	}
	else {
		//dbg("hit but invulnerable\n");
	}
}

void TCompPlayerModel::onHitboxEnter(const TMsgHitboxEnter& msg) {
	stateManager->getConcurrentState()->onHitboxEnter(msg.hitbox, msg.h_other_entity);
	stateManager->getState()->onHitboxEnter(msg.hitbox, msg.h_other_entity);
}

void TCompPlayerModel::onGainPower(const TMsgGainPower& msg) {
	getPowerGauge()->increasePower(msg.power);
}

void TCompPlayerModel::onOutOfBounds(const TMsgOutOfBounds& msg) {
	stateManager->changeConcurrentState(Free);
	stateManager->changeState(PitFalling);
}

void TCompPlayerModel::onRespawnChanged(const TMsgRespawnChanged& msg) {
	setRespawnPosition(msg.respawnPosition, msg.respawnYaw);
}

void TCompPlayerModel::onPurityChange(const TMsgPurityChange& msg) {
	getController()->invalidateCache(); //De esta forma no se queda sobre/en colliders estaticos al cambiar de pureza
}

CEntity* TCompPlayerModel::getPlayerEntity() {
	return entityHandle;
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

TCompRender* TCompPlayerModel::getRender() {
	return renderHandle;
}

TCompHitboxes* TCompPlayerModel::getHitboxes() {
	return hitboxesHandle;
}

TCompCamera* TCompPlayerModel::getCameraRender() {
	return cameraRenderHandle;
}

TCompCameraPlayer* TCompPlayerModel::getCameraPlayer() {
	return cameraPlayerHandle;
}

TCompSkeleton* TCompPlayerModel::getSkeleton() {
	return skeletonHandle;
}

TCompPowerGauge* TCompPlayerModel::getPowerGauge() {
	return powerGaugeHandle;
}

TCompSound* TCompPlayerModel::getSound() {
	return soundHandle;
}

TCompParticles* TCompPlayerModel::getParticles() {
	return particlesHandle;
}

TCompCollectableManager* TCompPlayerModel::getCollectableManager() {
	return collectableManagerHandle;
}

CMusicPlayer * TCompPlayerModel::getMusicPlayer() {
	return EngineSound.getMusicPlayer();
}

TCompPlayerModel::~TCompPlayerModel() {
	damageVisionTime = 0.f;
	cb_globals.global_saturation_adjustment = damageVisionOriginalSaturationLevel;

	for (int i = 0; i < NUMBER_OF_POWER_LEVELS; i++) {
		safeDelete(powerStats[i]);
	}
	safeDelete(stateManager);
}

bool TCompPlayerModel::isGrounded() {
	return dynamic_cast<GroundedActionState*>(getStateManager()->getState());
}

int TCompPlayerModel::getNumberOfCoins() {
	return getCollectableManager()->getNumberOfCoins();
}

int TCompPlayerModel::getMaxNumberOfCoins() {
	return getCollectableManager()->getMaxCoins();
}

float TCompPlayerModel::getPowerPerCoin() {
	State state = States::getState("SpendCoins");
	if (state != UndefinedState) {
		return ((SpendCoinsActionState*)getStateManager()->getState(state))->getPowerPerCoin();
	}
	else {
		return -1;
	}
}

bool TCompPlayerModel::addAttacker(std::string attacker, float slots) {
	if (attackSlotsTaken == 0) {
		getMusicPlayer()->setCombat(CMusicPlayer::DANGER);
	}
	if (attackSlotsTaken + slots <= maxAttackSlots) {
		if (attackers.insert(attacker).second) {
			attackSlotsTaken += slots;
			return true;
		}
	}
	return false;
}

void TCompPlayerModel::removeAttacker(std::string attacker, float slots) {
	if (attackers.find(attacker) != attackers.end()) {
		attackSlotsTaken = clamp(attackSlotsTaken - slots, 0.f, maxAttackSlots);
		attackers.erase(attacker);
	}
	if (attackSlotsTaken == 0) {
		getMusicPlayer()->setCombat(CMusicPlayer::OFF);
	}
}

void TCompPlayerModel::lockState(std::string state) {
	getStateManager()->lockState(state);
}

void TCompPlayerModel::lockConcurrentState(std::string state) {
	getStateManager()->lockConcurrentState(state);
}

void TCompPlayerModel::unlockState(std::string state) {
	getStateManager()->unlockState(state);
}

void TCompPlayerModel::changeState(std::string state) {
	getStateManager()->changeState(state);
}

void TCompPlayerModel::changeConcurrentState(std::string state) {
	getStateManager()->changeConcurrentState(state);
}

void TCompPlayerModel::setDummyState(std::string animation, bool isLoop, float duration, std::string exitAnimation) {
	dynamic_cast<DummyActionState*>(stateManager->getState(Dummy))->setDummyState(animation, isLoop, duration, exitAnimation);
}

void TCompPlayerModel::teleport(VEC3 targetPos) {
	float y, p, r;
	getTransform()->getYawPitchRoll(&y, &p, &r);
	setRespawnPosition(targetPos, y);
	getStateManager()->changeState(Teleport);
}