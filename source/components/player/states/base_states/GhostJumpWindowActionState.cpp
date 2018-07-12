#include "mcv_platform.h"
#include "GhostJumpWindowActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/AttackState.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


GhostJumpWindowActionState::GhostJumpWindowActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, GhostJumpWindow) {
}

void GhostJumpWindowActionState::update(float delta) {
	if (timer.elapsed() >= jumpTimeWindow) {
		//Como estamos ya en el aire, hacemos el cambio nosotros mismos
		if (!stateManager->isChangingBaseState) {
			stateManager->changeState(AirborneNormal);
		}
	}
	AirborneActionState::update(delta);
}

void GhostJumpWindowActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	timer.reset();
	enteringVelocity = getPlayerModel()->getVelocityVector()->Length();
	getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
	if (lastState->state == Dodge || dynamic_cast<AttackState*>(lastState)) {
		jumpTimeWindow = specialWindow;
	}
	else {
		jumpTimeWindow = regularWindow;
	}
}

void GhostJumpWindowActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}

void GhostJumpWindowActionState::onJumpHighButton() {
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	velocityVector->y = 0.f;
	*velocityVector += currentPowerStats->jumpVelocityVector;
	//Como estamos ya en el aire, hacemos el cambio nosotros mismos
	stateManager->changeState(AirborneNormal);
}

void GhostJumpWindowActionState::onJumpLongButton() {
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	*velocityVector = getPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
	velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
	stateManager->changeState(AirborneLong);
}
