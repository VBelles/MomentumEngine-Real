#pragma once

#include "components/player/states/IActionState.h"

class DeathActionState : public IActionState {
private:
	bool finish = false;
public:
	DeathActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	void respawn();

};