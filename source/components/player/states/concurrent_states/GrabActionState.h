#pragma once

#include "components/player/comp_player_model.h"
#include "components/player/states/AirborneActionState.h"


class GrabActionState : public AirborneActionState {
protected:
	CTimer timer;
	int warmUpFrames = 2;
	int activeFrames = 12;
	int endingLagFrames = 15;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;
	float lockDuration = 0.8f;

	int IASAFrames = 25;//Interruptible As Soon As
	float interruptibleTime;

	CHandle hitboxHandle;
	int damage = 0;
	AttackPhases phase = AttackPhases::Startup;

	uint32_t enemyId = getID("enemy");

	float powerToGet = 1000.f;

public:
	GrabActionState(CHandle playerModelHandle, CHandle hitbox);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}

	void onLanding() override;

	void setPose() override;

	void onHitboxEnter(CHandle entity) override {}
};