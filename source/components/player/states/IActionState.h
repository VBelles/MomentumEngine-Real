#pragma once

#include "components/comp_collider.h"
#include "components/comp_transform.h"
#include "components/comp_render.h"

class TCompPlayerModel;
class TCompCamera;

using namespace physx;

class IActionState {
protected:
	enum AttackPhases {
		Launch, Startup, Active, Recovery
	};
	IActionState* lastState;
	IActionState* nextState;

	CHandle playerModelHandle;
	CHandle playerTransformHandle;
	CHandle currentCameraHandle;
	CHandle colliderHandle;
	CHandle renderHandle;

	VEC3 deltaMovement;
	VEC2 movementInput;
	VEC3* accelerationVector;
	VEC3* velocityVector;

	TCompPlayerModel* GetPlayerModel() { return playerModelHandle; }
	TCompTransform* GetPlayerTransform() { return playerTransformHandle; }
	TCompCollider* GetCollider() { return colliderHandle; }
	TCompRender* GetRender() { return renderHandle; }

	TCompCamera* GetCamera();

	//Rota hacia targetPos a velocidad rotationSpeed durante el tiempo delta
	void RotatePlayerTowards(float delta, VEC3 targetPos, float rotationSpeed);

	//Factor a baseAcceleration según el ángulo entre baseDirection y desiredDirection
	float CalculateAccelerationAccordingToDirection(VEC3 baseDirection, VEC3 desiredDirection, float baseAcceleration,
		float backwardsMaxDotProduct, float sidewaysMaxDotProduct, float backwardsAirDriftFactor, float sidewaysAirDriftFactor);

	//Calcula el movimiento horizontal recorrido desde el último frame, teniendo en cuanta la posible nueva
	//dirección y lo clampea con la máxima distancia recorrida posible en caso de ir a máxima velocidad
	VEC3 CalculateHorizontalDeltaMovement(float delta, VEC3 lastFrameDirection, VEC3 newDirection,
		float acceleration, float maxSpeed);

	//Se transfiere toda la velocidad que se llevaba a una nueva direcci�n (opcional) y se recalcula velocityVector para el siguiente frame
	void TransferVelocityToDirectionAndAccelerate(float delta, bool wantToTransfer, VEC3 directionOfAcceleration, float acceleration);

	//Clampear velocidad horizontal, usando un VEC2, para no tocar la velocidad vertical
	void ClampHorizontalVelocity(float maxHorizontalSpeed);

public:
	IActionState(CHandle playerModelHandle);
	virtual void update(float delta) = 0;
	virtual void OnStateEnter(IActionState* lastState);
	virtual void OnStateExit(IActionState* nextState);

	virtual void SetPose();

	virtual void setMovementInput(VEC2 input);
	virtual VEC3 GetDeltaMovement() { return deltaMovement; }
	virtual VEC2 GetMovementInput() { return movementInput; }
	virtual void OnJumpHighButton() {}
	virtual void OnJumpHighButtonReleased() {}
	virtual void OnJumpLongButton() {}
	virtual void OnStrongAttackButton() {}
	virtual void OnStrongAttackButtonReleased() {}
	virtual void OnFastAttackButton() {}
	virtual void OnFastAttackButtonReleased() {}
	virtual void OnReleasePowerButton() {}

	virtual void OnShapeHit(const PxControllerShapeHit &hit) {}
	virtual void OnHitboxEnter(CHandle entity) {}

};
