#pragma once

#include "components/player/states/AirborneActionState.h"
#include "components/player/states/AttackState.h"

class ReleasePowerAirActionState : public AirborneActionState, public AttackState {
private:
	CTimer timer;
	float hitboxOutTime = frames2sec(7);
	float hitEndTime = frames2sec(26);
	float animationEndTime = frames2sec(20);
	int buttonPresses = 1;

	AttackPhases phase = AttackPhases::Startup;

	float stunDuration = 3.f;
	float damage = 0.f;

	std::string smallHitbox = "release_power_small";
	std::string bigHitbox = "release_power_big";
	std::string animation = "release_power_air";

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
	void onDamage(const TMsgAttackHit& msg) override;
	void onLanding() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};
