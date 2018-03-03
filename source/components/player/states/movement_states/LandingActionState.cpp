#include "mcv_platform.h"
#include "LandingActionState.h"

LandingActionState::LandingActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void LandingActionState::update (float delta) {
	bool hasInput = movementInput != VEC2::Zero;
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	VEC3 desiredDirection = GetPlayerModel()->GetCamera()->TransformToWorld(movementInput);
	if (hasInput) {
		VEC3 targetPos = GetPlayerTransform()->getPosition() + desiredDirection;
		RotatePlayerTowards(delta, targetPos, currentPowerStats->rotationSpeed);
	}

	if (timer.elapsed() >= landingLagTime) {
		if (!hasInput || enteringSpeed == 0.f) {
			GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Idle);
		}
		else {
			velocityVector->x = GetPlayerTransform()->getFront().x * enteringSpeed;
			velocityVector->z = GetPlayerTransform()->getFront().z * enteringSpeed;
			GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Run);
		}
	}

	currentPowerStats->currentGravityMultiplier = velocityVector->y < 0 ? currentPowerStats->fallingMultiplier : currentPowerStats->normalGravityMultiplier;
	deltaMovement.y = CalculateVerticalDeltaMovement(delta, accelerationVector->y * currentPowerStats->currentGravityMultiplier, currentPowerStats->maxVelocityVertical);

	velocityVector->y += accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta;
	velocityVector->y = clamp(velocityVector->y, -currentPowerStats->maxVelocityVertical, currentPowerStats->maxVelocityVertical);
}

void LandingActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	GetRender()->setMesh("data/meshes/pose_punch.mesh");

	VEC3 enteringVelocityVector = { velocityVector->x, 0.f, velocityVector->z };
	enteringSpeed = enteringVelocityVector.Length();
	velocityVector->x = 0.f;
	velocityVector->z = 0.f;
	deltaMovement = lastState->GetDeltaMovement();
	deltaMovement.x = 0.f;
	deltaMovement.z = 0.f;
	landingLagTime = landingLagFrames * (1.f / 60);
	timer.reset();
	dbg("Entrando en landing\n");
}

void LandingActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	dbg("Saliendo de landing\n");
}

void LandingActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
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
		VEC3 movementInputWorldSpace = GetPlayerModel()->GetCamera()->TransformToWorld(movementInput);
		float exitYaw = atan2(movementInputWorldSpace.x, movementInputWorldSpace.z);
		float y, p, r;
		GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
		GetPlayerTransform()->setYawPitchRoll(exitYaw, p, r);
		velocityVector->x = GetPlayerTransform()->getFront().x * enteringSpeed;
		velocityVector->z = GetPlayerTransform()->getFront().z * enteringSpeed;
	}
}
