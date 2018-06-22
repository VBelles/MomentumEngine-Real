#include "mcv_platform.h"
#include "ReturnToSpawnAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

ReturnToSpawnAction::ReturnToSpawnAction(Dreidel* dreidle): dreidel(dreidel) {
}

int ReturnToSpawnAction::execAction(float delta) {
	return 0;
}
