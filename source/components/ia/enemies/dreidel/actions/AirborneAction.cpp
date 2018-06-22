#include "mcv_platform.h"
#include "AirborneAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

AirborneAction::AirborneAction(Dreidel* dreidle): dreidel(dreidel) {
}

int AirborneAction::execAction(float delta) {
	return 0;
}
