#pragma once

#include "components/player/states/GroundedActionState.h"

class StrongAttack2ActionState : public GroundedActionState, public AttackState {
private:
	//parameters set in constructor
	
	float powerToGet = 3000.f;
	float damage = 2.f;
	float stunTime = frames2sec(64);

	float suspensionTime = 0.0f;
	VEC2 launchSpeed = { 4.0f, 6.0f };

	bool isSlashOut = false;
	bool isUltraSlashOut = false;

	CTimer movementTimer;

	std::string animation = "strong_attack2";

public:
	StrongAttack2ActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onStrongAttackButton() override;
	void onFastAttackButton() override;
	void setMovementInput(VEC2 input) override;
	void onDodgeButton() override;
	void onReleasePowerButton() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};
