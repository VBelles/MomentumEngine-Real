#include "mcv_platform.h"
#include "StunnedAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

StunnedAction::StunnedAction(Dreidel* dreidle): dreidel(dreidel) {
}

int StunnedAction::execAction(float delta) {
	return 0;
}
