#include "mcv_platform.h"
#include "JumpSquatLongActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/StateManager.h"


JumpSquatLongActionState::JumpSquatLongActionState(StateManager* stateManager) :
	GroundedActionState(stateManager, JumpSquatLong) {
}

void JumpSquatLongActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();

	if (timer.elapsed() >= squatTime) {
		//saltar
		//getPlayerModel()->isAttachedToPlatform = false;
		*velocityVector = getPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;
		getPlayerModel()->wannaJump = true;
	}
}

void JumpSquatLongActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	//dbg("Entrando en JumpSquatLong\n");
	timer.reset();
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	EngineSound.emitEvent(SOUND_JUMP_LONG);

}

void JumpSquatLongActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	//dbg("Saliendo de JumpSquatLong\n");
}

void JumpSquatLongActionState::onLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		stateManager->changeState(AirborneLong);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		stateManager->changeState(GhostJumpSquatLong);
	}
}

