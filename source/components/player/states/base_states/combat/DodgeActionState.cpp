#include "mcv_platform.h"
#include "components/player/comp_player_model.h"
#include "DodgeActionState.h"
#include "components/controllers/comp_camera_player.h"
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
		timer.reset();
	}
	else if(isMoving){
		//move
		deltaMovement.x = velocityVector->x * delta;
		deltaMovement.z = velocityVector->z * delta;
	}

}

void DodgeActionState::onStateEnter(IActionState * lastState) {
	VEC2 leavingInput = lastState == this ? movementInput : lastState->getMovementInput();
	GroundedActionState::onStateEnter(lastState);
	timer.reset();
	invencibilityTimer.reset();
	stateManager->changeConcurrentState(Free);
	isMoving = true;
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);

	//posibilidad: coger el último input y encarar esa dirección antes de decidir la velocidad
	if (leavingInput.Length() > PAD_DEAD_ZONE) {
		leavingInput.Normalize();
		VEC3 movementInputWorldSpace = getCamera()->getCamera()->TransformToWorld(leavingInput);
		float exitYaw = atan2(movementInputWorldSpace.x, movementInputWorldSpace.z);
		float y, p;
		getPlayerTransform()->getYawPitchRoll(&y, &p);
		getPlayerTransform()->setYawPitchRoll(exitYaw, p);
	}

	//set velocity vector
	*velocityVector = -getPlayerTransform()->getFront() * dodgeSpeed;

	//staling
	currentNumberOfCloseDodges++;
	if (currentNumberOfCloseDodges >= numberOfDodgesToStale) {
		bool isStaling = true;
		for (int i = 0; i < numberOfDodgesToStale - 1; i++) {
			if (staleTimers[i].elapsed() >= staleTime) {
				isStaling = false;
				currentNumberOfCloseDodges--;
			}
		}
		if (isStaling) {
			currentNumberOfCloseDodges = 1;
			isInterruptible = false;
			recoveryTime = staleRecoveryTime;
			dbg("No abuses, cabrón\n");
		}
	}
	//resetear el timer que me toca
	staleTimers[staleTimerIndex].reset();
	staleTimerIndex = (staleTimerIndex + 1) % (numberOfDodgesToStale - 1);

	getCameraPlayer()->moveCameraCloser(false);

	EngineSound.emitFollowingEvent(SOUND_DODGE, getPlayerTransform());
}

void DodgeActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState); 
	getSkeleton()->removeAction(animation, 0.2f);
	getSkeleton()->removeAction(recoveryAnimation, 0.2f);
	if (!isInterruptible) {
		isInterruptible = true;
		recoveryTime = regularRecoveryTime;
	}
}

void DodgeActionState::onJumpHighButton() {
	if (isInterruptible && invencibilityTimer.elapsed() >= takeActionTime) {
		multiplyHorizontalSpeed(leavingGroundSpeedMultiplier);
		GroundedActionState::onJumpHighButton();
	}
}

void DodgeActionState::onJumpLongButton() {
	if (isInterruptible && invencibilityTimer.elapsed() >= takeActionTime) {
		multiplyHorizontalSpeed(leavingGroundSpeedMultiplier);
		GroundedActionState::onJumpLongButton();
	}
}

void DodgeActionState::onStrongAttackButton() {
	if (isInterruptible && invencibilityTimer.elapsed() >= takeActionTime) {
		GroundedActionState::onStrongAttackButton();
	}
}

void DodgeActionState::onFastAttackButton() {
	if (isInterruptible && invencibilityTimer.elapsed() >= takeActionTime) {
		stateManager->changeState(Idle);
		multiplyHorizontalSpeed(0.f);
		GroundedActionState::onFastAttackButton();
	}
}

void DodgeActionState::onDodgeButton() {
	if (isInterruptible && invencibilityTimer.elapsed() >= takeActionTime) {
		onStateExit(this);
		onStateEnter(this);
	}
}

void DodgeActionState::onReleasePowerButton() {
	if (isInterruptible && invencibilityTimer.elapsed() >= takeActionTime) {
		GroundedActionState::onReleasePowerButton();
	}
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
	if (invencibilityTimer.elapsed() >= invencibilityTime) {
		GroundedActionState::onDamage(msg);
	}
	else {
		//enviar mensaje a enemigo conforme ha sido esquivado finamente
		dbg("Can't touch this! Hammer Time!\n");
		CEntity* attacker = msg.attacker;
		attacker->sendMsg(TMsgPerfectDodged{});
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
