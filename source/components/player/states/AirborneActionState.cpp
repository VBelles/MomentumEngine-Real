#include "mcv_platform.h"
#include "AirborneActionState.h"
//#include "../comp_player_model.h"
#include "base_states/wall_jump/HuggingWallActionState.h"

AirborneActionState::AirborneActionState(CHandle playerModelHandle)
	: IActionState::IActionState(playerModelHandle) {
	accelerationVector = getPlayerModel()->getAccelerationVector();
	velocityVector = getPlayerModel()->getVelocityVector();
}

void AirborneActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput != VEC2::Zero;	
	
	VEC3 desiredDirection = getCamera()->TransformToWorld(movementInput);
	if (!getPlayerModel()->lockTurning) {
		if (!isTurnAround) {
			if (hasInput) {
				if (getPlayerTransform()->getFront().Dot(desiredDirection) > backwardsMaxDotProduct) {
					VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
					rotatePlayerTowards(delta, targetPos, rotationSpeed);
				}
				else {
					isTurnAround = true;
					//calculate rotation speed
					exitYaw = atan2(desiredDirection.x, desiredDirection.z);
					float y, p, r;
					getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
					turnAroundRotationSpeed = (exitYaw - y) / turnAroundTime;
					turnAroundTimer.reset();
				}
			}
		}
		if (isTurnAround) {
			//rotate at rotation speed and set isTurnAround to false when timer arrives
			float y, p, r;
			getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
			if (abs(turnAroundRotationSpeed * delta) < abs(exitYaw - y)) {
				y += turnAroundRotationSpeed * delta;
				getPlayerTransform()->setYawPitchRoll(y, p, r);
			}
			else {
				getPlayerTransform()->setYawPitchRoll(exitYaw, p, r);
			}

			if (turnAroundTimer.elapsed() >= turnAroundTime) {
				isTurnAround = false;
			}
		}
	}

	if (hasInput) {
		//aceleraci�n seg�n sentido de movimiento
		float appliedAcceleration = calculateAccelerationAccordingToDirection(enterFront, desiredDirection,
			enteringPowerStats->airAcceleration, backwardsMaxDotProduct, sidewaysMaxDotProduct, backwardsAirDriftFactor,
			sidewaysAirDriftFactor);

		deltaMovement += calculateHorizontalDeltaMovement(delta, VEC3{ velocityVector->x , 0 , velocityVector->z},
			desiredDirection, appliedAcceleration,
			enteringPowerStats->maxHorizontalSpeed);
		transferVelocityToDirectionAndAccelerate(delta, false, desiredDirection, appliedAcceleration);
		clampHorizontalVelocity(enteringPowerStats->maxHorizontalSpeed);
	}
	else {
		deltaMovement.x = velocityVector->x * delta;
		deltaMovement.z = velocityVector->z * delta;
	}

	if(velocityVector->y < 0){
		getPlayerModel()->maxVerticalSpeed = enteringPowerStats->maxVelocityVertical;
		getPlayerModel()->setGravityMultiplier(enteringPowerStats->fallingMultiplier);
	}
}

void AirborneActionState::onStateEnter(IActionState * lastState) {
	IActionState::onStateEnter(lastState);
	enteringPowerStats = &*getPlayerModel()->getPowerStats();
	isTurnAround = false;
	turnAroundTime = turnAroundFrames * (1.f / 60);
	getPlayerModel()->maxVerticalSpeed = getPlayerModel()->maxVelocityUpwards;
	getPlayerModel()->resetGravity();
	enterFront = getPlayerTransform()->getFront();
	sidewaysMaxDotProduct = cos(deg2rad(sidewaysdMinAngle));
	backwardsMaxDotProduct = cos(deg2rad(backwardsdMinAngle));
}

void AirborneActionState::onStateExit(IActionState * nextState) {
	IActionState::onStateExit(nextState);
}

void AirborneActionState::onJumpHighButton() {
	getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::GrabHigh);
}

void AirborneActionState::onJumpLongButton() {
	getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::GrabLong);
}

void AirborneActionState::onStrongAttackButton() {
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::FallingAttack);
}

void AirborneActionState::onFastAttackButton() {
	if (getPlayerModel()->isConcurrentActionFree()) {
		getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::FastAttackAir);
	}
}

void AirborneActionState::onReleasePowerButton() {
	if (getPlayerModel()->isConcurrentActionFree()) {
		getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::ReleasePowerAir);
	}
}


void AirborneActionState::onLanding() {
	//Ir a landing action state
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Landing);
}

void AirborneActionState::onShapeHit(const PxControllerShapeHit& hit) {
	if (velocityVector->y < 0.f && (getPlayerModel()->lastWallNormal.dot(hit.worldNormal) < 0.8f 
		|| getPlayerModel()->sameNormalReattachTimer.elapsed() >= getPlayerModel()->sameNormalReattachTime)) {

		getPlayerModel()->lastWallEntered = hit.actor;

		VEC3 hitNormal = VEC3(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z);

		VEC3 worldInput = getCamera()->TransformToWorld(getPlayerModel()->baseState->getMovementInput());
		if (worldInput.Dot(-hitNormal) >= getPlayerModel()->attachWallByInputMinDot 
			|| getPlayerTransform()->getFront().Dot(-hitNormal) >= getPlayerModel()->attachWallByFrontMinDot) {
			float pitch = asin(-hit.worldNormal.y);
			if (pitch >= getPlayerModel()->huggingWallMinPitch && pitch <= getPlayerModel()->huggingWallMaxPitch) {
				HuggingWallActionState* actionState = getPlayerModel()->getBaseState<HuggingWallActionState*>(TCompPlayerModel::ActionStates::HuggingWall);
				actionState->SetHit(hit);
				getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::HuggingWall);
			}
		}
	}
}
