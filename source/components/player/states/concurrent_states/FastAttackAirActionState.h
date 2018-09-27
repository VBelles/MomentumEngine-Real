#pragma once

#include "components/player/states/AirborneActionState.h"
#include "components/player/states/AttackState.h"

class FastAttackAirActionState : public AirborneActionState, public AttackState {
private:
	CTimer timer;
	float hitboxOutTime = frames2sec(10);
	float hitEndTime = frames2sec(7);
	float animationEndTime = frames2sec(10);

	float damage = 1.f;
	float stunTime = 1.5f;
	AttackPhases phase = AttackPhases::Launch;

	float powerToGet = 1000.f;

	bool isSlashOut = false;
	bool isUltraSlashOut = false;
	CTimer movementTimer;

	std::string animation = "strong_attack2";
	std::string hitbox = "fast_attack_air";

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
