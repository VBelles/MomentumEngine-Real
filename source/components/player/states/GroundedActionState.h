#pragma once

#include "components/player/states/IActionState.h"

class GroundedActionState : public IActionState {
protected:
	float backwardsdMinAngle = deg2rad(130.f);
	float backwardsMaxDotProduct;

public:
	GroundedActionState(StateManager* stateManager, State state);
	GroundedActionState(StateManager* stateManager, ConcurrentState state);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onStrongAttackButton() override;
	void onFastAttackButton() override;
	void onReleasePowerButton() override;
	void onDodgeButton() override;
	void onMove(MoveState& moveState) override;
	void onDamage(const TMsgAttackHit& msg) override;

	virtual void onLeavingGround();
};