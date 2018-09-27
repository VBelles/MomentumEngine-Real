#include "mcv_platform.h"
#include "HuggingWallActionState.h"
#include "modules/game_modules/physics/basic_query_filter_callback.h"


HuggingWallActionState::HuggingWallActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, HuggingWall),
	huggingWallMinPitch(cosf(static_cast<float>(getPlayerModel()->huggingWallMinPitch + M_PI_2))) {
}

void HuggingWallActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	//delta movement horizontal según wall normal
	float yaw, pitch;
	getYawPitchFromVector(toVec3(wallNormal), &yaw, &pitch);
	VEC3 tangentVector;
	if (CheckIfHuggingWall(wallDirection)) {
		VEC3 worldInput = getCamera()->getCamera()->TransformToWorld(movementInput);
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
			if (!stateManager->isChangingBaseState) {
				//TurnAround();
				stateManager->changeState(AirborneNormal);
			}
		}
		else if (isClimbing) {
			tangentVector = getVectorFromYawPitch(yaw, static_cast<float>(pitch + M_PI_2));
			float module = (abs(deltaMovement.y) / abs(tangentVector.y)) * tangentVector.Length();
			deltaMovement = tangentVector * module;
			if (climbTimer.elapsed() >= climbTime) {
				isClimbing = false;
				TurnAround();
				getPlayerModel()->setGravityMultiplier(slideGravityMultiplier);
				getPlayerModel()->maxVerticalSpeed = slideMaxSpeed;
				getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
				huggingWallSound = EngineSound.emitFollowingEvent(SOUND_HUGGING_WALL, getPlayerTransform());
			}
		}
		else {
			tangentVector = getVectorFromYawPitch(yaw, static_cast<float>(pitch - M_PI_2));
			float module = (abs(deltaMovement.y) / abs(tangentVector.y)) * tangentVector.Length();
			deltaMovement = tangentVector * module;
		}
	}
	else {
		if (isClimbing) {
			*velocityVector += getPlayerTransform()->getFront() * climbLedgeExitSpeed;
			getPlayerModel()->lastWallNormal = PxVec3(0, 0, 0);
		}
		if (!stateManager->isChangingBaseState) {
			stateManager->changeState(AirborneNormal);
		}
	}
}

void HuggingWallActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	FaceWall();
	if (CheckIfHuggingWall(wallDirection)) {
		stateManager->changeConcurrentState(Free);
		getPlayerModel()->lastWallNormal = wallNormal;
		*velocityVector = VEC3::Zero;
		isClimbing = true;
		getPlayerModel()->setGravityMultiplier(climbingGravityMultiplier);
		getPlayerModel()->maxVerticalSpeed = climbingMaxSpeed;
		climbTimer.reset();
		getSkeleton()->blendCycle(animationClimbing, 0.2f, 0.2f);
	}
	else {
		stateManager->changeState(AirborneNormal);
	}

}

void HuggingWallActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	getPlayerModel()->sameNormalReattachTimer.reset();
	EngineSound.stopEvent(huggingWallSound);
}

void HuggingWallActionState::onJumpHighButton() {
	TurnAround();
	isClimbing = false;
	stateManager->changeState(WallJumpSquat);
}

void HuggingWallActionState::onJumpLongButton() {
	TurnAround();
	isClimbing = false;
	stateManager->changeState(WallJumpSquatPlummet);
}

void HuggingWallActionState::onMove(MoveState& moveState) {
	if (moveState.isTouchingBot) {
		if (isWalkable(moveState)) { //Slide
			stateManager->changeState(Slide);
		}
		else { //Land
			onLanding();
		}
	}
	else if (moveState.isTouchingTop && velocityVector->y > 0.f) {
		velocityVector->y = 0.f;
		//reset timer para hugging wall
		getPlayerModel()->lockHuggingWallTimer.reset();
	}
}


bool HuggingWallActionState::CheckIfHuggingWall(VEC3 wallDirection) {
	PxScene* scene = EnginePhysics.getScene();
	PxVec3 origin = toPxVec3(getPlayerTransform()->getPosition());
	PxVec3 unitDir = PxVec3(wallDirection.x, 0.f, wallDirection.z);
	PxRaycastBuffer raycastBuffer;
	PxQueryFilterData filterData;
	filterData.data = PxFilterData(EnginePhysics.Player, EnginePhysics.Scenario, 0, 0);
	filterData.flags |= PxQueryFlag::eANY_HIT | PxQueryFlag::ePREFILTER;

	bool status = scene->raycast(origin, unitDir, maxRaycastDistance, raycastBuffer,
		PxHitFlags(PxHitFlag::eDEFAULT), filterData, EnginePhysics.getGameQueryFilterCallback());
	if (!status) return false;

	wallNormal = raycastBuffer.block.normal;

	float yaw, pitch;
	getYawPitchFromVector(toVec3(wallNormal), &yaw, &pitch);
	if (pitch > huggingWallMinPitch) {
		return false;
	}

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
