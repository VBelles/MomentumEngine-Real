#pragma once

#include "components/player/states/AirborneActionState.h"

class HuggingWallActionState : public AirborneActionState {
private:
	bool isClimbing = true;
	CTimer climbTimer;
	float climbTime = 0.4f;
	CTimer releaseWallTimer;
	float releaseWallTime = 0.14f;
	float releaseWallMinDotProduct = -0.2f;
	bool isTryingToRelease = false;

	float climbingGravityMultiplier = -4.f;//Negativo
	float climbingMaxSpeed = 6.f;
	float slideGravityMultiplier = 8.f;
	float slideMaxSpeed = 5.f;

	float climbLedgeExitSpeed = 2.2f;

	PxReal maxRaycastDistance = 0.5f;
	PxControllerShapeHit hit;
	PxVec3 wallNormal;
	VEC3 wallDirection;//Front or back

	VEC3 huggingWallNormal;
public:
	HuggingWallActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;

	void setPose() override;
	void SetHit(PxControllerShapeHit hit) { this->hit = hit; }
	void setHuggingWallNormal(VEC3 huggingWallNormal) { this->huggingWallNormal = huggingWallNormal; }
	bool CheckIfHuggingWall(VEC3 wallDirection);
	void FaceWall();
	void TurnAround();

	virtual void onShapeHit(const PxControllerShapeHit &hit) override {}
};