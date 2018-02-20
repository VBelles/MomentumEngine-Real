#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_controller.h"
#include "comp_transform.h"
#include "comp_collider.h"
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
		if (EngineInput["kb_a"].isPressed()) {
			translationInput.x -= 1.f;
		}
		if (EngineInput["kb_d"].isPressed()) {
			translationInput.x += 1.f;
		}
		if (EngineInput["kb_s"].isPressed()) {
			translationInput.y -= 1.f;
		}
		if (EngineInput["kb_w"].isPressed()) {
			translationInput.y += 1.f;
		}
	}

	if (EngineInput["jump"].getsPressed()) {
		playerModel->JumpButtonPressed();
	}

	if (EngineInput["center_camera"].getsPressed()) {
		playerModel->CenterCameraButtonPressed();
	}

	playerModel->SetMovementInput(translationInput, dt);//Dejar este para el final
}
