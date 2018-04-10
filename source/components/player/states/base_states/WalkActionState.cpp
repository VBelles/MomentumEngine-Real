#include "mcv_platform.h"
#include "WalkActionState.h"

WalkActionState::WalkActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void WalkActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput != VEC2::Zero;
	PowerStats* currentPowerStats = GetPlayerModel()->getPowerStats();
	float desiredVelocity = currentPowerStats->maxHorizontalSpeed;
	bool wantToWalk = false;
	if (movementInput.Length() < 0.8f) {
		desiredVelocity = GetPlayerModel()->walkingSpeed;
		wantToWalk = true;
	}

	//Buscamos un punto en la direcci�n en la que el jugador querr�a ir y, seg�n si queda a izquierda o derecha, rotamos
	VEC3 desiredDirection = getCamera()->TransformToWorld(movementInput);
	bool isTurnAround = GetPlayerModel()->getTransform()->getFront().Dot(desiredDirection) <= backwardsMaxDotProduct;
	if (hasInput && !isTurnAround) {
		VEC3 targetPos = GetPlayerTransform()->getPosition() + desiredDirection;
		RotatePlayerTowards(delta, targetPos, currentPowerStats->rotationSpeed);
	}

	//Si hay input se traslada toda la velocidad antigua a la nueva direcci�n de front y se le a�ade lo acelerado
	if (hasInput) {
		deltaMovement += CalculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			GetPlayerTransform()->getFront(), currentPowerStats->acceleration,
			desiredVelocity);

		TransferVelocityToDirectionAndAccelerate(delta, true, GetPlayerTransform()->getFront(), currentPowerStats->acceleration);
		ClampHorizontalVelocity(desiredVelocity);
	}
	else {
		VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
		if (currentPowerStats->deceleration * delta < horizontalVelocity.Length()) {
			deltaMovement = CalculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
				-VEC3(velocityVector->x, 0, velocityVector->z), currentPowerStats->deceleration, desiredVelocity);

			TransferVelocityToDirectionAndAccelerate(delta, false, -VEC3(velocityVector->x, 0, velocityVector->z), currentPowerStats->deceleration);
		}
		else {
			velocityVector->x = 0.f;
			velocityVector->z = 0.f;
		}
	}
	
	if (isTurnAround) {
		GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::TurnAround);
	}
	else {
		VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };

		if (horizontalVelocity.Length() == 0.f) {
			GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Idle);
		}
		else if(!wantToWalk && horizontalVelocity.Length() > GetPlayerModel()->walkingSpeed){
			GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Run);
		}
	}

}

void WalkActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	SetPose();
}

void WalkActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
}
