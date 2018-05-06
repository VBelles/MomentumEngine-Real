#pragma once

#include "components/player/states/AirborneActionState.h"

class FallingAttackActionState : public AirborneActionState {
private:
	CTimer timer;
	int warmUpFrames = 12;
	float hitboxOutTime;
	float fallingMultiplier = 2.5f;
	float maxFallingVelocity = 55.f;
	float upwardsVelocity = 18.f;

	CHandle hitboxHandle;
	float damage = 2.f;

	float powerToGet = 4000.f;

	std::string hitbox = "falling_attack";
	std::string animationPositioning = "jump_inicio";

public:
	FallingAttackActionState(CHandle playerModelHandle) : AirborneActionState(playerModelHandle, "ataquecaida") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onLanding() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};