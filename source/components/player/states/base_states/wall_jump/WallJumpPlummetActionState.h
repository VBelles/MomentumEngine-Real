#pragma once

#include "components/player/states/AirborneActionState.h"
#include "components/player/states/AttackState.h"

class WallJumpPlummetActionState : public AirborneActionState, public AttackState {
protected:
	CTimer timer;
	float endingTime;
	float deceleration = 0.5f;

	float damage = 3;
	float powerToGet = 4000.f;
	float propelDuration = 0.5f;

	bool isSlashOut = false;
	CTimer movementTimer;

	std::string animation = "wall_jump_plummet_loop";

	std::string hitbox = "plummet_attack";

public:
	WallJumpPlummetActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onFastAttackButton() override;
	void onStrongAttackButton() override;
	//void onReleasePowerButton() override {}
	void onLanding() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};