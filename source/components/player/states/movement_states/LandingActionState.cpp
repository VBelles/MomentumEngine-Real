#include "mcv_platform.h"
#include "LandingActionState.h"

LandingActionState::LandingActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void LandingActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput != VEC2::Zero;
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	VEC3 desiredDirection = GetCamera()->TransformToWorld(movementInput);
	if (hasInput) {
		VEC3 targetPos = GetPlayerTransform()->getPosition() + desiredDirection;
		RotatePlayerTowards(delta, targetPos, currentPowerStats->rotationSpeed);
	}

	if (timer.elapsed() >= landingLagTime) {
		if (movementInput.Length() < 0.8f || enteringSpeed == 0.f) {
			GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Idle);
		}
		else {
			velocityVector->x = GetPlayerTransform()->getFront().x * enteringSpeed * 0.6f;
			velocityVector->z = GetPlayerTransform()->getFront().z * enteringSpeed * 0.6f;
			GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Walk);
		}
	}
}

void LandingActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	SetPose();
	VEC3 enteringVelocityVector = { velocityVector->x, 0.f, velocityVector->z };
	enteringSpeed = enteringVelocityVector.Length();
	velocityVector->x = 0.f;
	velocityVector->z = 0.f;
	landingLagFrames = GetPlayerModel()->GetPowerStats()->landingLag;
	landingLagTime = landingLagFrames * (1.f / 60);
	timer.reset();
	dbg("Entrando en landing\n");
}

void LandingActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	dbg("Saliendo de landing\n");
}

void LandingActionState::OnJumpHighButton() {
	SetFinalRotationAndVelocity();
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquat);
}

void LandingActionState::OnJumpLongButton() {
	SetFinalRotationAndVelocity();
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void LandingActionState::OnLeavingGround() {
	//Set state a alguno por defecto, luego las clases derivadas de esta ya sabrán qué hacer
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void LandingActionState::SetFinalRotationAndVelocity() {
	bool hasInput = movementInput != VEC2::Zero;
	if (hasInput) {
		movementInput.Normalize();
		VEC3 movementInputWorldSpace = GetCamera()->TransformToWorld(movementInput);
		float exitYaw = atan2(movementInputWorldSpace.x, movementInputWorldSpace.z);
		float y, p, r;
		GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
		GetPlayerTransform()->setYawPitchRoll(exitYaw, p, r);
		velocityVector->x = GetPlayerTransform()->getFront().x * enteringSpeed;
		velocityVector->z = GetPlayerTransform()->getFront().z * enteringSpeed;
	}
}
