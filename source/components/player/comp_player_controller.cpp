#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_controller.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("player_controller", TCompPlayerController);

void TCompPlayerController::debugInMenu() {

}

void TCompPlayerController::load(const json& j, TEntityParseContext& ctx) {

}

void TCompPlayerController::registerMsgs() {
	DECL_MSG(TCompPlayerController, TMsgEntitiesGroupCreated, OnGroupCreated);
}

void TCompPlayerController::OnGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	playerModel = get<TCompPlayerModel>();
	assert(playerModel);
}

void TCompPlayerController::update(float dt) {
	assert(playerModel);
	VEC2 translationInput = VEC2::Zero;
	VEC2 leftAnalogInput = VEC2::Zero;
	float leftTrigger = 0.f;
    leftAnalogInput.x = EngineInput[Input::EPadButton::PAD_LANALOG_X].value;
    leftAnalogInput.y = EngineInput[Input::EPadButton::PAD_LANALOG_Y].value;

	if (leftAnalogInput.Length() > padDeadZone) {
		//Manda el input del pad
		translationInput = leftAnalogInput;
	}
	else {
		//Detecto el teclado
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
	} else if (EngineInput["jump"].getsReleased()) {
		playerModel->JumpButtonReleased();
	}
	if (EngineInput["long_jump"].getsPressed()) {
		playerModel->LongJumpButtonPressed();
	}
	if (EngineInput["fast_attack"].getsPressed()) {
		playerModel->FastAttackButtonPressed();
	}else if (EngineInput["fast_attack"].getsReleased()) {
		playerModel->FastAttackButtonReleased();
	}
	if (EngineInput["strong_attack"].getsPressed()) {
		playerModel->StrongAttackButtonPressed();
	}else if (EngineInput["strong_attack"].getsReleased()) {
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
	if (EngineInput["pause"].getsPressed()) {
		//playerModel->PauseButtonPressed();
	}

	playerModel->SetMovementInput(translationInput, dt);//Dejar este para el final
}
