#include "mcv_platform.h"
#include "IdleWarAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

IdleWarAction::IdleWarAction(Dreidel* dreidle): dreidel(dreidel) {
}

int IdleWarAction::execAction(float delta) {
	return 0;
}
