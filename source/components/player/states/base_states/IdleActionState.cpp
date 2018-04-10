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

	//Buscamos un punto en la direcci�n en la que el jugador querr�a ir y, seg�n si queda a izquierda o derecha, rotamos
	VEC3 desiredDirection = GetCamera()->TransformToWorld(movementInput);
	bool isTurnAround = GetPlayerModel()->GetTransform()->getFront().Dot(desiredDirection) <= backwardsMaxDotProduct;
	if (hasInput && !isTurnAround) {
		VEC3 targetPos = GetPlayerTransform()->getPosition() + desiredDirection;
		RotatePlayerTowards(delta, targetPos, currentPowerStats->rotationSpeed);
	}

	//Si hay input se traslada toda la velocidad antigua a la nueva direcci�n de front y se le a�ade lo acelerado
	if (hasInput) {
		deltaMovement += CalculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			GetPlayerTransform()->getFront(), currentPowerStats->acceleration,
			GetPlayerModel()->walkingSpeed);

		TransferVelocityToDirectionAndAccelerate(delta, true, GetPlayerTransform()->getFront(), currentPowerStats->acceleration);
		ClampHorizontalVelocity(GetPlayerModel()->walkingSpeed);
	}

	VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
	if (isTurnAround) {
		GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::IdleTurnAround);
	}
	else if (horizontalVelocity.Length() > 0.f) {
		GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Walk);
	}
}

void IdleActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	SetPose();
}

void IdleActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
}
