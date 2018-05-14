#pragma once

#include "components/player/states/GroundedActionState.h"

class Hitboxes;

class FastAttackActionState : public GroundedActionState {
private:
	CTimer timer;
	float hitboxOutTime = frames2sec(10);
	float hitEndTime = frames2sec(15);
	float animationEndTime = frames2sec(5);

	float interruptibleTime = frames2sec(13);

	float beginLauncherTime = frames2sec(15);

	float damage = 1.f;
	AttackPhases phase = AttackPhases::Launch;

	float powerToGet = 2000.f;

	std::string hitbox;


public:
	FastAttackActionState(CHandle playerModelHandle) 
		: GroundedActionState(playerModelHandle, "melee"), hitbox("fast_attack") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void setMovementInput(VEC2 input) override {}
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onFastAttackButtonReleased() override;
	void onLeavingGround() override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;

};