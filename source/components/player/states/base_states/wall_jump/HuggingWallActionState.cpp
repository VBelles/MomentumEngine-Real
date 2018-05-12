#include "mcv_platform.h"
#include "HuggingWallActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "skeleton/comp_skeleton.h"
#include "modules/game/physics/basic_query_filter_callback.h"


HuggingWallActionState::HuggingWallActionState(CHandle playerModelHandle) :
	AirborneActionState(playerModelHandle, "bajandopared"),
	animationClimbing("correporlapared"),
	huggingWallMinPitch(cosf(getPlayerModel()->huggingWallMinPitch + M_PI_2)) {
}


void HuggingWallActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	//delta movement horizontal según wall normal
	float yaw, pitch;
	getYawPitchFromVector(fromPhysx(wallNormal), &yaw, &pitch);
	VEC3 tangentVector;
	if (CheckIfHuggingWall(wallDirection)) {
		VEC3 worldInput = getCamera()->TransformToWorld(movementInput);
		VEC3 normal = { wallNormal.x, 0.f, wallNormal.z };
		if (worldInput.Dot(normal) >= releaseWallMinDotProduct && movementInput != VEC2::Zero) {
			if (!isTryingToRelease) {
				isTryingToRelease = true;
				releaseWallTimer.reset();
			}
		}
		else {
			isTryingToRelease = false;
		}
		if (isTryingToRelease && releaseWallTimer.elapsed() >= releaseWallTime) {
			if (!isChangingBaseState) {
				//TurnAround();
				getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
			}
		}
		else if (isClimbing) {
			tangentVector = getVectorFromYawPitch(yaw, pitch + M_PI_2);
			float module = (abs(deltaMovement.y) / abs(tangentVector.y)) * tangentVector.Length();
			deltaMovement = tangentVector * module;
			if (climbTimer.elapsed() >= climbTime) {
				isClimbing = false;
				TurnAround();
				getPlayerModel()->setGravityMultiplier(slideGravityMultiplier);
				getPlayerModel()->maxVerticalSpeed = slideMaxSpeed;
				getPlayerModel()->getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
			}
		}
		else {
			tangentVector = getVectorFromYawPitch(yaw, pitch - M_PI_2);
			float module = (abs(deltaMovement.y) / abs(tangentVector.y)) * tangentVector.Length();
			deltaMovement = tangentVector * module;
		}
	}
	else {
		if (isClimbing) {
			*velocityVector += getPlayerTransform()->getFront() * climbLedgeExitSpeed;
			getPlayerModel()->lastWallNormal = PxVec3(0, 0, 0);
		}
		if (!isChangingBaseState) {
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
		}
	}
}

void HuggingWallActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	FaceWall();
	if (CheckIfHuggingWall(wallDirection)) {
		getPlayerModel()->lastWallNormal = wallNormal;
		*velocityVector = VEC3::Zero;
		isClimbing = true;
		getPlayerModel()->setGravityMultiplier(climbingGravityMultiplier);
		getPlayerModel()->maxVerticalSpeed = climbingMaxSpeed;
		climbTimer.reset();
		getPlayerModel()->getSkeleton()->blendCycle(animationClimbing, 0.2f, 0.2f);
	}
	else {
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	tryingToSlide = false;
}

void HuggingWallActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	getPlayerModel()->sameNormalReattachTimer.reset();
}

void HuggingWallActionState::onJumpHighButton() {
	TurnAround();
	isClimbing = false;
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::WallJumpSquat);
}

void HuggingWallActionState::onJumpLongButton() {
	TurnAround();
	isClimbing = false;
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::WallJumpSquatPlummet);
}

void HuggingWallActionState::onMove(HitState& hitState) {
	if (hitState.isGrounded) {
		float dot = hitState.hit.worldNormal.dot(PxVec3(0, 1, 0));
		if (dot < huggingWallMinPitch) {
			//Continue in hugging wall
		}
		else if (dot < getPlayerModel()->getController()->getSlopeLimit()) {
			if (!tryingToSlide) {
				tryingToSlide = true;
				slideTimer.reset();
			}
			else if (slideTimer.elapsed() >= slideWindowTime) {
				getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Slide);
			}
		}
		else {
			onLanding();
		}
	}
	if (hitState.isTouchingCeiling && velocityVector->y > 0.f) {
		velocityVector->y = 0.f;
	}
}


bool HuggingWallActionState::CheckIfHuggingWall(VEC3 wallDirection) {
	PxScene* scene = EnginePhysics.getScene();
	PxVec3 origin = toPhysx(getPlayerTransform()->getPosition());
	PxVec3 unitDir = PxVec3(wallDirection.x, 0.f, wallDirection.z);
	PxRaycastBuffer raycastBuffer;
	PxQueryFilterData filterData;
	filterData.data = PxFilterData(EnginePhysics.Player, EnginePhysics.Scenario, 0, 0);
	filterData.flags |= PxQueryFlag::eANY_HIT | PxQueryFlag::ePREFILTER;

	bool status = scene->raycast(origin, unitDir, maxRaycastDistance, raycastBuffer,
		PxHitFlags(PxHitFlag::eDEFAULT), filterData, EnginePhysics.getGameQueryFilterCallback());
	if (!status) return false;

	wallNormal = raycastBuffer.block.normal;
	//if (nearest.actor == hit.actor) {
	//	//dbg("same actor\n");
	//}
	//else {
	//	//dbg("not hugging wall\n");
	//	isHuggingWall = false;
	//}
	return true;
}

void HuggingWallActionState::FaceWall() {
	float y, p, r;
	getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	//y = atan2(-hit.worldNormal.x, -hit.worldNormal.z);
	y = atan2(-huggingWallNormal.x, -huggingWallNormal.z);
	getPlayerTransform()->setYawPitchRoll(y, p, r);
	wallDirection = getPlayerTransform()->getFront();
}

void HuggingWallActionState::TurnAround() {
	float y, p, r;
	getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	y = atan2(wallNormal.x, wallNormal.z);
	getPlayerTransform()->setYawPitchRoll(y, p, r);
	wallDirection = -getPlayerTransform()->getFront();
}
