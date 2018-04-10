#include "mcv_platform.h"
#include "HuggingWallActionState.h"
#include "components/comp_render.h"

HuggingWallActionState::HuggingWallActionState(CHandle playerModelHandle)
	:AirborneActionState::AirborneActionState(playerModelHandle) {
}

void HuggingWallActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
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
			//TurnAround();
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
		}
		else if (isClimbing) {
			if(climbTimer.elapsed() >= climbTime){
				isClimbing = false;
				TurnAround();
				getPlayerModel()->setGravityMultiplier(slideGravityMultiplier);
				getPlayerModel()->maxVerticalSpeed = slideMaxSpeed;
			}
		}
	}
	else {
		if (isClimbing) {
			*velocityVector += getPlayerTransform()->getFront() * climbLedgeExitSpeed;
			getPlayerModel()->lastWallNormal = PxVec3(0, 0, 0);
		}
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
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
		setPose();
		climbTimer.reset();
	}
	else {
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
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

void HuggingWallActionState::setPose() {
	getRender()->setMesh("data/meshes/pose_jump.mesh");
}

bool HuggingWallActionState::CheckIfHuggingWall(VEC3 wallDirection) {
	PxScene* scene = Engine.getPhysics().getScene();
	PxVec3 origin = { getPlayerTransform()->getPosition().x,getPlayerTransform()->getPosition().y, getPlayerTransform()->getPosition().z };

	const PxU32 bufferSize = 256;
	PxRaycastHit hitBuffer[bufferSize];
	PxRaycastBuffer buf(hitBuffer, bufferSize);

	PxVec3 unitDir = { wallDirection.x, 0.f, wallDirection.z};

	bool status = scene->raycast(origin, unitDir, maxRaycastDistance, buf);
	if (!status) return false;
	PxRaycastHit nearest = buf.touches[0];
	nearest.distance = maxRaycastDistance + 1;
	for (PxU32 i = 0; i < buf.nbTouches; i++) {
		if (!buf.touches[i].shape->getFlags().isSet(PxShapeFlag::eTRIGGER_SHAPE)) {
			if (buf.touches[i].distance < nearest.distance) {
				nearest = buf.touches[i];
			}
		}
	}
	bool isHuggingWall = status;
	wallNormal = nearest.normal;
	//if (nearest.actor == hit.actor) {
	//	//dbg("same actor\n");
	//}
	//else {
	//	//dbg("not hugging wall\n");
	//	isHuggingWall = false;
	//}
	return isHuggingWall;
}

void HuggingWallActionState::FaceWall() {
	float y, p, r;
	getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	y = atan2(-hit.worldNormal.x, -hit.worldNormal.z);
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
