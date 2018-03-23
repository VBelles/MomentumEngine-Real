#include "mcv_platform.h"
#include "AirborneActionState.h"

AirborneActionState::AirborneActionState(CHandle playerModelHandle)
	: IActionState::IActionState(playerModelHandle) {
	accelerationVector = GetPlayerModel()->GetAccelerationVector();
	velocityVector = GetPlayerModel()->GetVelocityVector();
}

void AirborneActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput != VEC2::Zero;	
	
	VEC3 desiredDirection = GetCamera()->TransformToWorld(movementInput);
	if (!GetPlayerModel()->lockWalk) {
		if (!isTurnAround) {
			if (hasInput) {
				if (GetPlayerTransform()->getFront().Dot(desiredDirection) > backwardsMaxDotProduct) {
					VEC3 targetPos = GetPlayerTransform()->getPosition() + desiredDirection;
					RotatePlayerTowards(delta, targetPos, rotationSpeed);
				}
				else {
					isTurnAround = true;
					//calculate rotation speed
					exitYaw = atan2(desiredDirection.x, desiredDirection.z);
					float y, p, r;
					GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
					turnAroundRotationSpeed = (exitYaw - y) / turnAroundTime;
					turnAroundTimer.reset();
				}
			}
		}
		if (isTurnAround) {
			//rotate at rotation speed and set isTurnAround to false when timer arrives
			float y, p, r;
			GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
			if (abs(turnAroundRotationSpeed * delta) < abs(exitYaw - y)) {
				y += turnAroundRotationSpeed * delta;
				GetPlayerTransform()->setYawPitchRoll(y, p, r);
			}
			else {
				GetPlayerTransform()->setYawPitchRoll(exitYaw, p, r);
			}

			if (turnAroundTimer.elapsed() >= turnAroundTime) {
				isTurnAround = false;
			}
		}
	}

	if (hasInput) {
		//aceleración según sentido de movimiento
		float appliedAcceleration = CalculateAccelerationAccordingToDirection(enterFront, desiredDirection,
			enteringPowerStats->airAcceleration, backwardsMaxDotProduct, sidewaysMaxDotProduct, backwardsAirDriftFactor,
			sidewaysAirDriftFactor);

		deltaMovement += CalculateHorizontalDeltaMovement(delta, VEC3{ velocityVector->x , 0 , velocityVector->z},
			desiredDirection, appliedAcceleration,
			enteringPowerStats->maxHorizontalSpeed);
		TransferVelocityToDirectionAndAccelerate(delta, false, desiredDirection, appliedAcceleration);
		ClampHorizontalVelocity(enteringPowerStats->maxHorizontalSpeed);
	}
	else {
		deltaMovement.x = velocityVector->x * delta;
		deltaMovement.z = velocityVector->z * delta;
	}

	if(velocityVector->y < 0){
		GetPlayerModel()->SetGravityMultiplier(enteringPowerStats->fallingMultiplier);
	}
}

void AirborneActionState::OnStateEnter(IActionState * lastState) {
	IActionState::OnStateEnter(lastState);
	enteringPowerStats = &*GetPlayerModel()->GetPowerStats();
	isTurnAround = false;
	turnAroundTime = turnAroundFrames * (1.f / 60);
	GetPlayerModel()->maxVerticalSpeed = enteringPowerStats->maxVelocityVertical;
	GetPlayerModel()->ResetGravity();
	enterFront = GetPlayerTransform()->getFront();
	sidewaysMaxDotProduct = cos(deg2rad(sidewaysdMinAngle));
	backwardsMaxDotProduct = cos(deg2rad(backwardsdMinAngle));
}

void AirborneActionState::OnStateExit(IActionState * nextState) {
	IActionState::OnStateExit(nextState);
}

void AirborneActionState::OnJumpHighButton() {
	GetPlayerModel()->SetConcurrentState(TCompPlayerModel::ActionStates::GrabHigh);
}

void AirborneActionState::OnJumpLongButton() {
	GetPlayerModel()->SetConcurrentState(TCompPlayerModel::ActionStates::GrabLong);
}

void AirborneActionState::OnStrongAttackButton() {
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::FallingAttack);
}

void AirborneActionState::OnFastAttackButton() {
	//TODO Ataque rápido aire
}

void AirborneActionState::OnReleasePowerButton() {
	if (GetPlayerModel()->IsConcurrentActionFree()) {
		GetPlayerModel()->SetConcurrentState(TCompPlayerModel::ActionStates::ReleasePowerAir);
	}
}


void AirborneActionState::OnLanding() {
	//Ir a landing action state
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::Landing);
}
