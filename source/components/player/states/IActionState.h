#pragma once

#include "mcv_platform.h"
#include "components/player/comp_player_model.h"

class TCompPlayerModel;

class IActionState {
protected:
	IActionState* lastState;
	IActionState* nextState;
	TCompPlayerModel* player;
	VEC3 deltaMovement;
	VEC2 movementInput;
	TCompTransform* playerTransform;
	TCompCamera* currentCamera;
	TCompCollider* collider;
	VEC3* accelerationVector;
	VEC3* velocityVector;

public:
	IActionState(TCompPlayerModel* player) {
		this->player = player;
	}
	virtual void update(float delta) = 0;
	virtual void OnStateEnter(IActionState* lastState) { this->lastState = lastState; }
	virtual void OnStateExit(IActionState* nextState) { this->nextState = nextState; }
	virtual void SetMovementInput(VEC2 movementInput) {}
	virtual VEC3 GetDeltaMovement() { return deltaMovement; }
	virtual void OnJumpHighButton() {}
	virtual void OnJumpLongButton() {}
	virtual void OnStrongAttack() {}
	virtual void OnFastAttack() {}

};
