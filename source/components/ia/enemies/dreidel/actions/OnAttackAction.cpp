#include "mcv_platform.h"
#include "OnAttackAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

OnAttackAction::OnAttackAction(Dreidel* dreidle): dreidel(dreidel) {
}

int OnAttackAction::execAction(float delta) {
	return 0;
}
