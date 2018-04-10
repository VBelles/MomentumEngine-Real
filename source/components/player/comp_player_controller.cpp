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
		playerModel->JumpButtonPressed();
	}
	else if (EngineInput["jump"].getsReleased()) {
		playerModel->JumpButtonReleased();
	}
	if (EngineInput["long_jump"].getsPressed()) {
		playerModel->LongJumpButtonPressed();
	}
	if (EngineInput["fast_attack"].getsPressed()) {
		playerModel->FastAttackButtonPressed();
	}
	else if (EngineInput["fast_attack"].getsReleased()) {
		playerModel->FastAttackButtonReleased();
	}
	if (EngineInput["strong_attack"].getsPressed()) {
		playerModel->StrongAttackButtonPressed();
	}
	else if (EngineInput["strong_attack"].getsReleased()) {
		playerModel->StrongAttackButtonReleased();
	}
	if (EngineInput["center_camera"].getsPressed()) {
		playerModel->CenterCameraButtonPressed();
	}
	if (EngineInput["release_power"].getsPressed()) {
		playerModel->ReleasePowerButtonPressed();
	}
	if (EngineInput["gain_power"].getsPressed()) {
		playerModel->GainPowerButtonPressed();
	}
	//Dejar este para el final
	playerModel->SetMovementInput(translationInput, delta);
}
