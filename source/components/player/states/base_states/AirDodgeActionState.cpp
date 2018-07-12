#include "mcv_platform.h"
#include "components/player/comp_player_model.h"
#include "AirDodgeActionState.h"
#include "components/comp_camera.h"
#include "components/controllers/comp_camera_player.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


AirDodgeActionState::AirDodgeActionState(StateManager* stateManager)
	: AirborneActionState::AirborneActionState(stateManager, AirDodge) {
}

void AirDodgeActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (!isDodging && timer.elapsed() >= recoveryTime) {
		stateManager->changeState(AirborneNormal);
	}
	else if (isDodging && timer.elapsed() >= dodgingTime) {
		getSkeleton()->blendCycle(airborneAnimation, 0.2f, 0.2f);
		isDodging = false;
		timer.reset();
	}

	if (timer.elapsed() >= dodgingTime) {
		VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
		if (deceleration * delta < horizontalVelocity.Length()) {
			deltaMovement += calculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
				-VEC3(velocityVector->x, 0, velocityVector->z), deceleration, dodgeSpeed);

			transferVelocityToDirectionAndAccelerate(delta, false, -VEC3(velocityVector->x, 0, velocityVector->z), deceleration);
		}
		else {
			velocityVector->x = 0.f;
			velocityVector->z = 0.f;
		}
	}
	deltaMovement.x = velocityVector->x * delta;
	deltaMovement.z = velocityVector->z * delta;

}

void AirDodgeActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	timer.reset();
	invencibilityTimer.reset();
	stateManager->changeConcurrentState(Free);
	isDodging = true;
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);

	//set velocity vector
	*velocityVector = -getPlayerTransform()->getFront() * dodgeSpeed;
}

void AirDodgeActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	getSkeleton()->removeAction(animation, 0.2f);
	//getSkeleton()->removeAction(airborneAnimation, 0.2f);
}

void AirDodgeActionState::onJumpHighButton() {
	if (invencibilityTimer.elapsed() >= takeActionTime) {
		AirborneActionState::onJumpHighButton();
	}
}

void AirDodgeActionState::onJumpLongButton() {
	if (invencibilityTimer.elapsed() >= takeActionTime) {
		AirborneActionState::onJumpLongButton();
	}
}

void AirDodgeActionState::onStrongAttackButton() {
	if (invencibilityTimer.elapsed() >= takeActionTime) {
		AirborneActionState::onStrongAttackButton();
	}
}

void AirDodgeActionState::onFastAttackButton() {
	if (invencibilityTimer.elapsed() >= takeActionTime) {
		AirborneActionState::onFastAttackButton();
	}
}

void AirDodgeActionState::onReleasePowerButton() {
	if (invencibilityTimer.elapsed() >= takeActionTime) {
		AirborneActionState::onReleasePowerButton();
	}
}

void AirDodgeActionState::onDamage(const TMsgAttackHit & msg) {
	if (invencibilityTimer.elapsed() >= invencibilityTime) {
		AirborneActionState::onDamage(msg);
	}
	else {
		//enviar mensaje a enemigo conforme ha sido esquivado finamente
		dbg("Can't touch this! Hammer Time!\n");
		CEntity* attacker = msg.attacker;
		attacker->sendMsg(TMsgPerfectDodged{});
	}
}

void AirDodgeActionState::onMove(MoveState& moveState) {
	//Don't try to hug wall

	//Touching top
	if (moveState.isTouchingTop && velocityVector->y > 0.f) {
		velocityVector->y = 0.f;
	}
	//Land or slide
	if (moveState.isTouchingBot) {
		if (!isWalkable(moveState)) {
			if (slideWindowTimer.elapsed() >= slideWindowTime) {
				stateManager->changeState(Slide);
				getPlayerModel()->lockFallingAttack = false;
				getPlayerModel()->lockAirDodge = false;
			}
		}
		else {
			onLanding();
		}
	}
	else {
		slideWindowTimer.reset();
	}
}