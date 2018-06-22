#include "mcv_platform.h"
#include "GrabbedAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

GrabbedAction::GrabbedAction(Dreidel* dreidle): dreidel(dreidel) {
}

int GrabbedAction::execAction(float delta) {
	return 0;
}
