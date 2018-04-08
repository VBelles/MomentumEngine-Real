#include "mcv_platform.h"
#include "comp_fixed_camera.h"
#include "components/comp_camera.h"
#include "components/comp_transform.h"
#include "game_constants.h"

DECL_OBJ_MANAGER("fixed_camera", TCompFixedCamera);

void TCompFixedCamera::debugInMenu() {
}

// -------------------------------------------------
void TCompFixedCamera::renderDebug() {
}

// -------------------------------------------------
void TCompFixedCamera::load(const json& j, TEntityParseContext& ctx) {
	returnToPlayerCameraWithInput = j.value("returnToPlayerCameraWithInput", false);
	panningSpeed = j.value("panningSpeed", 1.f);
	timeToMixOut = j.value("timeToMixOut", 1.f);
}

void TCompFixedCamera::registerMsgs() {
	DECL_MSG(TCompFixedCamera, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompFixedCamera, TMsgLockCameraInput, onLockCameraInput);
}

void TCompFixedCamera::onLockCameraInput(const TMsgLockCameraInput & msg) {
	isMovementLocked = msg.isLocked;
}

void TCompFixedCamera::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	transformHandle = get<TCompTransform>();
	assert(transformHandle.isValid());
}

void TCompFixedCamera::update(float delta) {	
	updateInput();
}

void TCompFixedCamera::updateInput() {
	input = VEC2::Zero;
	//Hacer sólo si la cámara está mixeada
	if (!isMovementLocked) {
		VEC2 padInput = {
			EngineInput[Input::EPadButton::PAD_RANALOG_X].value,
			EngineInput[Input::EPadButton::PAD_RANALOG_Y].value
		};
		auto& mouse = EngineInput[Input::PLAYER_1].mouse();
		if (returnToPlayerCameraWithInput && (padInput.Length() > PAD_DEAD_ZONE || mouse.position_delta != VEC2::Zero)) {
			CHandle playerCameraHandle = getEntityByName(PLAYER_CAMERA);
			Engine.getCameras().blendInCamera(playerCameraHandle, timeToMixOut, CModuleCameras::EPriority::GAMEPLAY);
			isMovementLocked = true;
		}
		else {
			if (padInput.Length() > PAD_DEAD_ZONE) {
				input.x -= padInput.x * panningSpeed;
				input.y += padInput.y * panningSpeed;
			}
			else if (!CApp::get().showDebug) {
				input.x -= mouse.position_delta.x * panningSpeed;
				input.y -= mouse.position_delta.y * panningSpeed;
			}
		}
	}
}

TCompTransform* TCompFixedCamera::getTransform() {
	return transformHandle;
}