#pragma once

#include "components/player/comp_player_model.h"
#include "components/player/states/AirborneActionState.h"

class WallJumpPlummetActionState : public AirborneActionState {
protected:
	CTimer timer;
	float endingTime;
	float deceleration = 0.5f;

	CHandle hitboxHandle;
	int damage = 3;
	float powerToGet = 4000.f;

public:
	WallJumpPlummetActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onFastAttackButton() override;
	void onStrongAttackButton() override;
	//void onReleasePowerButton() override {}

	void setPose() override { getRender()->setMesh("data/meshes/pose_long_jump.mesh"); }
	virtual void onLanding() override;
	void onHitboxEnter(CHandle entity) override;
};