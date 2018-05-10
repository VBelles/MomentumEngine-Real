#include "mcv_platform.h"
#include "SlideActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "skeleton/comp_skeleton.h"

void SlideActionState::update(float delta) {
	//AirborneActionState::update(delta);
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;

	bool hasInput = movementInput != VEC2::Zero;
	VEC3 desiredDirection = getCamera()->TransformToWorld(movementInput);
	desiredDirection.y = 0.f;
	float yaw, pitch;
	getYawPitchFromVector(hitNormal, &yaw, &pitch);
	VEC3 tangentVector = getVectorFromYawPitch(yaw, pitch - M_PI_2);


	float module = (abs(deltaMovement.y) / abs(tangentVector.y)) * tangentVector.Length();
	deltaMovement = tangentVector * module;

	if (hasInput) {
		VEC3 sidewaysVector;
		//float yawDifference = abs(getYawFromVector(desiredDirection) - (yaw + M_PI_2));
		//if (yawDifference < M_PI_2 || yawDifference > 3.f * M_PI_2) {
		//	sidewaysVector = getVectorFromYawPitch(yaw + M_PI_2, pitch);//derecha mirando hacia la pred
		//}
		//else {
		//	sidewaysVector = getVectorFromYawPitch(yaw - M_PI_2, pitch);//izquierda mirando hacia la pred
		//}
		sidewaysVector = getVectorFromYawPitch(yaw + M_PI_2, pitch);
		sidewaysVector.y = 0.f;
		sidewaysVector.Normalize();
		sidewaysVector *= sidewaysVector.Dot(desiredDirection);
		deltaMovement += sidewaysVector * sidewaysSlidingVelocity;
	}

	if (deltaMovement.Length() > maxVerticalSlidingVelocity) {
		deltaMovement.Normalize();
		deltaMovement *= maxVerticalSlidingVelocity;
	}

	TCompTransform* transform = getPlayerTransform();
	transform->getYawPitchRoll(&yaw, &pitch);
	transform->setYawPitchRoll(getYawFromVector(hitNormal), pitch);
}

void SlideActionState::onStateEnter(IActionState* lastState) {
	AirborneActionState::onStateEnter(lastState);
	TCompTransform* transform = getPlayerTransform();
	float yaw, pitch;
	transform->getYawPitchRoll(&yaw, &pitch);
	transform->setYawPitchRoll(getYawFromVector(hitNormal), pitch);

	getPlayerModel()->maxVerticalSpeed = maxVerticalSlidingVelocity;

	getPlayerModel()->getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
	velocityVector->y = 0.f;
	velocityVector->x = 0.f;
	velocityVector->z = 0.f;
	isTryingToLand = false;
}

void SlideActionState::onStateExit(IActionState* nextState) {
	AirborneActionState::onStateExit(nextState);
}

void SlideActionState::onMove(MoveState& moveState) {
	if (!moveState.isTouchingBot) { //Not grounded, change to airborne normal
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	else { //Grounded, check slope
		bool grounded = true;
		for (HitState& hitState : moveState.hits) {
			if (hitState.dotUp < getPlayerModel()->getController()->getSlopeLimit()) {
				grounded = false;
				break;
			}
		}
		if (grounded) {
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Walk);
		}
		else {
			if (!moveState.hits.empty()) {
				hitNormal = fromPhysx(moveState.hits[moveState.hits.size() - 1].hit.worldNormal);
			}
			else {
				dbg("Wops\n");
			}
		}
	}

}

