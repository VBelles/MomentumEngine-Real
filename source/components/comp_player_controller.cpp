#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_controller.h"
#include "comp_transform.h"
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

	VEC2 translationInput = VEC2::Zero;
	float rotationInput = 0.f;

	VEC2 leftAnalogInput = VEC2::Zero;
	leftAnalogInput.x = pad.button(Input::EPadButton::PAD_LANALOG_X).value;
	leftAnalogInput.y = pad.button(Input::EPadButton::PAD_LANALOG_Y).value;

	if (leftAnalogInput.Length() > padDeadZone) {
		//Manda el input del pad
		translationInput = leftAnalogInput;
	}
	else {
		//Detecto el teclado
		if (isPressed('A')) {
			translationInput.x -= 1.f;
		}
		if (isPressed('D')) {
			translationInput.x += 1.f;
		}
		if (isPressed('S')) {
			translationInput.y -= 1.f;
		}
		if (isPressed('W')) {
			translationInput.y += 1.f;
		}
	}

	//Esto es para tirar en un futuro cercano
	if (isPressed('Q')) {
		rotationInput += 1.f;
	}
	if (isPressed('E')) {
		rotationInput -= 1.f;
	}

	playerModel->SetTranslationInput(translationInput, dt);
	playerModel->SetRotationInput(rotationInput, dt);

	//Esto es para tirar en un futuro cercano
	const Input::TButton& bt = CEngine::get().getInput().host(Input::PLAYER_1).keyboard().key(VK_SPACE);
	if (bt.getsPressed() || pad.button(Input::EPadButton::PAD_A).getsPressed()) {
		TEntityParseContext ctx;
		if (parseScene("data/prefabs/bullet.prefab", ctx)) {
			assert(!ctx.entities_loaded.empty());
			// Send the entity who has generated the bullet
			ctx.entities_loaded[0].sendMsg(TMsgAssignBulletOwner{ CHandle(this).getOwner() });
		}
	}
}

