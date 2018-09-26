#pragma once

#include "components/player/states/GroundedActionState.h"
#include "components/player/states/AttackState.h"

class ReleasePowerGroundActionState : public GroundedActionState, public AttackState {
private:
	CTimer timer;
	float hitboxOutTime = frames2sec(7);
	float hitEndTime = frames2sec(26);
	float animationEndTime = frames2sec(27);
	int buttonPresses = 1;

	AttackPhases phase = AttackPhases::Startup;

	float damage = 0.f;
	float stunTime = 3.f;

	std::string animation = "release_power";

	std::string smallHitbox = "release_power_small";
	std::string bigHitbox = "release_power_big";

public:
	ReleasePowerGroundActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onDodgeButton() override;
	void onReleasePowerButton() override;
	void onDamage(const TMsgAttackHit& msg) override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};