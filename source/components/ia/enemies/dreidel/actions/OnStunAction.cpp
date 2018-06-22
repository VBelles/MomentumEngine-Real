#include "mcv_platform.h"
#include "OnStunAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

OnStunAction::OnStunAction(Dreidel* dreidle): dreidel(dreidel) {
}

int OnStunAction::execAction(float delta) {
	return 0;
}
