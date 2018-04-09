#include "mcv_platform.h"
#include "HuggingWallActionState.h"

HuggingWallActionState::HuggingWallActionState(CHandle playerModelHandle)
	:AirborneActionState::AirborneActionState(playerModelHandle) {
	animation = "animation";
}

void HuggingWallActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (CheckIfHuggingWall(wallDirection)) {
		VEC3 worldInput = GetCamera()->TransformToWorld(movementInput);
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
			GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
		}
		else if (isClimbing) {
			if(climbTimer.elapsed() >= climbTime){
				isClimbing = false;
				TurnAround();
				GetPlayerModel()->SetGravityMultiplier(slideGravityMultiplier);
				GetPlayerModel()->maxVerticalSpeed = slideMaxSpeed;
			}
		}
	}
	else {
		if (isClimbing) {
			*velocityVector += GetPlayerTransform()->getFront() * climbLedgeExitSpeed;
			GetPlayerModel()->lastWallNormal = PxVec3(0, 0, 0);
		}
		GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
}

void HuggingWallActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	FaceWall();
	if (CheckIfHuggingWall(wallDirection)) {
		GetPlayerModel()->lastWallNormal = wallNormal;
		*velocityVector = VEC3::Zero;
		isClimbing = true;
		GetPlayerModel()->SetGravityMultiplier(climbingGravityMultiplier);
		GetPlayerModel()->maxVerticalSpeed = climbingMaxSpeed;
		SetPose();
		climbTimer.reset();
		GetPlayerModel()->getSkeleton()->executeAction(animation);
	}
	else {
		GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
}

void HuggingWallActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	GetPlayerModel()->sameNormalReattachTimer.reset();
}

void HuggingWallActionState::OnJumpHighButton() {
	TurnAround();
	isClimbing = false;
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::WallJumpSquat);
}

void HuggingWallActionState::OnJumpLongButton() {
	TurnAround();
	isClimbing = false;
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::WallJumpSquatPlummet);
}

bool HuggingWallActionState::CheckIfHuggingWall(VEC3 wallDirection) {
	PxScene* scene = Engine.getPhysics().getScene();
	PxVec3 origin = { GetPlayerTransform()->getPosition().x,GetPlayerTransform()->getPosition().y, GetPlayerTransform()->getPosition().z };

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
	GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	y = atan2(-hit.worldNormal.x, -hit.worldNormal.z);
	GetPlayerTransform()->setYawPitchRoll(y, p, r);
	wallDirection = GetPlayerTransform()->getFront();
}

void HuggingWallActionState::TurnAround() {
	float y, p, r;
	GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	y = atan2(wallNormal.x, wallNormal.z);
	GetPlayerTransform()->setYawPitchRoll(y, p, r);
	wallDirection = -GetPlayerTransform()->getFront();
}
