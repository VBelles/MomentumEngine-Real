#include "mcv_platform.h"
#include "OnHorizontalLaunchAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

OnHorizontalLaunchAction::OnHorizontalLaunchAction(Dreidel* dreidle): dreidel(dreidel) {
}

int OnHorizontalLaunchAction::execAction(float delta) {
	return 0;
}
