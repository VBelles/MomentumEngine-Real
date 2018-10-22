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
		float ratio = rotationTimer.elapsed() / rotationTime;
		roll = lerp(initialRotation, targetRotation, clamp(ratio, 0.f, 1.f));
		transform->setYawPitchRoll(yaw, pitch, roll);

		auto kinematicActor = static_cast<PxRigidDynamic*>(collider->actor);
		kinematicActor->setKinematicTarget(toPxTransform(transform));
		//si ha acabado
		if (ratio >= 1) {
			isRotating = false;
		}
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
	float yaw, pitch, initialRotation;
	((TCompTransform*)finalDoorTransform)->getYawPitchRoll(&yaw, &pitch, &initialRotation);
	targetRotation = initialRotation + M_PI;
}

void TCompFinalDoorController::onTriggerEnter(const TMsgTriggerEnter & msg) {
	CEntity* entity = msg.h_other_entity;
	if (PLAYER_NAME == entity->getName()) {
		//llamar evento de lua
		EngineSound.getMusicPlayer()->fadeOut(6);
		EngineScripting.throwEvent(finalDoorCutscene, "");
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
	rotationTimer.reset();
}

void TCompFinalDoorController::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
}