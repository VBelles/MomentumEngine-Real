#include "mcv_platform.h"
#include "IActionState.h"

TCompCamera* IActionState::GetCamera() {
	CEntity* camera = (CEntity *)getEntityByName("game_camera");
	TCompCamera* currentCamera = camera->get<TCompCamera>();
	assert(currentCamera);
	return currentCamera;
}

void IActionState::RotatePlayerTowards(float delta, VEC3 targetPos, float rotationSpeed) {
	if (abs(GetPlayerTransform()->getDeltaYawToAimTo(targetPos)) > 0.01f) {
		float y, p, r;
		GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
		float yMult = GetPlayerTransform()->isInLeft(targetPos) ? 1.f : -1.f;
		y += rotationSpeed * delta * yMult;
		GetPlayerTransform()->setYawPitchRoll(y, p, r);
	}
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

VEC3 IActionState::CalculateHorizontalDeltaMovement(
	float delta, VEC3 lastFrameDirection, VEC3 newDirection,
	float acceleration, float maxSpeed) {
	//Copiamos los ejes x/z del vector de velocidad en un VEC2, para trabajar sin tocar el eje y
	VEC2 horizontalVelocity = { velocityVector->x , velocityVector->z };
	float currentSpeed = horizontalVelocity.Length();
	lastFrameDirection.Normalize();
	newDirection.Normalize();
	//Calculamos la distancia recorrida desde el �ltimo frame 
	VEC3 resultingDeltaMovement = lastFrameDirection * currentSpeed * delta + 0.5f * newDirection * acceleration * delta * delta;
	//Si ha recorrido m�s distancia que lo m�ximo permitido por la velocidad m�xima, lo clampeamos a (maxSpeed * delta)
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
		//Pasamos toda la velocidad que llev�bamos a la nueva direcci�n
		directionOfAcceleration.Normalize();
		horizontalVelocity.x = directionOfAcceleration.x * currentSpeed;
		horizontalVelocity.y = directionOfAcceleration.z * currentSpeed;
	}

	//Actualizamos el vector de velocidad con la aceleraci�n
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

float IActionState::CalculateVerticalDeltaMovement(float delta, float acceleration, float maxVelocityVertical) {
	float resultingDeltaMovement;
	resultingDeltaMovement = velocityVector->y * delta + 0.5f * acceleration * delta * delta;
	//clampear distancia vertical
	resultingDeltaMovement = resultingDeltaMovement > maxVelocityVertical * delta ? maxVelocityVertical * delta : resultingDeltaMovement;
	return resultingDeltaMovement;
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

void IActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}
