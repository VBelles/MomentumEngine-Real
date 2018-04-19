#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_controller.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "entity/common_msgs.h"
#include "comp_player_model.h"
#include "game_constants.h"

DECL_OBJ_MANAGER("player_controller", TCompPlayerController);

void TCompPlayerController::registerMsgs() {
	DECL_MSG(TCompPlayerController, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompPlayerController::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	playerModelHandle = get<TCompPlayerModel>();
	assert(playerModelHandle.isValid());
}


void TCompPlayerController::update(float delta) {
	TCompPlayerModel* playerModel = getPlayerModel();
	VEC2 translationInput = VEC2::Zero;
	VEC2 leftAnalogInput = VEC2(
		EngineInput[Input::EPadButton::PAD_LANALOG_X].value,
		EngineInput[Input::EPadButton::PAD_LANALOG_Y].value
	);
	if (leftAnalogInput.Length() > PAD_DEAD_ZONE) { //Manda el input del pad
		translationInput = leftAnalogInput;
	}
	else { //Detecto el teclado
		if (EngineInput["left"].isPressed()) {
			translationInput.x -= 1.f;
		}
		if (EngineInput["right"].isPressed()) {
			translationInput.x += 1.f;
		}
		if (EngineInput["backwards"].isPressed()) {
			translationInput.y -= 1.f;
		}
		if (EngineInput["forward"].isPressed()) {
			translationInput.y += 1.f;
		}
		if (EngineInput["walk"].isPressed()) {
			translationInput.Normalize();
			translationInput *= 0.5f;
		}
	}

	if (EngineInput["jump"].getsPressed()) {
		playerModel->jumpButtonPressed();
	}
	else if (EngineInput["jump"].getsReleased()) {
		playerModel->jumpButtonReleased();
	}
	if (EngineInput["long_jump"].getsPressed()) {
		playerModel->longJumpButtonPressed();
	}
	if (EngineInput["fast_attack"].getsPressed()) {
		playerModel->fastAttackButtonPressed();
	}
	else if (EngineInput["fast_attack"].getsReleased()) {
		playerModel->fastAttackButtonReleased();
	}
	if (EngineInput["strong_attack"].getsPressed()) {
		playerModel->strongAttackButtonPressed();
	}
	else if (EngineInput["strong_attack"].getsReleased()) {
		playerModel->strongAttackButtonReleased();
	}
	if (EngineInput["center_camera"].getsPressed()) {
		playerModel->centerCameraButtonPressed();
	}
	if (EngineInput["release_power"].getsPressed()) {
		playerModel->releasePowerButtonPressed();
	}
	if (EngineInput["gain_power"].getsPressed()) {
		playerModel->gainPowerButtonPressed();
	}
	playerModel->setMovementInput(translationInput, delta);
}

TCompPlayerModel * TCompPlayerController::getPlayerModel() {
	return playerModelHandle;
}