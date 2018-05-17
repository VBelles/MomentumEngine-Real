#pragma once

#include "components/player/states/AirborneActionState.h"
#include "components/player/states/AttackActionState.h"

class GrabActionState : public AirborneActionState, public AttackActionState {
protected:
	CTimer timer;
	float hitboxOutTime = frames2sec(2);
	float hitEndTime = frames2sec(12);
	float animationEndTime = frames2sec(15);
	float lockDuration = 0.8f;
	float interruptibleTime = frames2sec(25);

	AttackPhases phase = AttackPhases::Startup;

	int damage = 0;
	float powerToGet = 1000.f;

	std::string animation = "melee";
	std::string hitbox = "grab";

public:
	GrabActionState(StateManager* stateManager, ConcurrentState state);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onLanding() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override {}
};