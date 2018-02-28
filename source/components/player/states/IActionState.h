#pragma once

#include "mcv_platform.h"
#include "components/comp_camera.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "components/comp_transform.h"


class TCompPlayerModel;

class IActionState {
protected:
	IActionState* lastState;
	IActionState* nextState;

	CHandle playerHandle;
	CHandle playerTransformHandle;
	CHandle currentCameraHandle;
	CHandle colliderHandle;

	VEC3 deltaMovement;
	VEC2 movementInput;
	VEC3* accelerationVector;
	VEC3* velocityVector;

	TCompPlayerModel* GetPlayer() { return playerHandle; }
	TCompTransform* GetPlayerTransform() { return playerTransformHandle; }
	TCompCollider* GetCollider() { return colliderHandle; }

	//Rota hacia targetPos a velocidad rotationSpeed durante el tiempo delta
	void RotatePlayerTowards(float delta, VEC3 targetPos, float rotationSpeed) {
		if (abs(GetPlayerTransform()->getDeltaYawToAimTo(targetPos)) > 0.01f) {
			float y, p, r;
			GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
			float yMult = GetPlayerTransform()->isInLeft(targetPos) ? 1.f : -1.f;
			y += rotationSpeed * delta * yMult;
			GetPlayerTransform()->setYawPitchRoll(y, p, r);
		}
	}

	//Factor a baseAcceleration seg�n el �ngulo entre baseDirection y desiredDirection
	float CalculateAccelerationAccordingToDirection(VEC3 baseDirection, VEC3 desiredDirection, float baseAcceleration,
		float backwardsMaxDotProduct, float sidewaysMaxDotProduct, float backwardsAirDriftFactor, float sidewaysAirDriftFactor
	) {
		float resultingAcceleration = baseAcceleration;
		if (baseDirection.Dot(desiredDirection) <= backwardsMaxDotProduct) {
			resultingAcceleration *= backwardsAirDriftFactor;
		}
		else if (baseDirection.Dot(desiredDirection) <= sidewaysMaxDotProduct) {
			resultingAcceleration *= sidewaysAirDriftFactor;
		}
		return resultingAcceleration;
	}

	//Calcula el movimiento horizontal recorrido desde el �ltimo frame, teniendo en cuanta la posible nueva
	//direcci�n y lo clampea con la m�xima distancia recorrida posible en caso de ir a m�xima velocidad
	VEC3 CalculateHorizontalDeltaMovement(
		float delta, VEC3 lastFrameDirection, VEC3 newDirection,
		float acceleration, float maxSpeed
	) {
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

	//Se transfiere toda la velocidad que se llevaba a una nueva direcci�n (opcional) y se recalcula velocityVector para el siguiente frame
	void TransferVelocityToDirectionAndAccelerate(float delta, bool wantToTransfer, VEC3 directionOfAcceleration, float acceleration) {
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

	//Clampear velocidad horizontal, usando un VEC2, para no tocar la velocidad vertical
	void ClampHorizontalVelocity(float maxHorizontalSpeed) {
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

	//Calcula el movimiento vertical desde el �ltimo frame y lo clampea con la m�xima distancia 
	//recorrida posible en caso de ir a m�xima velocidad
	float CalculateVerticalDeltaMovement(float delta, float acceleration, float maxVelocityVertical) {
		float resultingDeltaMovement;
		resultingDeltaMovement = velocityVector->y * delta + 0.5f * acceleration * delta * delta;
		//clampear distancia vertical
		resultingDeltaMovement = resultingDeltaMovement > maxVelocityVertical * delta ? maxVelocityVertical * delta : resultingDeltaMovement;
		return resultingDeltaMovement;
	}

public:
	IActionState(CHandle playerHandle) {
		this->playerHandle = playerHandle;
		CEntity* playerEntity = playerHandle.getOwner();
		this->colliderHandle = playerEntity->get<TCompCollider>();
		this->playerTransformHandle = playerEntity->get<TCompTransform>();
	}
	virtual void update(float delta) = 0;
	virtual void OnStateEnter(IActionState* lastState) { this->lastState = lastState; }
	virtual void OnStateExit(IActionState* nextState) {
		this->nextState = nextState;
		deltaMovement = VEC3::Zero;
		movementInput = VEC2::Zero;
	}
	virtual void SetMovementInput(VEC2 movementInput) {}
	virtual VEC3 GetDeltaMovement() { return deltaMovement; }
	virtual VEC2 GetMovementInput() { return movementInput; }
	virtual void OnJumpHighButton() {}
	virtual void OnJumpHighButtonReleased() {}
	virtual void OnJumpLongButton() {}
	virtual void OnStrongAttackButton() {}
	virtual void OnStrongAttackButtonReleased() {}
	virtual void OnFastAttackButton() {}
	virtual void OnFastAttackButtonReleased() {}

	virtual void OnHitboxEnter(CHandle entity) {}
};
