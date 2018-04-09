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
	panningSpeed = j.value("panningSpeed", 12.f);
	timeToMixOut = j.value("timeToMixOut", 1.f);
	modifyPlayerCameraRotation = j.value("modifyPlayerCameraRotation", true);
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
	cubicOutInterpolator = new TCubicOutInterpolator();
	TCompTransform* transform = transformHandle;
	originalPosition = transform->getPosition();
	leftPosition = originalPosition + transform->getLeft() * positionRange;
	rightPosition = originalPosition - transform->getLeft() * positionRange;
	upperPosition = originalPosition + transform->getUp() * positionRange;
	upperPosition = originalPosition - transform->getUp() * positionRange;
}

void TCompFixedCamera::update(float delta) {	
	TCompTransform* transform = getTransform();
	updateInput();
	if (!countingForReposition) {
		if (input == VEC2::Zero) {
			countingForReposition = true;
			repositionTimer.reset();
		}
	}
	else {
		if (input != VEC2::Zero) {
			countingForReposition = false;
			isRepositioning = false;
		}
		else if(!isRepositioning && repositionTimer.elapsed() >= repositionTime){
			isRepositioning = true;
		}
	}
	if (isRepositioning) {
		if (panningSpeed * delta < VEC3::Distance(originalPosition, transform->getPosition())) {
			VEC3 returnDirection = originalPosition - transform->getPosition();
			transform->setPosition(transform->getPosition() + returnDirection * panningSpeed * delta);
		}
		else {
			transform->setPosition(originalPosition);
		}
	}
	else if (!returnToPlayerCameraWithInput) {
		
		input.Normalize();
		VEC3 verticalMovement = transform->getUp() * input.y;
		VEC3 horizontalMovement = transform->getLeft() * input.x;
		VEC3 newPos = transform->getPosition() + (verticalMovement + horizontalMovement) * panningSpeed * delta;
		
		transform->setPosition(newPos);
	}
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
		if (returnToPlayerCameraWithInput && (padInput.Length() > PAD_DEAD_ZONE || mouse.position_delta != VEC2::Zero || EngineInput["center_camera"].getsPressed())) {
			if (modifyPlayerCameraRotation) {
				CopyRotationFromMixedCameraToPlayerCamera();
			}
			CHandle playerCameraHandle = getEntityByName(PLAYER_CAMERA);
			Engine.getCameras().blendInCamera(playerCameraHandle, timeToMixOut, CModuleCameras::EPriority::GAMEPLAY, cubicOutInterpolator);
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

void TCompFixedCamera::CopyRotationFromMixedCameraToPlayerCamera() {
	CHandle leavingCameraHandle = CHandle(this).getOwner();
	CEntity* leavingCameraEntity = leavingCameraHandle;
	TCompTransform* leavingCameraTransform = leavingCameraEntity->get<TCompTransform>();
	CHandle playerCameraHandle = getEntityByName(PLAYER_CAMERA);
	CEntity* playerCameraEntity = playerCameraHandle;
	TCompTransform* playerCameraTransform = playerCameraEntity->get<TCompTransform>();
	//playerCameraTransform->setPosition(leavingCameraTransform->getPosition());
	float yPLayer, pPlayer, rPlayer;
	playerCameraTransform->getYawPitchRoll(&yPLayer, &pPlayer, &rPlayer);
	float yLeavingCamera, pLeavingCamera, rLeavingCamera;
	leavingCameraTransform->getYawPitchRoll(&yLeavingCamera, &pLeavingCamera, &rLeavingCamera);
	playerCameraTransform->setYawPitchRoll(yLeavingCamera, pLeavingCamera, rPlayer);
}