#include "mcv_platform.h"
#include "ReturnToSpawnCondition.h"
#include "components/ia/enemies/dreidel/Dreidel.h"

ReturnToSpawnCondition::ReturnToSpawnCondition(Dreidel* dreidle): dreidel(dreidel) {
}

bool ReturnToSpawnCondition::testCondition(float delta) {
	return false;
}
