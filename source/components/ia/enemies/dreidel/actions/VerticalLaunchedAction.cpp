#include "mcv_platform.h"
#include "VerticalLaunchedAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

VerticalLaunchedAction::VerticalLaunchedAction(Dreidel* dreidle): dreidel(dreidel) {
}

int VerticalLaunchedAction::execAction(float delta) {
	return 0;
}
