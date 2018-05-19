#pragma once

#include "components/player/states/GroundedActionState.h"
#include "components/player/states/AttackState.h"

class ReleasePowerGroundActionState : public GroundedActionState, public AttackState {
private:
	CTimer timer;
	float hitboxOutTime = frames2sec(7);
	float hitEndTime = frames2sec(26);
	float animationEndTime = frames2sec(20);
	int buttonPresses = 1;

	float interruptibleTime = frames2sec(25);

	AttackPhases phase = AttackPhases::Startup;

	float damage = 0.f;
	float stunDuration = 3.f;

	std::string animation = "liberarenergia";

	std::string smallHitbox = "release_power_small";
	std::string bigHitbox = "release_power_big";

public:
	ReleasePowerGroundActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};