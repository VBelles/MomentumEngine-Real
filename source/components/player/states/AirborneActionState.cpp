#include "mcv_platform.h"
#include "AirborneActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/base_states/wall_jump/HuggingWallActionState.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"



void AirborneActionState::update(float delta) {
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
		//aceleración según sentido de movimiento
		float appliedAcceleration = calculateAccelerationAccordingToDirection(enterFront, desiredDirection,
			enteringPowerStats->airAcceleration, backwardsMaxDotProduct, sidewaysMaxDotProduct, backwardsAirDriftFactor,
			sidewaysAirDriftFactor);

		deltaMovement += calculateHorizontalDeltaMovement(delta, VEC3{ velocityVector->x , 0 , velocityVector->z },
			desiredDirection, appliedAcceleration,
			enteringPowerStats->maxHorizontalSpeed);
		transferVelocityToDirectionAndAccelerate(delta, false, desiredDirection, appliedAcceleration);
		clampHorizontalVelocity(enteringPowerStats->maxHorizontalSpeed);
	}
	else {
		deltaMovement.x = velocityVector->x * delta;
		deltaMovement.z = velocityVector->z * delta;
	}

	if (velocityVector->y < 0) {
		getPlayerModel()->maxVerticalSpeed = enteringPowerStats->maxVelocityVertical;
		getPlayerModel()->setGravityMultiplier(enteringPowerStats->fallingMultiplier);
	}
}

void AirborneActionState::onStateEnter(IActionState * lastState) {
	IActionState::onStateEnter(lastState);
	enteringPowerStats = &*getPlayerModel()->getPowerStats();
	isTurnAround = false;
	getPlayerModel()->maxVerticalSpeed = getPlayerModel()->maxVelocityUpwards;
	getPlayerModel()->resetGravity();
	enterFront = getPlayerTransform()->getFront();
	sidewaysMaxDotProduct = cos(deg2rad(sidewaysdMinAngle));
	backwardsMaxDotProduct = cos(deg2rad(backwardsdMinAngle));

	isTryingToSlide = false;
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

void AirborneActionState::onMove(MoveState& moveState) {
	//Hugging wall
	for (HitState& hitState : moveState.sideHits) {
		if (velocityVector->y < 0.f && (getPlayerModel()->lastWallNormal.dot(hitState.hit.worldNormal) < 0.8f
			|| getPlayerModel()->sameNormalReattachTimer.elapsed() >= getPlayerModel()->sameNormalReattachTime)) {

			VEC3 hitNormal = toVec3(hitState.hit.worldNormal);

			VEC3 worldInput = getCamera()->TransformToWorld(getPlayerModel()->baseState->getMovementInput());
			if (worldInput.Dot(-hitNormal) >= getPlayerModel()->attachWallByInputMinDot
				|| getPlayerTransform()->getFront().Dot(-hitNormal) >= getPlayerModel()->attachWallByFrontMinDot) {
				float pitch = asin(-hitNormal.y);
				if (pitch >= getPlayerModel()->huggingWallMinPitch && pitch <= getPlayerModel()->huggingWallMaxPitch) {
					HuggingWallActionState* actionState = getPlayerModel()->getBaseState<HuggingWallActionState*>(TCompPlayerModel::ActionStates::HuggingWall);
					actionState->setHuggingWallNormal(hitNormal);
					getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::HuggingWall);
				}
			}
		}
	}
	/*for (HitState& hitState : moveState.sideHits) {
		//Restar inercia
		VEC3 hitNormal = toVec3(hitState.hit.worldNormal);

		//float angle = acos(hitNormal.Dot(VEC3::Up));

		VEC2 normal = VEC2(hitNormal.x, hitNormal.z);
		VEC2 velocity2 = VEC2(getPlayerModel()->getVelocityVector()->x, getPlayerModel()->getVelocityVector()->z);
		VEC2 velocityNormal = VEC2(velocity2);
		velocityNormal.Normalize();

		VEC2 perpendicularNormal = VEC2(-normal.y, normal.x);

		float dot = normal.Dot(velocityNormal);
		float det = normal.x * velocityNormal.y - normal.y * velocityNormal.x;
		float a = atan2(det, dot);

		velocity2 = perpendicularNormal * (sin(a) * velocity2.Length());

		velocityVector->x = velocity2.x;
		velocityVector->z = velocity2.y;
	}*/

	if (moveState.isTouchingTop && velocityVector->y > 0.f) {
		velocityVector->y = 0.f;
	}
	if (moveState.isTouchingBot) {
		if (!isWalkable(moveState)) {
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Slide);
		}
		else {
			onLanding();
		}
	}


}


void AirborneActionState::onLanding() {
	//Ir a landing action state
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Landing);
}


void AirborneActionState::onDamage(float damage, bool isHard) {
	IActionState::onDamage(damage, isHard);
	//getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::SoftKnockbackAir);
}
