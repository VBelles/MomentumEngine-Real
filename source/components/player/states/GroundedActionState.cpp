#include "mcv_platform.h"
#include "GroundedActionState.h"


GroundedActionState::GroundedActionState(StateManager* stateManager, State state)
	: IActionState(stateManager, state) {
};

GroundedActionState::GroundedActionState(StateManager* stateManager, ConcurrentState state)
	: IActionState(stateManager, state) {
};


void GroundedActionState::update(float delta) {

}

void GroundedActionState::onStateEnter(IActionState * lastState) {
	IActionState::onStateEnter(lastState);
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	if (currentPowerStats) getPlayerModel()->maxVerticalSpeed = currentPowerStats->maxVelocityVertical;
	getPlayerModel()->resetGravity();
	backwardsMaxDotProduct = cos(backwardsdMinAngle);
	slideWindowTimer.reset();
}

void GroundedActionState::onStateExit(IActionState * nextState) {
	IActionState::onStateExit(nextState);
	getPlayerModel()->lastWallNormal = PxVec3(0, 0, 0);
}

void GroundedActionState::onJumpHighButton() {
	stateManager->changeConcurrentState(Free);
	stateManager->changeState(JumpSquat);
}

void GroundedActionState::onJumpLongButton() {
	stateManager->changeConcurrentState(Free);
	stateManager->changeState(JumpSquatLong);
}

void GroundedActionState::onStrongAttackButton() {
	stateManager->changeState(StrongAttack);
}

void GroundedActionState::onFastAttackButton() {
	if (stateManager->isConcurrentActionFree()) {
		stateManager->changeConcurrentState(FastAttack);
	}
}

void GroundedActionState::onReleasePowerButton() {
	stateManager->changeState(ReleasePowerGround);
}

void GroundedActionState::onDodgeButton() {
	stateManager->changeState(Dodge);
}

void GroundedActionState::onMove(MoveState& moveState) {
	if (!moveState.isTouchingBot) {
		onLeavingGround();
	}
	else {
		if (!isWalkable(moveState)) { //Slide
			if (slideWindowTimer.elapsed() >= slideWindowTime) {
				stateManager->changeState(Slide);
			}
		}
		else {
			slideWindowTimer.reset();
		}
		velocityVector->y = 0.f;
	}

}

void GroundedActionState::onLeavingGround() {
	//Set state a alguno por defecto, luego las clases derivadas de esta ya sabr�n qu� hacer
	stateManager->changeConcurrentState(Free);
	stateManager->changeState(GhostJumpWindow);
}


void GroundedActionState::onDamage(const TMsgAttackHit& msg) {
	bool hasSuperArmor = false;
	bool hasInvulnerability = false;
	if (AttackState* attackState = dynamic_cast<AttackState*>(this)) {
		hasSuperArmor = attackState->hasSuperArmor();
		hasInvulnerability = attackState->hasInvulnerability();
	}
	if (!hasSuperArmor && !hasInvulnerability) {
		if (msg.info.stun) {
			stateManager->changeState(HardKnockbackGround);
		}
		else {
			stateManager->changeConcurrentState(SoftKnockbackGround);
		}
	}
	else if (!hasInvulnerability) {
		//super armor sound
	}

	if (!hasInvulnerability) {
		IActionState::onDamage(msg);//Hacemos esto al final para sobreescribir el estado de muerte
	}
	else {
		//invulnerability sound
	}
}
