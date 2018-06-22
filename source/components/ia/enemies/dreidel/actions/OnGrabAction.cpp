#include "mcv_platform.h"
#include "OnGrabAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

OnGrabAction::OnGrabAction(Dreidel* dreidle): dreidel(dreidel) {
}

int OnGrabAction::execAction(float delta) {
	return 0;
}
