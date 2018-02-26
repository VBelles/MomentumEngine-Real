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

	//Buscamos un punto en la dirección en la que el jugador querría ir y, según si queda a izquierda o derecha, rotamos
	if (hasInput) {
		VEC3 desiredDirection = currentCamera->TransformToWorld(movementInput);
		VEC3 targetPos = playerTransform->getPosition() + desiredDirection;
		RotatePlayerTowards(delta, targetPos, currentPowerStats->rotationSpeed);
	}

	deltaMovement = VEC3::Zero;
	//Si hay input se traslada toda la velocidad antigua a la nueva dirección de front y se le añade lo acelerado
	if (hasInput) {
		deltaMovement = CalculateHorizontalDeltaMovement(delta, playerTransform->getFront(),
						playerTransform->getFront(), currentPowerStats->acceleration,
						currentPowerStats->maxHorizontalSpeed);

		TransferVelocityToDirectionAndAccelerate(delta, true, playerTransform->getFront(), currentPowerStats->acceleration);
		ClampHorizontalVelocity(currentPowerStats->maxHorizontalSpeed);
	}
	else {
		//Cuando no hay input se frena (TODO no frenar de golpe, desacelerar)
		velocityVector->x = 0.f;
		velocityVector->z = 0.f;
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
