#include "mcv_platform.h"
#include "OnAirborneAction.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"

OnAirborneAction::OnAirborneAction(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int OnAirborneAction::execAction(float delta) {
	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	enemy->airborne = true;
	return Leave;
}
