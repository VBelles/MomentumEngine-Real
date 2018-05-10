#include "mcv_platform.h"
#include "GroundedActionState.h"
#include "components/player/comp_player_model.h"


void GroundedActionState::update(float delta) {

}

void GroundedActionState::onStateEnter(IActionState * lastState) {
	IActionState::onStateEnter(lastState);
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	if (currentPowerStats) getPlayerModel()->maxVerticalSpeed = currentPowerStats->maxVelocityVertical;
	getPlayerModel()->resetGravity();
	backwardsMaxDotProduct = cos(backwardsdMinAngle);
	getPlayerModel()->lastWallEntered = nullptr;//En realidad al tocar el suelo ya se sobreescribe la variable
	getPlayerModel()->tryingToSlide = false;
}

void GroundedActionState::onStateExit(IActionState * nextState) {
	IActionState::onStateExit(nextState);
	getPlayerModel()->lastWallEntered = nullptr;
	getPlayerModel()->lastWallNormal = PxVec3(0, 0, 0);
	getPlayerModel()->tryingToSlide = false;
}

void GroundedActionState::onJumpHighButton() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquat);
}

void GroundedActionState::onJumpLongButton() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void GroundedActionState::onStrongAttackButton() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::StrongAttack);
}

void GroundedActionState::onFastAttackButton() {
	if (getPlayerModel()->isConcurrentActionFree()) {
		getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::FastAttack);
	}
}

void GroundedActionState::onReleasePowerButton() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::ReleasePowerGround);
}

void GroundedActionState::onMove(MoveState& moveState) {
	if (!moveState.isTouchingBot) {
		onLeavingGround();
		return;
	}

	bool grounded = false;
	for (HitState& hit : moveState.hits) {
		if (hit.dotUp >= getPlayerModel()->getController()->getSlopeLimit()) {
			grounded = true;
			break;
		}
	}

	if (!grounded) { //Slide
		if (!getPlayerModel()->tryingToSlide) {
			getPlayerModel()->tryingToSlide = true;
			slideTimer.reset();
		}
		else if (slideTimer.elapsed() >= slideWindowTime) {
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Slide);
		}
	}
	else {
		velocityVector->y = 0.f;
		getPlayerModel()->tryingToSlide = false;
	}

}

void GroundedActionState::onLeavingGround() {
	//Set state a alguno por defecto, luego las clases derivadas de esta ya sabr�n qu� hacer
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}
