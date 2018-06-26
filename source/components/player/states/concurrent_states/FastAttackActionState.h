#pragma once

#include "components/player/states/GroundedActionState.h"
#include "components/player/states/AttackState.h"

class FastAttackActionState : public GroundedActionState, public AttackState {
private:
	CTimer timer;
	float hitboxOutTime = frames2sec(10);
	float hitEndTime = frames2sec(15);
	float animationEndTime = frames2sec(40);
	float beginLauncherTime = frames2sec(15);

	AttackPhases phase = AttackPhases::Launch;

	float damage = 1.f;
	float powerToGet = 2000.f;
	float suspensionTime = 0.0f;
	VEC2 launchSpeed = { 0.5f, 8.f };

	std::string animation = "fast_attack";
	std::string hitbox = "fast_attack";

public:
	FastAttackActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override;
	void onStrongAttackButton() override;
	void onDodgeButton() override;
	void onReleasePowerButton() override;
	void onFastAttackButtonReleased() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};