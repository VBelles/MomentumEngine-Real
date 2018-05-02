#include "mcv_platform.h"
#include "JumpSquatActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"

JumpSquatActionState::JumpSquatActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	animation = "jump_inicio";
}

void JumpSquatActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	if (timer.elapsed() >= squatTime) {
		//saltar
		getPlayerModel()->isAttachedToPlatform = false;
		velocityVector->y = isShortHop ? currentPowerStats->shortHopVelocity : currentPowerStats->jumpVelocityVector.y;
		//Dejamos que el cambio de estado se haga cuando lo detecte ground sensor
		deltaMovement = *velocityVector * delta;
		getPlayerModel()->wannaJump = true;
	}
	else {
		bool hasInput = movementInput != VEC2::Zero;
		if (hasInput) {
			deltaMovement += getPlayerTransform()->getFront() * enteringVelocity * delta;
		}
	}
}

void JumpSquatActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	//dbg("Entrando en JumpSquat\n");
	squatTime = squatFrames * (1.f / 60);
	isShortHop = false;
	timer.reset();
	enteringVelocity = getPlayerModel()->getVelocityVector()->Length();
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void JumpSquatActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	//dbg("Saliendo de JumpSquat\n");
}

void JumpSquatActionState::onJumpHighButtonReleased() {
	isShortHop = true;
}

void JumpSquatActionState::setPose() {
	getRender()->setMesh("data/meshes/pose_jump_squat.mesh");
}

void JumpSquatActionState::onLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);

	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpSquat);
	}
}
