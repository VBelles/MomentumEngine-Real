#include "mcv_platform.h"
#include "OnDeathAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

OnDeathAction::OnDeathAction(Dreidel* dreidle): dreidel(dreidel) {
}

int OnDeathAction::execAction(float delta) {
	return 0;
}
