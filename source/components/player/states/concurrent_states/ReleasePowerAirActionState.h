#pragma once

#include "components/player/states/AirborneActionState.h"
#include "components/player/states/AttackActionState.h"

class ReleasePowerAirActionState : public AirborneActionState, public AttackActionState{
private:
	CTimer timer;
	float hitboxOutTime = frames2sec(7);
	float hitEndTime = frames2sec(26);
	float animationEndTime = frames2sec(20);
	int buttonPresses = 1;

	float interruptibleTime = frames2sec(25);

	AttackPhases phase = AttackPhases::Startup;

	float stunDuration = 3.f;
	float damage = 0.f;

	std::string smallHitbox = "release_power_small";
	std::string bigHitbox = "release_power_big";
	std::string animation = "liberarenergia";

public:
	ReleasePowerAirActionState(StateManager* stateManager);

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
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};