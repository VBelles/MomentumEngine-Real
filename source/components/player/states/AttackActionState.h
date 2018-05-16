#pragma once

#include "components/player/states/IActionState.h"

class AttackActionState : public IActionState {
protected:
	CTimer timer;
	float hitboxOutTime;
	float hitEndTime;
	float animationEndTime;

	float damage;
	AttackPhases phase = AttackPhases::Startup;

	std::string animation;

	std::string hitbox;

public:
	AttackActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
};