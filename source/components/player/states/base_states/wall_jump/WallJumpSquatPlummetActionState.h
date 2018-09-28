#pragma once

#include "components/player/states/AirborneActionState.h"

class WallJumpSquatPlummetActionState : public AirborneActionState {
protected:
	CTimer timer;
	float endingTime = frames2sec(20);
	float frontVelocity = 30.f;
	float verticalVelocity = -30.f;
	
	std::string animation = "wall_jump_plummet";
	std::string animationHugging = "hugging_wall";

public:
	WallJumpSquatPlummetActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onDodgeButton() override {}
	void onReleasePowerButton() override {}
};
