#pragma once

#include "components/player/comp_player_model.h"
#include "components/player/states/AirborneActionState.h"


class HuggingWallActionState : public AirborneActionState {
private:
	PxControllerShapeHit hit;
public:
	HuggingWallActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	void SetPose() override { /*GetRender()->setMesh("data/meshes/pose_landing.mesh");*/ }
	void SetHit(PxControllerShapeHit hit) { this->hit = hit; }
};