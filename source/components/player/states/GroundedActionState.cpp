#include "mcv_platform.h"
#include "GroundedActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/StateManager.h"
#include "entity/common_msgs.h"

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
}

void GroundedActionState::onStateExit(IActionState * nextState) {
	IActionState::onStateExit(nextState);
	getPlayerModel()->lastWallNormal = PxVec3(0, 0, 0);
}

void GroundedActionState::onJumpHighButton() {
	stateManager->changeState(JumpSquat);
}

void GroundedActionState::onJumpLongButton() {
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

void GroundedActionState::onMove(MoveState& moveState) {
	if (!moveState.isTouchingBot) {
		onLeavingGround();
	}
	else {
		if (!isWalkable(moveState)) { //Slide
			stateManager->changeState(Slide);
		}
		velocityVector->y = 0.f;
	}
}

void GroundedActionState::onLeavingGround() {
	//Set state a alguno por defecto, luego las clases derivadas de esta ya sabr�n qu� hacer
	stateManager->changeState(GhostJumpWindow);
}


void GroundedActionState::onDamage(const TMsgAttackHit& msg) {
	if (msg.info.stun) {
		stateManager->changeState(HardKnockbackGround);
	}
	else {
		stateManager->changeConcurrentState(SoftKnockbackGround);
	}
	IActionState::onDamage(msg);//Hacemos esto al final para sobreescribir el estado de muerte
}
