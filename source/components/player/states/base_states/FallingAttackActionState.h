#pragma once

#include "components/player/states/AirborneActionState.h"
#include "components/player/states/AttackState.h"

class FallingAttackActionState : public AirborneActionState, public AttackState {
private:
	CTimer timer;
	float hitboxOutTime = frames2sec(12);
	float fallingMultiplier = 2.5f;
	float maxFallingVelocity = 55.f;
	float upwardsVelocity = 18.f;

	float damage = 2.f;
	float powerToGet = 4000.f;
	float propelDuration = 0.5f;

	std::string animation = "falling_attack";
	std::string animationPositioning = "falling_attack_positioning";

	std::string hitbox = "falling_attack";

public:
	FallingAttackActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onLanding() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};