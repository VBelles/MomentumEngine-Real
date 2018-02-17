#include "mcv_platform.h"
#include "entity/entity_parser.h"
#include "comp_player_model.h"
#include "player/AirborneActionState.h"


DECL_OBJ_MANAGER("player_model", TCompPlayerModel);

void TCompPlayerModel::debugInMenu() {
	ImGui::DragFloat("Speed", &speedFactor, 0.1f, 0.f, 20.f);
	ImGui::DragFloat("Rotation", &rotationSpeed, 0.1f, 0.f, 20.f);
	ImGui::DragFloat("fallingMultiplier", &fallingMultiplier, 0.01f, 1.f, 2.f);
	ImGui::DragFloat3("jumpVelocity", &jumpVelocity.x, 1.f, -50.f, 1000000.f);
	ImGui::DragFloat3("accelerationVector", &accelerationVector.x, 1.f, -1500.f, -0.1f);
}

void TCompPlayerModel::load(const json& j, TEntityParseContext& ctx) {
	speedFactor = j.value("speed", 10.0f);
	rotationSpeed = j.value("rotation_speed", 11.0f);
	fallingMultiplier = j.value("falling_multiplier", 1.0f);

	if (j.count("initialAcceleration")) accelerationVector = loadVEC3(j["initialAcceleration"]);
	if (j.count("initialVelocity")) velocityVector = loadVEC3(j["initialVelocity"]);
	if (j.count("jumpVelocity")) jumpVelocity = loadVEC3(j["jumpVelocity"]);
	if (j.count("maxVelocity")) maxVelocity = loadVEC3(j["maxVelocity"]);
}


void TCompPlayerModel::registerMsgs() {
	DECL_MSG(TCompPlayerModel, TMsgEntitiesGroupCreated, OnGroupCreated);
}

void TCompPlayerModel::OnGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	myTransform = get<TCompTransform>();

	CEntity *camera = (CEntity *)getEntityByName("xthe_camera");
	currentCamera = camera->get<TCompCamera>();
	assert(currentCamera);

	collider = get<TCompCollider>();
	assert(collider);

	actionStates = {
		{ ActionStates::Airborne, new AirborneActionState(this), }
	};
	actionState = actionStates[ActionStates::Airborne];
}

void TCompPlayerModel::update(float dt) {
	actionState->update(dt);
}

//Aqu� llega sin normalizar, se debe hacer justo antes de aplicar el movimiento si se quiere que pueda caminar
void TCompPlayerModel::SetTranslationInput(VEC2 input, float delta) {
	bool hasInput = input != VEC2::Zero;
	VEC3 desiredDirection = currentCamera->TransformToWorld(input);
	VEC3 targetPos = myTransform->getPosition() + desiredDirection * speedFactor * delta;

	if (hasInput && abs(myTransform->getDeltaYawToAimTo(targetPos)) > 0.01f) {
		float y, p, r;
		myTransform->getYawPitchRoll(&y, &p, &r);
		float yMult = myTransform->isInLeft(targetPos) ? 1.f : -1.f;
		y += rotationSpeed * delta * yMult;
		myTransform->setYawPitchRoll(y, p, r);
	}

	deltaMovement.x = 0;
	deltaMovement.z = 0;
	if (hasInput) {
		deltaMovement = myTransform->getFront() * speedFactor * delta;
	}

	currentGravityMultiplier = velocityVector.y < 0 ? fallingMultiplier : normalGravityMultiplier;
	deltaMovement.y = velocityVector.y * delta + 0.5f * accelerationVector.y * currentGravityMultiplier * delta * delta;
	physx::PxControllerCollisionFlags myFlags = collider->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	velocityVector.y += clamp(accelerationVector.y * delta, -maxVelocity.y, maxVelocity.y);

	grounded = myFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_DOWN);
	if (grounded) velocityVector.y = 0;
}

void TCompPlayerModel::SetRotationInput(float input, float delta) {
	TCompTransform *myTransform = get<TCompTransform>();

	float amountRotated = rotationSpeed * delta;
	float currentYaw = 0.f;
	float currentPitch = 0.f;
	myTransform->getYawPitchRoll(&currentYaw, &currentPitch);
	currentYaw += amountRotated * input;
	myTransform->setYawPitchRoll(currentYaw, currentPitch, 0.f);
}

void TCompPlayerModel::JumpButtonPressed() {
	if (grounded) velocityVector += jumpVelocity;
}



