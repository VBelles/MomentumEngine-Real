#include "mcv_platform.h"
#include "RespawnAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

RespawnAction::RespawnAction(Dreidel* dreidle): dreidel(dreidel) {
}

int RespawnAction::execAction(float delta) {
	return 0;
}
