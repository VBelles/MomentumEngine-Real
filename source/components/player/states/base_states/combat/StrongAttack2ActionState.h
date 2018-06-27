#pragma once

#include "components/player/states/GroundedActionState.h"
#include "components/player/states/AttackState.h"

class StrongAttack2ActionState : public GroundedActionState, public AttackState {
private:
	//parameters set in constructor
	
	float powerToGet = 3000.f;
	float damage = 2.f;

	float suspensionTime = 0.5f;
	VEC2 launchSpeed = { 10.5f, 10.f };

	bool isSlashOut = false;

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