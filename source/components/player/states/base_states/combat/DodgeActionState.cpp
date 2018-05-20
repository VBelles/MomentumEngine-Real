#include "mcv_platform.h"
#include "components/player/comp_player_model.h"
#include "DodgeActionState.h"
#include "components/comp_camera.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


DodgeActionState::DodgeActionState(StateManager* stateManager)
	: GroundedActionState::GroundedActionState(stateManager, Dodge) {
}

void DodgeActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (!isMoving && timer.elapsed() >= recoveryTime) {
		stateManager->changeState(Idle);
	}
	else if (isMoving && timer.elapsed() >= movingTime) {
		getSkeleton()->executeAction(recoveryAnimation, 0.2f, 0.2f);
		*velocityVector = VEC3::Zero;
		deltaMovement.x = 0.f;
		deltaMovement.z = 0.f;
		isMoving = false;
	}
	else if(isMoving){
		//move
		deltaMovement.x = velocityVector->x * delta;
		deltaMovement.z = velocityVector->z * delta;
	}

}

void DodgeActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	timer.reset();
	stateManager->changeConcurrentState(Free);
	isMoving = true;
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	//posibilidad: coger el último input y encarar esa dirección antes de decidir la velocidad
	//set velocity vector
	*velocityVector = -getPlayerTransform()->getFront() * dodgeSpeed;
}

void DodgeActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState); 
	getSkeleton()->removeAction(animation, 0.2f);
	getSkeleton()->removeAction(recoveryAnimation, 0.2f);
}

void DodgeActionState::onJumpHighButton() {
	multiplyHorizontalSpeed(leavingGroundSpeedMultiplier);
	stateManager->changeState(JumpSquat);
}

void DodgeActionState::onJumpLongButton() {
	multiplyHorizontalSpeed(leavingGroundSpeedMultiplier);
	stateManager->changeState(JumpSquatLong);
}

void DodgeActionState::onMove(MoveState & moveState) {
	if (!moveState.isTouchingBot) {
		onLeavingGround();
	}
	else {
		if (!isWalkable(moveState)) { //Slide
			stateManager->changeState(Slide);
			multiplyHorizontalSpeed(leavingGroundSpeedMultiplier);
		}
		velocityVector->y = 0.f;
	}
}

void DodgeActionState::onDamage(const TMsgAttackHit & msg) {
	if (timer.elapsed() >= invencibilityTime) {
		GroundedActionState::onDamage(msg);
	}
}

void DodgeActionState::onLeavingGround() {
	multiplyHorizontalSpeed(leavingGroundSpeedMultiplier);
	stateManager->changeState(GhostJumpWindow);
}

void DodgeActionState::multiplyHorizontalSpeed(float multiplier) {
	velocityVector->x *= multiplier;
	velocityVector->z *= multiplier;
}
