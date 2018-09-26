#pragma once

#include "components/player/states/GroundedActionState.h"
#include "components/player/states/AttackState.h"

class StrongAttackActionState : public GroundedActionState, public AttackState {
private:
	//parameters set in constructor

	float beginLauncherTime = frames2sec(20);
	
	float powerToGet = 3000.f;
	float damage = 2.f;
	float suspensionTime = 0.3f;
	VEC2 launchSpeed = { 12.f, 3.f };

	bool fromRun = false;
	bool isSlashOut = false;

	CTimer movementTimer;
	
	std::string animation = "strong_attack";
	std::string animationIdle = "idle";

public:
	StrongAttackActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onStrongAttackButton() override;
	void onStrongAttackButtonReleased() override;
	void onFastAttackButton() override;
	void setMovementInput(VEC2 input) override;
	void onDodgeButton() override;
	void onReleasePowerButton() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};
