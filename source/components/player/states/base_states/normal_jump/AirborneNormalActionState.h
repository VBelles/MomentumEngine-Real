#pragma once

#include "components/player/states/AirborneActionState.h"

class AirborneNormalActionState : public AirborneActionState {
public:
	AirborneNormalActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	void setPose() override;


};