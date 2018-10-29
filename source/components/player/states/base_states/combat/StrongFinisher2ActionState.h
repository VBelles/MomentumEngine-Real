#pragma once

#include "components/player/states/GroundedActionState.h"

class StrongFinisher2ActionState : public GroundedActionState, public AttackState {
private:
	//parameters set in constructor

	float powerToGet = 3000.f;
	float damage = 3.f;
	float stunTime = 2.f;
	float suspensionTime = 2.0f;
	bool isSlashOut = false;

	CTimer2 movementTimer;

	std::string animation = "strong_finisher2";

public:
	StrongFinisher2ActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onStrongAttackButton() override {}
	void onFastAttackButton() override {}
	void onDodgeButton() override;
	void onReleasePowerButton() override {};
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};
