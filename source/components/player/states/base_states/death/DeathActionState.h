#pragma once

#include "components/player/states/IActionState.h"

class DeathActionState : public IActionState {
private:
	CTimer timer;
	float deathTime = frames2sec(95);
	float respawnTime = 2.2f;
	bool finish = false;
	std::string animation = "death";

public:
	DeathActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	virtual void onJumpHighButton() {}
	virtual void onJumpHighButtonReleased() {}
	virtual void onJumpLongButton() {}
	virtual void onStrongAttackButton() {}
	virtual void onStrongAttackButtonReleased() {}
	virtual void onFastAttackButton() {}
	virtual void onDodgeButton() {}
	virtual void onFastAttackButtonReleased() {}
	virtual void onSpendCoinsButton() {}
	virtual void onSpendCoinsButtonReleased() {}
	virtual void onReleasePowerButton() {}
	virtual void onHitboxEnter(std::string hitbox, CHandle entity) {}
	virtual void onDead() {}
	virtual void onDamage(const TMsgAttackHit& msg) {}
	virtual void onMove(MoveState& moveState) {}

	void respawn();
	void copyCam();
};
