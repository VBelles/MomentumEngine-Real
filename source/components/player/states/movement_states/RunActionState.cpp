#include "mcv_platform.h"
#include "RunActionState.h"

RunActionState::RunActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void RunActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput != VEC2::Zero;
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	bool wantToWalk = false;
	if (movementInput.Length() < 0.8f) {
		wantToWalk = true;
	}

	//Buscamos un punto en la dirección en la que el jugador querría ir y, según si queda a izquierda o derecha, rotamos
	VEC3 desiredDirection = GetPlayerModel()->GetCamera()->TransformToWorld(movementInput);
	bool isTurnAround = GetPlayerModel()->GetTransform()->getFront().Dot(desiredDirection) <= backwardsMaxDotProduct;
	if (hasInput && !isTurnAround) {
		VEC3 targetPos = GetPlayerTransform()->getPosition() + desiredDirection;
		RotatePlayerTowards(delta, targetPos, currentPowerStats->rotationSpeed);
	}

	//Si hay input se traslada toda la velocidad antigua a la nueva dirección de front y se le añade lo acelerado
	if (hasInput && !wantToWalk) {
		deltaMovement += CalculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			GetPlayerTransform()->getFront(), currentPowerStats->acceleration,
			currentPowerStats->maxHorizontalSpeed);

		TransferVelocityToDirectionAndAccelerate(delta, true, GetPlayerTransform()->getFront(), currentPowerStats->acceleration);
		ClampHorizontalVelocity(currentPowerStats->maxHorizontalSpeed);
	}
	else {
		VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
		if (currentPowerStats->deceleration * delta < horizontalVelocity.Length()) {
			deltaMovement = CalculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
				-VEC3(velocityVector->x, 0, velocityVector->z), currentPowerStats->deceleration, currentPowerStats->maxHorizontalSpeed);

			TransferVelocityToDirectionAndAccelerate(delta, false, -VEC3(velocityVector->x, 0, velocityVector->z), currentPowerStats->deceleration);
		}
		else {
			velocityVector->x = 0.f;
			velocityVector->z = 0.f;
		}
	}

	if (isTurnAround) {
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::TurnAround);
	}
	else {
		VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };

		if (horizontalVelocity.Length() == 0.f) {
			GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Idle);
		}
		else if (horizontalVelocity.Length() <= GetPlayerModel()->walkingSpeed) {
			GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Walk);
		}
	}

}

void RunActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	SetPose();
	dbg("Entrando en run\n");
}

void RunActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	dbg("Saliendo de run\n");
}

void RunActionState::OnJumpHighButton() {
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquat);
}

void RunActionState::OnJumpLongButton() {
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void RunActionState::OnLeavingGround() {
	//Set state a alguno por defecto, luego las clases derivadas de esta ya sabrán qué hacer
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}
