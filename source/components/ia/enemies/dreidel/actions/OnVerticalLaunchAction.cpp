#include "mcv_platform.h"
#include "OnVerticalLaunchAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

OnVerticalLaunchAction::OnVerticalLaunchAction(Dreidel* dreidle): dreidel(dreidel) {
}

int OnVerticalLaunchAction::execAction(float delta) {
	return 0;
}
