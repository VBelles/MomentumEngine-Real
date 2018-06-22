#include "mcv_platform.h"
#include "OnIdleWarAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

OnIdleWarAction::OnIdleWarAction(Dreidel* dreidle): dreidel(dreidel) {
}

int OnIdleWarAction::execAction(float delta) {
	return 0;
}
