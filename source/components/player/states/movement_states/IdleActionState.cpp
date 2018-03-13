#include "mcv_platform.h"
#include "IdleActionState.h"

IdleActionState::IdleActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void IdleActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput != VEC2::Zero;
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();

	//Buscamos un punto en la dirección en la que el jugador querría ir y, según si queda a izquierda o derecha, rotamos
	VEC3 desiredDirection = GetPlayerModel()->GetCamera()->TransformToWorld(movementInput);
	bool isTurnAround = GetPlayerModel()->GetTransform()->getFront().Dot(desiredDirection) <= backwardsMaxDotProduct;
	if (hasInput && !isTurnAround) {
		VEC3 targetPos = GetPlayerTransform()->getPosition() + desiredDirection;
		RotatePlayerTowards(delta, targetPos, currentPowerStats->rotationSpeed);
	}

	//Si hay input se traslada toda la velocidad antigua a la nueva dirección de front y se le añade lo acelerado
	if (hasInput) {
		deltaMovement += CalculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			GetPlayerTransform()->getFront(), currentPowerStats->acceleration,
			currentPowerStats->maxHorizontalSpeed);

		TransferVelocityToDirectionAndAccelerate(delta, true, GetPlayerTransform()->getFront(), currentPowerStats->acceleration);
		ClampHorizontalVelocity(currentPowerStats->maxHorizontalSpeed);
	}

	if (isTurnAround) {
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::TurnAround);
	}
	if (velocityVector->x > 0.f || velocityVector->z > 0.f) {
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Run);
	}
}

void IdleActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	SetPose();
	dbg("Entrando en idle\n");
}

void IdleActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	dbg("Saliendo de idle\n");
}

void IdleActionState::OnJumpHighButton() {
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquat);
}

void IdleActionState::OnJumpLongButton() {
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void IdleActionState::OnLeavingGround() {
	//Set state a alguno por defecto, luego las clases derivadas de esta ya sabrán qué hacer
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}
