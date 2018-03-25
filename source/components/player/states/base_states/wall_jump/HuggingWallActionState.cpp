#include "mcv_platform.h"
#include "HuggingWallActionState.h"

HuggingWallActionState::HuggingWallActionState(CHandle playerModelHandle)
	:AirborneActionState::AirborneActionState(playerModelHandle) {
}

void HuggingWallActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (CheckIfHuggingWall(wallDirection)) {
		if (isClimbing) {
			if(climbTimer.elapsed() >= climbTime){
				isClimbing = false;
				TurnAround();
				GetPlayerModel()->SetGravityMultiplier(8.f);
				GetPlayerModel()->maxVerticalSpeed = 5.f;
			}
		}
	}
	else {
		GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
}

void HuggingWallActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	//TODO desengancharse poniendo el stick en direcci�n opuesta
	FaceWall();
	if (CheckIfHuggingWall(wallDirection)) {
		*velocityVector = VEC3::Zero;
		isClimbing = true;
		GetPlayerModel()->SetGravityMultiplier(-4.f);
		GetPlayerModel()->maxVerticalSpeed = 6.f;
		SetPose();
		climbTimer.reset();
	}
	else {
		GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
}

void HuggingWallActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
}

void HuggingWallActionState::OnJumpHighButton() {
	TurnAround();
	dbg("wall jump, time = %f\n", climbTimer.elapsed());
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::HuggingWallJumpSquat);
}

void HuggingWallActionState::OnJumpLongButton() {
	TurnAround();
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

bool HuggingWallActionState::CheckIfHuggingWall(VEC3 wallDirection) {
	PxScene* scene = Engine.getPhysics().getScene();
	PxVec3 origin = { GetPlayerTransform()->getPosition().x,GetPlayerTransform()->getPosition().y, GetPlayerTransform()->getPosition().z };

	const PxU32 bufferSize = 256;
	PxRaycastHit hitBuffer[bufferSize];
	PxRaycastBuffer buf(hitBuffer, bufferSize);

	PxReal maxDistance = 1.f;
	PxVec3 unitDir = { wallDirection.x, 0.f, wallDirection.z};

	bool status = scene->raycast(origin, unitDir, maxDistance, buf);
	PxRaycastHit nearest = buf.touches[0];
	nearest.distance = maxDistance + 1;
	for (PxU32 i = 0; i < buf.nbTouches; i++) {
		if (!buf.touches[i].shape->getFlags().isSet(PxShapeFlag::eTRIGGER_SHAPE)) {
			if (buf.touches[i].distance < nearest.distance) {
				nearest = buf.touches[i];
			}
		}
	}
	bool isHuggingWall = true;
	if (nearest.actor == hit.actor) {
		//dbg("same actor\n");
		wallNormal = nearest.normal;
	}
	else {
		//dbg("not hugging wall\n");
		isHuggingWall = false;
	}
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
