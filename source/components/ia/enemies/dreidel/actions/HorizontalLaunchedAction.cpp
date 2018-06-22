#include "mcv_platform.h"
#include "HorizontalLaunchedAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

HorizontalLaunchedAction::HorizontalLaunchedAction(Dreidel* dreidle): dreidel(dreidel) {
}

int HorizontalLaunchedAction::execAction(float delta) {
	return 0;
}
