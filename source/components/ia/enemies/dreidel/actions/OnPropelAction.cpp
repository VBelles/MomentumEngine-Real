#include "mcv_platform.h"
#include "OnPropelAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

OnPropelAction::OnPropelAction(Dreidel* dreidle): dreidel(dreidel) {
}

int OnPropelAction::execAction(float delta) {
	return 0;
}
