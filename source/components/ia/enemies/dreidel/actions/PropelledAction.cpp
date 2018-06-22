#include "mcv_platform.h"
#include "PropelledAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

PropelledAction::PropelledAction(Dreidel* dreidle): dreidel(dreidel) {
}

int PropelledAction::execAction(float delta) {
	return 0;
}
