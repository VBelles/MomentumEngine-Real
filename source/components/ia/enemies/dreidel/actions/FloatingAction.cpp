#include "mcv_platform.h"
#include "FloatingAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

FloatingAction::FloatingAction(Dreidel* dreidle): dreidel(dreidel) {
}

int FloatingAction::execAction(float delta) {
	return 0;
}
