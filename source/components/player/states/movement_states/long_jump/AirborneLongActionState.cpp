#include "mcv_platform.h"
#include "AirborneLongActionState.h"

AirborneLongActionState::AirborneLongActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void AirborneLongActionState::update (float delta) {
	bool hasInput = movementInput != VEC2::Zero;

	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();

	VEC3 desiredDirection = GetPlayerModel()->GetCamera()->TransformToWorld(movementInput);

	if (hasInput) {
		//aceleraci�n seg�n sentido de movimiento
		float appliedAcceleration = CalculateAccelerationAccordingToDirection(enterFront, desiredDirection,
			currentPowerStats->acceleration, backwardsMaxDotProduct, sidewaysMaxDotProduct, backwardsAirDriftFactor,
			sidewaysAirDriftFactor);

		deltaMovement = CalculateHorizontalDeltaMovement(delta, VEC3{ velocityVector->x , 0 , velocityVector->z },
			desiredDirection, appliedAcceleration, currentPowerStats->longJumpVelocityVector.z);

		TransferVelocityToDirectionAndAccelerate(delta, false, desiredDirection, appliedAcceleration);
		ClampHorizontalVelocity(currentPowerStats->longJumpVelocityVector.z);
	}
	else {
		deltaMovement.x = velocityVector->x * delta;
		deltaMovement.z = velocityVector->z * delta;
	}

	currentPowerStats->currentGravityMultiplier = currentPowerStats->longGravityMultiplier;
	deltaMovement.y = CalculateVerticalDeltaMovement(delta, accelerationVector->y * currentPowerStats->currentGravityMultiplier, currentPowerStats->maxVelocityVertical);

	velocityVector->y += accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta;
	velocityVector->y = clamp(velocityVector->y, -currentPowerStats->maxVelocityVertical, currentPowerStats->maxVelocityVertical);
}

void AirborneLongActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	//dbg("Entrando en airborne long\n");
	enterFront = GetPlayerTransform()->getFront();
	sidewaysMaxDotProduct = cos(deg2rad(sidewaysdMinAngle));
	backwardsMaxDotProduct = cos(deg2rad(backwardsdMinAngle));
	GetRender()->setMesh("data/meshes/pose_long_jump.mesh");
}

void AirborneLongActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	//dbg("Saliendo de airborne long\n");
}

void AirborneLongActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

void AirborneLongActionState::OnJumpHighButton() {
	GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::GrabHigh);
}

void AirborneLongActionState::OnJumpLongButton() {
	GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::GrabLong);
}

void AirborneLongActionState::OnLanding() {
	//Ir a landing action state
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Run);
}
