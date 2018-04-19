#include "mcv_platform.h"
#include "JumpSquatLongActionState.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/player/comp_player_model.h"
#include "skeleton/comp_skeleton.h"

JumpSquatLongActionState::JumpSquatLongActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	animation = "jump_inicio";
}

void JumpSquatLongActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	
	if (timer.elapsed() >= squatTime) {
		//saltar
		getPlayerModel()->isAttachedToPlatform = false;
		*velocityVector = getPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;
	}
}

void JumpSquatLongActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	//dbg("Entrando en JumpSquatLong\n");
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void JumpSquatLongActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	//dbg("Saliendo de JumpSquatLong\n");
}

void JumpSquatLongActionState::onLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneLong);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpSquatLong);
	}
}

void JumpSquatLongActionState::setPose() {
	getRender()->setMesh("data/meshes/pose_jump_squat.mesh");
}
