#pragma once

#include "components/player/states/AirborneActionState.h"

class WallJumpPlummetActionState : public AirborneActionState {
protected:
	CTimer timer;
	float endingTime;
	float deceleration = 0.5f;

	float damage = 3;
	float powerToGet = 4000.f;

	std::string hitbox;

public:
	WallJumpPlummetActionState(CHandle playerModelHandle) : AirborneActionState(playerModelHandle, "jump_volando"),
		hitbox("plummet_attack") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onFastAttackButton() override;
	void onStrongAttackButton() override;
	//void onReleasePowerButton() override {}
	void onLanding() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};