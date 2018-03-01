#include "mcv_platform.h"
#include "GroundedActionState.h"

GroundedActionState::GroundedActionState(CHandle adf)
	: IActionState::IActionState(adf) {
	accelerationVector = GetPlayerModel()->GetAccelerationVector();
	velocityVector = GetPlayerModel()->GetVelocityVector();
}

void GroundedActionState::update(float delta) {
	bool hasInput = movementInput != VEC2::Zero;
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();

	VEC3 desiredDirection = GetPlayerModel()->GetCamera()->TransformToWorld(movementInput);
	/*VEC3 front = GetPlayerModel()->GetTransform()->getFront();
	if (front.Dot(desiredDirection) <= backwardsMaxDotProduct) {
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::TurnAround);*/
	if (false) {
	}
	else {
		//Buscamos un punto en la dirección en la que el jugador querría ir y, según si queda a izquierda o derecha, rotamos
		if (hasInput) {
			VEC3 targetPos = GetPlayerTransform()->getPosition() + desiredDirection;
			RotatePlayerTowards(delta, targetPos, currentPowerStats->rotationSpeed);
		}

		deltaMovement = VEC3::Zero;
		//Si hay input se traslada toda la velocidad antigua a la nueva dirección de front y se le añade lo acelerado
		if (hasInput) {
			deltaMovement = CalculateHorizontalDeltaMovement(delta, GetPlayerTransform()->getFront(),
				GetPlayerTransform()->getFront(), currentPowerStats->acceleration,
				currentPowerStats->maxHorizontalSpeed);

			TransferVelocityToDirectionAndAccelerate(delta, true, GetPlayerTransform()->getFront(), currentPowerStats->acceleration);
			ClampHorizontalVelocity(currentPowerStats->maxHorizontalSpeed);
		}
		else {
			//Cuando no hay input se frena (TODO no frenar de golpe, desacelerar)
			velocityVector->x = 0.f;
			velocityVector->z = 0.f;
		}
	}
	//distancia vertical recorrida
	currentPowerStats->currentGravityMultiplier = velocityVector->y < 0 ? currentPowerStats->fallingMultiplier : currentPowerStats->normalGravityMultiplier;
	deltaMovement.y = CalculateVerticalDeltaMovement(delta, accelerationVector->y * currentPowerStats->currentGravityMultiplier, currentPowerStats->maxVelocityVertical);

	//Nueva velocidad vertical y clampeo
	velocityVector->y += accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta;
	velocityVector->y = clamp(velocityVector->y, -currentPowerStats->maxVelocityVertical, currentPowerStats->maxVelocityVertical);
}

void GroundedActionState::OnStateEnter(IActionState * lastState) {
	IActionState::OnStateEnter(lastState);
	backwardsMaxDotProduct = cos(deg2rad(backwardsdMinAngle));
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
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquat);
}

void GroundedActionState::OnJumpLongButton() {
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void GroundedActionState::OnStrongAttackButton() {
	if (GetPlayerModel()->IsAttackFree()) {
		GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::StrongAttack);
	}
}

void GroundedActionState::OnFastAttackButton() {
	if (GetPlayerModel()->IsAttackFree()) {
		GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::FastAttack);
	}
}

void GroundedActionState::OnLeavingGround() {
	//Set state a alguno por defecto, luego las clases derivadas de esta ya sabrán qué hacer
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}
