#pragma once

#include "components/player/states/AirborneActionState.h"
#include "components/player/states/AttackState.h"

class FastAttackAirActionState : public AirborneActionState, public AttackState {
private:
	CTimer timer;
	float hitboxOutTime = frames2sec(6);
	float hitEndTime = frames2sec(12);
	float animationEndTime = frames2sec(25);

	float damage = 1.f;
	AttackPhases phase = AttackPhases::Launch;

	float powerToGet = 1000.f;

	std::string animation = "fast_attack";
	std::string hitbox = "fast_attack";

public:
	FastAttackAirActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onFastAttackButtonReleased() override {} //posible comportamiento extra
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};