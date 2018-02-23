#include "mcv_platform.h"
#include "JumpSquatLongActionState.h"

JumpSquatLongActionState::JumpSquatLongActionState(TCompPlayerModel * player)
	: GroundedActionState::GroundedActionState(player) {
}


void JumpSquatLongActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	//dbg("Entrando en JumpSquatLong\n");
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
}

void JumpSquatLongActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	//dbg("Saliendo de JumpSquatLong\n");
}

void JumpSquatLongActionState::update (float delta) {
	PowerStats* currentPowerStats = player->GetPowerStats();
	velocityVector = player->GetVelocityVector();
	playerTransform = player->GetTransform();
	accelerationVector = player->GetAccelerationVector();
	collider = player->GetCollider();

	if (timer.elapsed() >= squatTime) {
		//saltar
		*velocityVector = playerTransform->getFront() * currentPowerStats->longJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
		//Dejamos que el cambio de estado se haga cuando lo detecte ground sensor
		deltaMovement = *velocityVector * delta;
	}
	else {
		deltaMovement = VEC3::Zero;
		currentPowerStats->currentGravityMultiplier = currentPowerStats->normalGravityMultiplier;
		deltaMovement.y = velocityVector->y * delta + 0.5f * accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta * delta;
	}

	physx::PxControllerCollisionFlags myFlags = collider->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	velocityVector->y += clamp(accelerationVector->y * delta, -currentPowerStats->maxVelocityVertical, currentPowerStats->maxVelocityVertical);

	player->isGrounded = myFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_DOWN);
	if (player->isGrounded) {
		velocityVector->y = 0;
	}
	else {
		OnLeavingGround();
	}
}

void JumpSquatLongActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

//ni caso a este input
void JumpSquatLongActionState::OnJumpHighButton() {}

void JumpSquatLongActionState::OnJumpLongButton() {}

void JumpSquatLongActionState::OnLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		player->SetActionState(TCompPlayerModel::ActionStates::AirborneLong);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		player->SetActionState(TCompPlayerModel::ActionStates::GhostJumpSquatLong);
	}
}
