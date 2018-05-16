#pragma once

#include "components/player/states/GroundedActionState.h"

class StrongAttackActionState : public GroundedActionState {
private:
	CTimer timer;
	float hitboxOutTime = frames2sec(60);
	float hitEndTime = frames2sec(10);
	float animationEndTime = frames2sec(60);

	float interruptibleTime = frames2sec(60);

	float beginLauncherTime = frames2sec(20);

	AttackPhases phase = AttackPhases::Launch;
	
	float powerToGet = 3000.f;
	float damage = 5.f;

	std::string animation = "melee_strong";

	std::string hitbox = "strong_attack";

public:
	StrongAttackActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onStrongAttackButton() override {}
	void onStrongAttackButtonReleased() override;
	void onFastAttackButton() override {}
	void onReleasePowerButton() override {};
	void onLeavingGround() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};