#pragma once

#include "components/player/states/EStates.h"
#include "components/player/attack_info.h"

const float FPS = 1.f / 60.f;
#define frames2sec(frames) frames * FPS

class StateManager;
class TCompPlayerModel;
class TCompCamera;
class CCamera;
class TCompCollider;
class TCompRender;
class TCompTransform;
class TCompHitboxes;
class TCompRenderBlurRadial;
struct HitState;
struct MoveState;
struct TMsgAttackHit;

using namespace physx;

class IActionState {

protected:
	VEC3 deltaMovement;
	VEC2 movementInput;
	VEC3* accelerationVector;
	VEC3* velocityVector;

	TCompPlayerModel* getPlayerModel();
	TCompTransform* getPlayerTransform();
	TCompCollider* getCollider();
	TCompRender* getRender();
	TCompHitboxes* getHitboxes();
	TCompCamera* getCamera();
	TCompRenderBlurRadial* getBlurRadial();

	//Rota hacia targetPos a velocidad rotationSpeed durante el tiempo delta
	void rotatePlayerTowards(float delta, VEC3 targetPos, float rotationSpeed);

	//Factor a baseAcceleration según el ángulo entre baseDirection y desiredDirection
	float calculateAccelerationAccordingToDirection(VEC3 baseDirection, VEC3 desiredDirection, float baseAcceleration,
		float backwardsMaxDotProduct, float sidewaysMaxDotProduct, float backwardsAirDriftFactor, float sidewaysAirDriftFactor);

	//Calcula el movimiento horizontal recorrido desde el último frame, teniendo en cuanta la posible nueva
	//dirección y lo clampea con la máxima distancia recorrida posible en caso de ir a máxima velocidad
	VEC3 calculateHorizontalDeltaMovement(float delta, VEC3 lastFrameDirection, VEC3 newDirection,
		float acceleration, float maxSpeed);

	//Se transfiere toda la velocidad que se llevaba a una nueva direcci�n (opcional) y se recalcula velocityVector para el siguiente frame
	void transferVelocityToDirectionAndAccelerate(float delta, bool wantToTransfer, VEC3 directionOfAcceleration, float acceleration);

	//Clampear velocidad horizontal, usando un VEC2, para no tocar la velocidad vertical
	void clampHorizontalVelocity(float maxHorizontalSpeed);

	bool isWalkable(MoveState& moveState);

public:

	StateManager* stateManager;

	State state;
	ConcurrentState concurrentState;

	IActionState* lastState;
	IActionState* nextState;

	IActionState(StateManager * stateManager, State state, ConcurrentState concurrentState);
	IActionState(StateManager* stateManager, State state);
	IActionState(StateManager* stateManager, ConcurrentState concurrentState);

	virtual void update(float delta) = 0;
	virtual void onStateEnter(IActionState* lastState);
	virtual void onStateExit(IActionState* nextState);

	virtual void setMovementInput(VEC2 input);

	virtual void onJumpHighButton() {}
	virtual void onJumpHighButtonReleased() {}
	virtual void onJumpLongButton() {}
	virtual void onStrongAttackButton() {}
	virtual void onStrongAttackButtonReleased() {}
	virtual void onFastAttackButton() {}
	virtual void onFastAttackButtonReleased() {}
	virtual void onReleasePowerButton() {}
	virtual void onHitboxEnter(std::string hitbox, CHandle entity) {}
	virtual void onDead();
	virtual void onDamage(const TMsgAttackHit& msg);
	virtual void onMove(MoveState& moveState) {}

	VEC3 getDeltaMovement() { return deltaMovement; }
	VEC2 getMovementInput() { return movementInput; }
};
