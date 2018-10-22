#include "mcv_platform.h"
#include "comp_final_door_controller.h"

DECL_OBJ_MANAGER("final_door_controller", TCompFinalDoorController);

void TCompFinalDoorController::debugInMenu() {
}

void TCompFinalDoorController::registerMsgs() {
	DECL_MSG(TCompFinalDoorController, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompFinalDoorController, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompFinalDoorController, TMsgTriggerExit, onTriggerExit);
	DECL_MSG(TCompFinalDoorController, TMsgColliderDestroyed, onColliderDestroyed);
	DECL_MSG(TCompFinalDoorController, TMsgChrysalisCollected, onChrysalisCollected);
}

void TCompFinalDoorController::load(const json & j, TEntityParseContext & ctx) {
	finalDoorName = j.value("final_door_name", "");
}

void TCompFinalDoorController::update(float delta){
}

void TCompFinalDoorController::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
	std::string name = ((CEntity*)CHandle(this).getOwner())->getName();
	UniqueElement* uniqueEvent = EngineUniques.getUniqueEvent(name);
	if (uniqueEvent && uniqueEvent->done) {
		((TCompCollider*)get<TCompCollider>())->create();
	}
}

void TCompFinalDoorController::onTriggerEnter(const TMsgTriggerEnter & msg) {
	CEntity* entity = msg.h_other_entity;
	if (PLAYER_NAME == entity->getName()) {
		//lanzar lo que sea
		dbg("Final door trigger entered\n");
	}
}

void TCompFinalDoorController::onTriggerExit(const TMsgTriggerExit & msg) {
}

void TCompFinalDoorController::onChrysalisCollected(const TMsgChrysalisCollected & msg) {
	if (msg.numberOfChrysalidesCollected >= CHRYSALIS_TARGET_NUMBER) {
		std::string name = ((CEntity*)CHandle(this).getOwner())->getName();
		UniqueElement* uniqueEvent = EngineUniques.getUniqueEvent(name);
		if (uniqueEvent && !uniqueEvent->done) {
			((TCompCollider*)get<TCompCollider>())->create();
			EngineUniques.setEventTriggered(name, true);
		}
	}
}

void TCompFinalDoorController::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
}