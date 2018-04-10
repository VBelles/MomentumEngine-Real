#include "mcv_platform.h"
#include "IActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_camera.h"

IActionState::IActionState(CHandle playerModelHandle) {
	this->playerModelHandle = playerModelHandle;
	CEntity* playerEntity = playerModelHandle.getOwner();
	this->colliderHandle = playerEntity->get<TCompCollider>();
	this->playerTransformHandle = playerEntity->get<TCompTransform>();
	this->renderHandle = playerEntity->get<TCompRender>();
}

void IActionState::OnStateEnter(IActionState* lastState) {
	this->lastState = lastState;
	deltaMovement = VEC3::Zero;
	movementInput = VEC2::Zero;
}
void IActionState::OnStateExit(IActionState* nextState) {
	this->nextState = nextState;
}

void IActionState::SetPose() {
	GetRender()->setMesh("data/meshes/pose_idle.mesh");
}

void IActionState::setMovementInput(VEC2 input) {
	movementInput = input;
}

TCompCamera* IActionState::getCamera() {
	CEntity* camera = (CEntity *)getEntityByName("game_camera");
	TCompCamera* currentCamera = camera->get<TCompCamera>();
	assert(currentCamera);
	return currentCamera;
}

void IActionState::RotatePlayerTowards(float delta, VEC3 targetPos, float rotationSpeed) {
    float rotationIncrement = rotationSpeed * delta;
    float deltaYaw = GetPlayerTransform()->getDeltaYawToAimTo(targetPos);
    float y, p, r;
    GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
    if (abs(deltaYaw) >= rotationIncrement) {
        y = (deltaYaw > 0) ? (y + rotationIncrement) : (y - rotationIncrement);
    }
    else {
        y += deltaYaw;
    }
    GetPlayerTransform()->setYawPitchRoll(y, p, r);
}

float IActionState::CalculateAccelerationAccordingToDirection(VEC3 baseDirection, VEC3 desiredDirection, float baseAcceleration,
	float backwardsMaxDotProduct, float sidewaysMaxDotProduct, float backwardsAirDriftFactor, float sidewaysAirDriftFactor) {
	float resultingAcceleration = baseAcceleration;
	if (baseDirection.Dot(desiredDirection) <= backwardsMaxDotProduct) {
		resultingAcceleration *= backwardsAirDriftFactor;
	}
	else if (baseDirection.Dot(desiredDirection) <= sidewaysMaxDotProduct) {
		resultingAcceleration *= sidewaysAirDriftFactor;
	}
	return resultingAcceleration;
}

VEC3 IActionState::CalculateHorizontalDeltaMovement(float delta, VEC3 lastFrameDirection, VEC3 newDirection,
	float acceleration, float maxSpeed) {
	//Copiamos los ejes x/z del vector de velocidad en un VEC2, para trabajar sin tocar el eje y
	VEC2 horizontalVelocity = { velocityVector->x , velocityVector->z };
	float currentSpeed = horizontalVelocity.Length();
	lastFrameDirection.Normalize();
	newDirection.Normalize();
	//Calculamos la distancia recorrida desde el último frame 
	VEC3 resultingDeltaMovement = lastFrameDirection * currentSpeed * delta + 0.5f * newDirection * acceleration * delta * delta;
	//Si ha recorrido más distancia que lo máximo permitido por la velocidad máxima, lo clampeamos a (maxSpeed * delta)
	if (resultingDeltaMovement.Length() > maxSpeed * delta) {
		resultingDeltaMovement.Normalize();
		resultingDeltaMovement *= maxSpeed * delta;
	}
	return resultingDeltaMovement;
}

void IActionState::TransferVelocityToDirectionAndAccelerate(float delta, bool wantToTransfer, VEC3 directionOfAcceleration, float acceleration) {
	//Copiamos los ejes x/z del vector de velocidad en un VEC2, para trabajar sin tocar el eje y
	VEC2 horizontalVelocity = { velocityVector->x , velocityVector->z };
	float currentSpeed = horizontalVelocity.Length();
	if (wantToTransfer) {
		//Pasamos toda la velocidad que llevábamos a la nueva dirección
		directionOfAcceleration.Normalize();
		horizontalVelocity.x = directionOfAcceleration.x * currentSpeed;
		horizontalVelocity.y = directionOfAcceleration.z * currentSpeed;
	}

	//Actualizamos el vector de velocidad con la aceleración
	velocityVector->x = horizontalVelocity.x + directionOfAcceleration.x * acceleration * delta;
	velocityVector->z = horizontalVelocity.y + directionOfAcceleration.z * acceleration * delta;
}

void IActionState::ClampHorizontalVelocity(float maxHorizontalSpeed) {
	VEC2 horizontalVelocity;
	horizontalVelocity.x = velocityVector->x;
	horizontalVelocity.y = velocityVector->z;
	if (horizontalVelocity.Length() > maxHorizontalSpeed) {
		horizontalVelocity.Normalize();
		horizontalVelocity *= maxHorizontalSpeed;
		velocityVector->x = horizontalVelocity.x;
		velocityVector->z = horizontalVelocity.y;
	}
}
