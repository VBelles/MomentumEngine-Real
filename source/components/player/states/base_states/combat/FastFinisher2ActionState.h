#pragma once

#include "components/player/states/GroundedActionState.h"
#include "components/player/states/AttackState.h"

class FastFinisher2ActionState : public GroundedActionState, public AttackState {
private:
	//parameters set in constructor

	float beginLauncherTime = frames2sec(20);

	float powerToGet = 3000.f;
	float damage = 5.f;

	std::string animation = "melee_strong";

public:
	FastFinisher2ActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onStrongAttackButton() override {}
	void onFastAttackButton() override {}
	void onDodgeButton() override;
	void onReleasePowerButton() override {};
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};