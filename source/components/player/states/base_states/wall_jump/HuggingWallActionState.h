#pragma once

#include "components/player/comp_player_model.h"
#include "components/player/states/AirborneActionState.h"


class HuggingWallActionState : public AirborneActionState {
private:
	bool isClimbing = true;
	CTimer climbTimer;
	float climbTime = 0.4f;
	CTimer releaseWallTimer;
	float releaseWallTime = 0.14f;
	float releaseWallMinDotProduct = 0.5f;
	bool isTryingToRelease = false;

	float climbingGravityMultiplier = -4.f;//Negativo
	float climbingMaxSpeed = 6.f;
	float slideGravityMultiplier = 8.f;
	float slideMaxSpeed = 5.f;

	float climbLedgeExitSpeed = 2.2f;

	PxReal maxRaycastDistance = 1.f;
	PxControllerShapeHit hit;
	PxVec3 wallNormal;
	VEC3 wallDirection;//Front or back
public:
	HuggingWallActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	void SetPose() override { GetRender()->setMesh("data/meshes/pose_jump.mesh"); }
	void SetHit(PxControllerShapeHit hit) { this->hit = hit; }
	bool CheckIfHuggingWall(VEC3 wallDirection);
	void FaceWall();
	void TurnAround();
};