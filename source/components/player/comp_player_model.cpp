#include "mcv_platform.h"
#include "comp_player_model.h"
#include "components/comp_render_ui.h"
#include "components/comp_render.h"
#include "components/comp_camera.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_collectable.h"
#include "components/comp_respawn_point.h"
#include "components/controllers/comp_camera_player.h"
#include "components/player/comp_power_gauge.h"
#include "components/player/comp_collectable_manager.h"
#include "components/player/states/AirborneActionState.h"
#include "components/player/states/GroundedActionState.h"
#include "components/player/states/base_states/moving_around/RunActionState.h"
#include "components/player/states/StateManager.h"

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

	if (j.count("materials")) {
		auto& j_mats = j["materials"];
		for (size_t i = 0; i < j_mats.size(); ++i) {
			std::string name_material = j_mats[i];
			materials[i] = name_material;
			dbg("material: %s\n", name_material.c_str());
		}
	}
	powerStats[0] = loadPowerStats(j["ssj1"]);
	powerStats[1] = loadPowerStats(j["ssj2"]);
	powerStats[2] = loadPowerStats(j["ssj3"]);

	resetHp();
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

	getStateManager()->getState<RunActionState>(Run)->setAnimation(msg.powerLvl);

	EngineScripting.throwEvent(onPowerLevelChange, std::to_string(msg.powerLvl));
}

void TCompPlayerModel::onAllScenesCreated(const TMsgAllScenesCreated& msg) {
	TCompRenderUI* renderUI = get<TCompRenderUI>();

	transformHandle = get<TCompTransform>();
	colliderHandle = get<TCompCollider>();
	powerGaugeHandle = get<TCompPowerGauge>();
	collectableManagerHandle = get<TCompCollectableManager>();

	respawnPosition = getTransform()->getPosition();

	renderUI->registerOnRenderUI([&]() {

		bool showWindow = true;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor { 0, 0, 0, 0 });
		ImGui::SetNextWindowPos(ImVec2(50, 35));
		ImGui::SetNextWindowSize(ImVec2(200, 300));
		ImGui::Begin("HpPower", &showWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

		//Hp bar
		std::string hpProgressBarText = "HP: " + std::to_string((int)hp) + "/" + std::to_string((int)maxHp);
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, (ImVec4)ImColor { 34, 177, 76 });
		ImGui::ProgressBar((float)hp / maxHp, ImVec2(-1, 0), hpProgressBarText.c_str());
		ImGui::PopStyleColor();

		//Power bar
		std::string powerProgressBarText = "Power";
		ImVec4 color = getPowerGauge()->getPowerLevel() == 1 ? ImColor{ 133, 78, 128 } : getPowerGauge()->getPowerLevel() == 2 ? ImColor{ 24, 174, 186 } : ImColor{ 255, 255, 255 };
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
		ImGui::ProgressBar(getPowerGauge()->getBarPercentage(), ImVec2(-1, 0), powerProgressBarText.c_str());
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
			ImGui::TextUnformatted("CONGRATULATIONS!\n\nYou collected enough chrysalis\n\nto open the path to the final boss!\n\n\n");
			ImGui::TextUnformatted("You can keep exploring and see\n\nif you can collect the other two.\n");
			ImGui::End();
			ImGui::PopStyleColor();
		}

		if (CApp::get().isDebug()) {
			ImGui::Begin("Params Main Character", &showWindow);
			debugInMenu();
			ImGui::End();
		}

	});

	currentPowerStats = powerStats[0];

	stateManager = new StateManager(CHandle(this).getOwner());

}

void TCompPlayerModel::update(float delta) {
	PROFILE_FUNCTION("update");
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
	if (this->hp == 0.f && !stateManager->getState()->state == PitFalling) {
		stateManager->changeState(Death);
		stateManager->changeConcurrentState(Free);
	}
	//EngineGUI.getVariables().getVariant("hp_progress")->setFloat(hp / maxHp);
	//EngineGUI.getVariables().getVariant("hp")->setInt(hp);
}

void TCompPlayerModel::setRespawnPosition(VEC3 position) {
	respawnPosition = position;
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

void TCompPlayerModel::damage(float damage) {//tendría que llegar también si es hard o no
	setHp(hp - damage);
	TCompRender* render = get<TCompRender>();
	render->TurnRed(0.5f);
}

void TCompPlayerModel::makeInvulnerable(float time) {
	invulnerableTime = time;
	isInvulnerable = true;
	invulnerableTimer.reset();
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
		receivedAttack = msg.info;
		stateManager->getState()->onDamage(msg);
		//lo que diferencia hard de soft es el stun
	}
	else {
		dbg("hit but invulnerable\n");
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
	setRespawnPosition(msg.respawnPosition);
}

void TCompPlayerModel::onPurityChange(const TMsgPurityChange& msg) {
	getController()->invalidateCache(); //De esta forma no se queda sobre/en colliders estaticos al cambiar de pureza
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

TCompCollectableManager* TCompPlayerModel::getCollectableManager() {
	return collectableManagerHandle;
}

TCompPlayerModel::~TCompPlayerModel() {
	for (int i = 0; i < NUMBER_OF_POWER_LEVELS; i++) {
		SAFE_DELETE(powerStats[i]);
	}
	SAFE_DELETE(stateManager);
}