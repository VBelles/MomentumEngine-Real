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
	auto& pad = CEngine::get().getInput().host(Input::PLAYER_1).pad();
	auto& keyboard = CEngine::get().getInput().host(Input::PLAYER_1).keyboard();

	VEC2 translationInput = VEC2::Zero;

	VEC2 leftAnalogInput = VEC2::Zero;
	leftAnalogInput.x = pad.button(Input::EPadButton::PAD_LANALOG_X).value;
	leftAnalogInput.y = pad.button(Input::EPadButton::PAD_LANALOG_Y).value;

	if (leftAnalogInput.Length() > padDeadZone) {
		//Manda el input del pad
		translationInput = leftAnalogInput;
	}
	else {
		//Detecto el teclado
		if (keyboard.key(0x41).isPressed()) { //A
			translationInput.x -= 1.f;
		}
		if (keyboard.key(0x44).isPressed()) { //D
			translationInput.x += 1.f;
		}
		if (keyboard.key(0x53).isPressed()) { //S
			translationInput.y -= 1.f;
		}
		if (keyboard.key(0x57).isPressed()) { //W
			translationInput.y += 1.f;
		}
	}

	if (pad.button(Input::EPadButton::PAD_A).getsPressed() ||
		keyboard.key(VK_SPACE).getsPressed()) {
		playerModel->JumpButtonPressed();
	}

	dbg("Input: (%f, %f)\n", translationInput.x, translationInput.y);

	playerModel->SetTranslationInput(translationInput, dt);
}
