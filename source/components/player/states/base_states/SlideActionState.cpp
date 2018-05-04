#include "mcv_platform.h"
#include "SlideActionState.h"
#include "components/player/comp_player_model.h"
#include "skeleton/comp_skeleton.h"

void SlideActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	float yaw, pitch;
	getYawPitchFromVector(hitNormal, &yaw, &pitch);
	VEC3 tangentVector = getVectorFromYawPitch(yaw, pitch - M_PI_2);
	float module = (abs(deltaMovement.y) / abs(tangentVector.y)) * tangentVector.Length();
	deltaMovement = tangentVector * module;

}

void SlideActionState::onStateEnter(IActionState* lastState) {
	AirborneActionState::onStateEnter(lastState);
	getPlayerModel()->getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
	velocityVector->y = 0.f;
	velocityVector->x = 0.f;
	velocityVector->z = 0.f;
	isTryingToLand = false;
}

void SlideActionState::onStateExit(IActionState* nextState) {
	AirborneActionState::onStateExit(nextState);
}

void SlideActionState::onMove(HitState& hitState) {
	if (!hitState.hasHit) { //Not grounded, change to airborne normal
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	else { //Grounded, check slope
		dbg("normal.y: %f\n", hitState.hit.worldNormal.y);
		if (!isTryingToLand) {
			isTryingToLand = true;
			landingWindowTimer.reset();
		}
		else if (landingWindowTimer.elapsed() >= landingWindowTime) {
			float dot = hitState.hit.worldNormal.dot(PxVec3(0, 1, 0));
			if (dot >= getPlayerModel()->getController()->getSlopeLimit()) {
				getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Walk);
			}
		}
		hitNormal = fromPhysx(hitState.hit.worldNormal);
	}

}

