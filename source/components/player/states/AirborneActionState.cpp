#include "mcv_platform.h"
#include "AirborneActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/base_states/wall_jump/HuggingWallActionState.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "components/player/states/StateManager.h"


AirborneActionState::AirborneActionState(StateManager* stateManager, State state)
	: IActionState(stateManager, state) {
};

AirborneActionState::AirborneActionState(StateManager* stateManager, ConcurrentState state)
	: IActionState(stateManager, state) {
};

void AirborneActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;

	VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
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
}

void AirborneActionState::onStateExit(IActionState * nextState) {
	IActionState::onStateExit(nextState);
}

void AirborneActionState::onJumpHighButton() {
	stateManager->changeConcurrentState(GrabHigh);
}

void AirborneActionState::onJumpLongButton() {
	stateManager->changeConcurrentState(GrabLong);
}

void AirborneActionState::onStrongAttackButton() {
	stateManager->changeState(FallingAttack);
}

void AirborneActionState::onFastAttackButton() {
	if (stateManager->isConcurrentActionFree()) {
		stateManager->changeConcurrentState(FastAttackAir);
	}
}

void AirborneActionState::onReleasePowerButton() {
	if (stateManager->isConcurrentActionFree()) {
		stateManager->changeConcurrentState(ReleasePowerAir);
	}
}

void AirborneActionState::onMove(MoveState& moveState) {
	//Hugging wall
	for (HitState& hitState : moveState.sideHits) {
		if (velocityVector->y < 0.f && (getPlayerModel()->lastWallNormal.dot(hitState.hit.worldNormal) < 0.8f
			|| getPlayerModel()->sameNormalReattachTimer.elapsed() >= getPlayerModel()->sameNormalReattachTime)) {

			VEC3 hitNormal = toVec3(hitState.hit.worldNormal);

			VEC3 worldInput = getCamera()->getCamera()->TransformToWorld(stateManager->getState()->getMovementInput());
			if (worldInput.Dot(-hitNormal) >= getPlayerModel()->attachWallByInputMinDot
				|| getPlayerTransform()->getFront().Dot(-hitNormal) >= getPlayerModel()->attachWallByFrontMinDot) {
				float pitch = asin(-hitNormal.y);
				if (pitch >= getPlayerModel()->huggingWallMinPitch && pitch <= getPlayerModel()->huggingWallMaxPitch) {
					HuggingWallActionState* actionState = (HuggingWallActionState*) stateManager->getState(HuggingWall);
					actionState->setHuggingWallNormal(hitNormal);
					stateManager->changeState(HuggingWall);
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
			stateManager->changeState(Slide);
		}
		else {
			onLanding();
		}
	}


}


void AirborneActionState::onLanding() {
	//Ir a landing action state
	stateManager->changeState(Landing);
}


void AirborneActionState::onDamage(const TMsgAttackHit& msg) {
	if (msg.info.stun) {
		stateManager->changeState(HardKnockbackAir);
	}
	else {
		stateManager->changeConcurrentState(SoftKnockbackAir);
	}
	IActionState::onDamage(msg);//Hacemos esto al final para sobreescribir el estado de muerte

}
