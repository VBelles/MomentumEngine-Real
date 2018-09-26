#pragma once

#include "components/player/states/AirborneActionState.h"

class AirDodgeActionState : public AirborneActionState {
private:
	CTimer timer;
	float dodgingTime = frames2sec(12);
	float recoveryTime = frames2sec(30);
	CTimer invencibilityTimer;
	float invencibilityTime = frames2sec(12);
	float takeActionTime = frames2sec(3);
	float dodgeSpeed = 22.f;
	float deceleration = 5.f;

	bool isDodging = true;

	std::string animation = "dodge";
	std::string airborneAnimation = "free_fall";

public:
	AirDodgeActionState(StateManager* stateManager);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onStrongAttackButton() override;
	void onFastAttackButton() override;
	void onDodgeButton() override {}
	void onReleasePowerButton() override;
	void onDamage(const TMsgAttackHit& msg) override;
	void onMove(MoveState& moveState) override;
};