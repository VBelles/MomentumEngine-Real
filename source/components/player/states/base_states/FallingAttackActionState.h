#pragma once

#include "components/player/comp_player_model.h"
#include "../AirborneActionState.h"


class FallingAttackActionState : public AirborneActionState {
	CTimer timer;
	int warmUpFrames = 12;
	float hitboxOutTime;
	float fallingMultiplier = 2.5f;
	float maxFallingVelocity = 55.f;
	float upwardsVelocity = 18.f;

	CHandle hitboxHandle;
	float damage = 2.f;

	float powerToGet = 4000.f;

public:
	FallingAttackActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}

	void onLanding() override;
	void setPose() override;
	void onHitboxEnter(CHandle entity) override;
};