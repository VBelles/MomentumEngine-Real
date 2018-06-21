#include "mcv_platform.h"
#include "JumpSquatActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


JumpSquatActionState::JumpSquatActionState(StateManager* stateManager) :
	GroundedActionState(stateManager, JumpSquat) {
}

void JumpSquatActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	if (timer.elapsed() >= squatTime) {
		//saltar
		velocityVector->y = isShortHop ? currentPowerStats->shortHopVelocity : currentPowerStats->jumpVelocityVector.y;
		//Dejamos que el cambio de estado se haga cuando lo detecte ground sensor
		deltaMovement = *velocityVector * delta;
		getPlayerModel()->wannaJump = true;
	}
	else {
		bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;
		if (hasInput) {
			deltaMovement += getPlayerTransform()->getFront() * enteringVelocity * delta;
		}
	}
}

void JumpSquatActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	//dbg("Entrando en JumpSquat\n");
	isShortHop = false;
	timer.reset();
	enteringVelocity = getPlayerModel()->getVelocityVector()->Length();
	getSkeleton()->executeAction(animationSquat, 0.1f, 0.03f);
}

void JumpSquatActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	//dbg("Saliendo de JumpSquat\n");
}

void JumpSquatActionState::onJumpHighButtonReleased() {
	isShortHop = true;
}


void JumpSquatActionState::onLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		getSkeleton()->executeAction(animationJump, 0.03f, 0.3f);
		stateManager->changeState(AirborneNormal);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		stateManager->changeState(GhostJumpSquat);
	}
}
