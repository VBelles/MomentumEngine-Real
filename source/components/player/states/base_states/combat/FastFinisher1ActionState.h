#pragma once

#include "components/player/states/GroundedActionState.h"
#include "components/player/states/AttackState.h"

class FastFinisher1ActionState : public GroundedActionState, public AttackState {
private:
	//parameters set in constructor

	float beginLauncherTime = frames2sec(20);

	float powerToGet = 3000.f;
	float damage = 2.f;
	float stunTime = 2.0f;

	float suspensionTime = 2.0f;
	VEC2 launchSpeed = { 30.f, 6.f };
	bool isSlashOut = false;

	CTimer movementTimer;

	std::string animation = "fast_finisher1";

public:
	FastFinisher1ActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onStrongAttackButton() override {}
	void onFastAttackButton() override {}
	void onDodgeButton() override;
	void onReleasePowerButton() override {};
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};