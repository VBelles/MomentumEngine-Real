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
	if (isRotating) {
		TCompCollider* collider = finalDoorCollider;
		TCompTransform* transform = finalDoorTransform;
		float yaw, pitch, roll;
		transform->getYawPitchRoll(&yaw, &pitch, &roll);
		transform->setYawPitchRoll(yaw, pitch + M_PI, roll);
		auto kinematicActor = static_cast<PxRigidDynamic*>(collider->actor);
		kinematicActor->setKinematicTarget(toPxTransform(transform));
		//si ha acabado
		isRotating = false;
		dbg("Rotating\n");
	}
}

void TCompFinalDoorController::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
	std::string name = ((CEntity*)CHandle(this).getOwner())->getName();
	UniqueElement* uniqueEvent = EngineUniques.getUniqueEvent(name);
	if (uniqueEvent && uniqueEvent->done) {
		((TCompCollider*)get<TCompCollider>())->create();
	}
	CEntity* entity = getEntityByName(finalDoorName);
	finalDoorCollider = entity->get<TCompCollider>();
	finalDoorTransform = entity->get<TCompTransform>();
}

void TCompFinalDoorController::onTriggerEnter(const TMsgTriggerEnter & msg) {
	CEntity* entity = msg.h_other_entity;
	if (PLAYER_NAME == entity->getName()) {
		//lanzar lo que sea
		dbg("Final door trigger entered\n");
		//llamar evento de lua

		//quitar, esto se tiene que llamar desde lua
		rotateFinalDoor();
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

void TCompFinalDoorController::rotateFinalDoor() {
	isRotating = true;
}

void TCompFinalDoorController::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
}