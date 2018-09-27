#include "mcv_platform.h"
#include "AirborneActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/base_states/wall_jump/HuggingWallActionState.h"
#include "components/controllers/comp_camera_player.h"
#include "components/player/states/StateManager.h"
#include "components/player/states/AttackState.h"


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
	getPlayerModel()->isWalking = false;
	enterFront = getPlayerTransform()->getFront();
	sidewaysMaxDotProduct = cos(deg2rad(sidewaysdMinAngle));
	backwardsMaxDotProduct = cos(deg2rad(backwardsdMinAngle));
	getCameraPlayer()->moveCameraCloser(false);
	slideWindowTimer.reset();
	hardLandingTimer.reset();
	if (lastState->state == PropelHigh || lastState->state == PropelLong) {
		getPlayerModel()->lockFallingAttack = false;
		getPlayerModel()->lockAirDodge = false;
	}
	else if (lastState->state == FallingAttack) {
		getPlayerModel()->lockFallingAttack = true;
	}
	else if (lastState->state == AirDodge) {
		getPlayerModel()->lockAirDodge = true;
	}
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
	if (!getPlayerModel()->lockFallingAttack) {
		stateManager->changeState(FallingAttack);
	}
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

void AirborneActionState::onDodgeButton() {
	if (!getPlayerModel()->lockAirDodge) {
		stateManager->changeState(AirDodge);
	}
}

void AirborneActionState::onMove(MoveState& moveState) {
	//Try to hug wall
	for (const HitState& hitState : moveState.allHits) {
		if (hugWall(hitState)) break;
	}
	//Touching top
	if (moveState.isTouchingTop && velocityVector->y > 0.f) {
		velocityVector->y = 0.f;
	}
	//Land or slide
	if (moveState.isTouchingBot) {
		if (!isWalkable(moveState)) {
			if (slideWindowTimer.elapsed() >= slideWindowTime) {
				stateManager->changeState(Slide);
				getPlayerModel()->lockFallingAttack = false;
				getPlayerModel()->lockAirDodge = false;
				*velocityVector = VEC3::Zero;
			}
		}
		else {
			onLanding();
		}
	}
	else {
		slideWindowTimer.reset();
	}
}

bool AirborneActionState::hugWall(const HitState& hitState) {
	if (getPlayerModel()->lockHuggingWallTimer.elapsed() >= getPlayerModel()->lockHuggingWallTime) {
		if (velocityVector->y < 0.f && (getPlayerModel()->lastWallNormal.dot(hitState.hit.worldNormal) < 0.8f
			|| getPlayerModel()->sameNormalReattachTimer.elapsed() >= getPlayerModel()->sameNormalReattachTime)) {

			VEC3 hitNormal = toVec3(hitState.hit.worldNormal);

			VEC3 worldInput = getCamera()->getCamera()->TransformToWorld(stateManager->getState()->getMovementInput());
			if (worldInput.Dot(-hitNormal) >= getPlayerModel()->attachWallByInputMinDot
				|| getPlayerTransform()->getFront().Dot(-hitNormal) >= getPlayerModel()->attachWallByFrontMinDot) {
				float pitch = asin(-hitNormal.y);
				if (pitch >= getPlayerModel()->huggingWallMinPitch && pitch <= getPlayerModel()->huggingWallMaxPitch) {
					HuggingWallActionState* actionState = (HuggingWallActionState*)stateManager->getState(HuggingWall);
					actionState->setHuggingWallNormal(hitNormal);
					stateManager->changeState(HuggingWall);
					getPlayerModel()->lockFallingAttack = false;
					getPlayerModel()->lockAirDodge = false;
					return true;
				}
			}
		}
	}
	return false;
}


void AirborneActionState::onLanding() {
	//Ir a landing action state
	stateManager->changeConcurrentState(Free);
	if (hardLandingTimer.elapsed() < hardLandingTime) {
		stateManager->changeState(SoftLanding);
	}
	else {
		stateManager->changeState(HardLanding);
	}
}


void AirborneActionState::onDamage(const TMsgAttackHit& msg) {
	bool hasSuperArmor = false;
	bool hasInvulnerability = false;
	if (AttackState* attackState = dynamic_cast<AttackState*>(this)) {
		hasSuperArmor = attackState->hasSuperArmor();
		hasInvulnerability = attackState->hasInvulnerability();
	}
	if (!hasSuperArmor && !hasInvulnerability) {
		if (msg.info.stun) {
			stateManager->changeState(HardKnockbackAir);
		}
		else if (stateManager->getConcurrentState()->state != GrabHigh && stateManager->getConcurrentState()->state != GrabLong) {
			stateManager->changeConcurrentState(SoftKnockbackAir);
		}
	}
	else if (!hasInvulnerability) {
		//super armor sound
	}

	if (!hasInvulnerability) {
		IActionState::onDamage(msg);//Hacemos esto al final para sobreescribir el estado de muerte
	}
	else {
		//invulnerable sound
	}
}
