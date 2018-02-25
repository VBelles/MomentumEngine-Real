#include "mcv_platform.h"
#include "GroundedActionState.h"

GroundedActionState::GroundedActionState(TCompPlayerModel * player)
	: IActionState::IActionState(player) {
	accelerationVector = player->GetAccelerationVector();
	velocityVector = player->GetVelocityVector();
	playerTransform = player->GetTransform();
}

void GroundedActionState::update (float delta) {
	bool hasInput = movementInput != VEC2::Zero;
	currentCamera = player->GetCamera();
	collider = player->GetCollider();
	PowerStats* currentPowerStats = player->GetPowerStats();

	VEC3 desiredDirection = player->GetCamera()->TransformToWorld(movementInput);
	VEC3 targetPos = playerTransform->getPosition() + desiredDirection * currentPowerStats->maxHorizontalSpeed * delta;

	if (hasInput && abs(playerTransform->getDeltaYawToAimTo(targetPos)) > 0.01f) {
		float y, p, r;
		playerTransform->getYawPitchRoll(&y, &p, &r);
		float yMult = playerTransform->isInLeft(targetPos) ? 1.f : -1.f;
		y += currentPowerStats->rotationSpeed * delta * yMult;
		playerTransform->setYawPitchRoll(y, p, r);
	}

	deltaMovement.x = 0;
	deltaMovement.z = 0;
	VEC2 horizontalVelocity = { velocityVector->x , velocityVector->z };
	float currentSpeed = horizontalVelocity.Length();
	//dbg("current speed: %f\n", currentSpeed);
	if (hasInput) {
		deltaMovement = playerTransform->getFront() * currentSpeed * delta + 0.5f * playerTransform->getFront() * currentPowerStats->acceleration * delta * delta;

		horizontalVelocity.x = playerTransform->getFront().x * currentSpeed;
		horizontalVelocity.y = playerTransform->getFront().z * currentSpeed;

		velocityVector->x = horizontalVelocity.x + playerTransform->getFront().x * currentPowerStats->acceleration * delta;
		velocityVector->z = horizontalVelocity.y + playerTransform->getFront().z * currentPowerStats->acceleration * delta;

		//clampear velocidad horizontal
		horizontalVelocity.x = velocityVector->x;
		horizontalVelocity.y = velocityVector->z;

		if (horizontalVelocity.Length() > currentPowerStats->maxHorizontalSpeed) {
			horizontalVelocity.Normalize();
			horizontalVelocity *= currentPowerStats->maxHorizontalSpeed;
			velocityVector->x = horizontalVelocity.x;
			velocityVector->z = horizontalVelocity.y;
		}
	}
	else {
		velocityVector->x = 0.f;
		velocityVector->z = 0.f;
	}

	currentPowerStats->currentGravityMultiplier = velocityVector->y < 0 ? currentPowerStats->fallingMultiplier : currentPowerStats->normalGravityMultiplier;
	float verticalVelocityIncrement = accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta;
	deltaMovement.y = velocityVector->y * delta + 0.5f * verticalVelocityIncrement * delta;

	velocityVector->y += verticalVelocityIncrement;
	velocityVector->y = clamp(velocityVector->y, -currentPowerStats->maxVelocityVertical, currentPowerStats->maxVelocityVertical);
}

void GroundedActionState::OnStateEnter(IActionState * lastState) {
	IActionState::OnStateEnter(lastState);
	//dbg("Entrando en grounded\n");
}

void GroundedActionState::OnStateExit(IActionState * nextState) {
	IActionState::OnStateExit(nextState);
	//dbg("Saliendo de grounded\n");
}

void GroundedActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

void GroundedActionState::OnJumpHighButton() {
	player->SetMovementState(TCompPlayerModel::ActionStates::JumpSquat);
}

void GroundedActionState::OnJumpLongButton() {
	player->SetMovementState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void GroundedActionState::OnStrongAttackButton() {
	if(player->IsAttackFree()){
		player->SetAttackState(TCompPlayerModel::ActionStates::StrongAttack);
	}
}

void GroundedActionState::OnFastAttackButton() {
}

void GroundedActionState::OnLeavingGround() {
	//Set state a alguno por defecto, luego las clases derivadas de esta ya sabrán qué hacer
	player->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}
