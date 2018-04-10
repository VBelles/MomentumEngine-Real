#pragma once

#include "components/player/comp_player_model.h"
#include "../AirborneActionState.h"


class ReleasePowerAirActionState : public AirborneActionState {
private:
	CTimer timer;
	int warmUpFrames = 7;
	int activeFrames = 13;
	int endingLagFrames = 10;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;
	int buttonPresses = 1;
	float stunDuration = 3.f;

	int IASAFrames = 25;//Interruptible As Soon As
	float interruptibleTime;

	CHandle hitboxSmallHandle;
	CHandle hitboxBigHandle;
	float damage = 0.f;
	AttackPhases phase = AttackPhases::Startup;

	uint32_t enemyId = getID("enemy");

public:
	ReleasePowerAirActionState(CHandle playerModelHandle, CHandle hitboxSmall, CHandle hitboxBig);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override;


	void onLanding() override;

	void setPose() override { getRender()->setMesh("data/meshes/pose_grab.mesh"); }

	void onHitboxEnter(CHandle entity) override;
};